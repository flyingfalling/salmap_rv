

auto i = GETINPUT("input");

auto output = GETOUTPUT("output");




















  
if( false == scratch.initialized() )
  {
    float64_t bandwidth_octaves = param_as_float64( params, "bandwidth_octaves" );
    float64_t orientation_theta_rad = deg_to_rad( param_as_float64( params, "orient_theta_deg" ) ); 
    float64_t atten_size_prop = param_as_float64( params, "prop_width_edge_atten" );

    float64_t f0_cyc_dva = param_as_float64( params, "f0_cyc_dva" );

    
    
    
    
    float64_t f0_lambda_dva = (1/f0_cyc_dva);
    float64_t f0_cyc_pix = f0_cyc_dva * i->get_dva_per_pix_wid(); 
    float64_t f0_lambda_pix = 1/f0_cyc_pix;
      
      
      
    
      
    
    cv::Mat cosimg = cv::Mat( i->size(), i->cpu().type() );
    cv::Mat sinimg = cosimg.clone();
    float64_t xoffset = cosimg.size().width / 2;
    float64_t yoffset = cosimg.size().height / 2;

    
    
    

    

    
    
    
    
    
    
    
    float64_t cycmult=2*CV_PI;
    float64_t kx = f0_cyc_pix * cycmult * cos( orientation_theta_rad + CV_PI/2 ); 
    float64_t ky = f0_cyc_pix * cycmult * sin( orientation_theta_rad + CV_PI/2 );
    
    
    for( size_t y=0; y<cosimg.size().height; ++y )
      {
	for(size_t x=0; x<cosimg.size().width; ++x )
	  {
	    float64_t xarg = x - xoffset; 
	    float64_t yarg = y - yoffset; 
	    float64_t arg = kx * xarg + ky * yarg;
	    cosimg.at<float32_t>(y,x) = cos(arg); 
	    sinimg.at<float32_t>(y,x) = sin(arg);
	  }
      }
    
    
    

    
    sinimg.copyTo(scratch.cpu_w("sinimg"));
    cosimg.copyTo(scratch.cpu_w("cosimg"));
    
        
    
    
    
    
    
    
    float64_t sig_dva = sigma_from_bandwidth_lambda( bandwidth_octaves, (f0_lambda_dva) ); 
    float64_t xsigma_pix = sig_dva * i->get_pix_per_dva_wid(); 

#ifdef GAUSS_USE_KERNEL
    
    int ksize_pix = ksize_compute( xsigma_pix );
    
    cv::Mat kern = cv::getGaussianKernel( ksize_pix, xsigma_pix );
    kern.copyTo(scratch.cpu_w("kern"));
#endif

    float64_t attensize_pix = atten_size_prop * f0_lambda_pix;
    
    scratch.float64("sig_pix") = xsigma_pix;
    HELPPRINTF(stdout, "ORIENT_STEERABLE (%s): F0 cyc dva %lf (lamb %lf dva), cyc pix %lf, lamb pix %lf (img size %ld %ld) :: ORI Gauss corresp: (%lf) pix LPF smooth -- Attenuate [%lf] of lambda, i.e. attenuating [%lf] pixels\n", nickname.c_str(), f0_cyc_dva ,f0_lambda_dva, f0_cyc_pix, f0_lambda_pix, i->size().width, i->size().height, xsigma_pix, atten_size_prop, attensize_pix );
      
    
    
    attenuation_filter_cpu_wid( attensize_pix, cosimg, scratch.cpu_w("atten") ); 
    
    
    
  }
  

cv::multiply( i->cpu(), scratch.cpu("cosimg"), scratch.cpu_w("real") );

cv::multiply( i->cpu(), scratch.cpu("sinimg"), output->cpu_w() );


#ifdef GAUSS_USE_KERNEL
cv::sepFilter2D( scratch.cpu("real"), scratch.cpu_w("real"), -1, scratch.cpu("kern"), scratch.cpu("kern") );
cv::sepFilter2D( output->cpu(), output->cpu_w(), -1, scratch.cpu("kern"), scratch.cpu("kern") );
#else
cv::GaussianBlur(scratch.cpu("real"), scratch.cpu_w("real"), cv::Size(0,0), scratch.float64("sig_pix"));
cv::GaussianBlur(output->cpu(), output->cpu_w(), cv::Size(0,0), scratch.float64("sig_pix"));
#endif

cv::magnitude( output->cpu(), scratch.cpu("real"), output->cpu_w() );
  
cv::multiply( output->cpu_w(), scratch.cpu("atten"), output->cpu_w() ); 


output->setMetadata( curr_time, i->get_dva_wid(), i->get_dva_hei(), i->get_origaspectratio_wh() );



