
auto i = GETINPUT("input");


auto output = GETOUTPUT("output");




















  
if( false == scratch.initialized() )
  {
    
    float64_t bandwidth_octaves = param_as_float64( params, "bandwidth_octaves" );
    float64_t orientation_theta_rad = deg_to_rad( param_as_float64( params, "orient_theta_deg" ) ); 
    float64_t atten_size_prop = param_as_float64( params, "prop_width_edge_atten" );

    
    
    float64_t f0_cyc_dva = param_as_float64( params, "f0_cyc_dva" );

    
    
    
    
      
    float64_t f0_lambda_dva = 1/f0_cyc_dva;
    float64_t f0_cyc_pix = f0_cyc_dva * i->get_dva_per_pix_wid(); 
    float64_t f0_lambda_pix = 1/f0_cyc_pix;
    
    
    float64_t sig_dva = sigma_from_bandwidth_lambda( bandwidth_octaves, f0_lambda_dva );
    float64_t sig_pix = sig_dva / i->get_dva_per_pix_wid(); 


    
    int ksize_pix = ksize_compute( sig_pix ); 
    
    if( ksize_pix < 7 )
      {
	fprintf(stderr, "REV: WARNING: you should not have a ksize for ori < 7 pixels...won't do anything. (note ksize is %d dva/pixel is %lf)\n", ksize_pix, i->get_dva_per_pix_wid());
      }
    
    if( ksize_pix % 2 == 0 )
      {
	ksize_pix += 1;
      }
    
    double gamma = 1;
    double psi = 0;

    
    cv::Mat tmpr = cv::getGaborKernel(cv::Size(ksize_pix, ksize_pix), sig_pix, orientation_theta_rad, f0_lambda_pix, gamma, psi);
    cv::Mat tmpi = cv::getGaborKernel(cv::Size(ksize_pix, ksize_pix), sig_pix, orientation_theta_rad, f0_lambda_pix, gamma, psi+(CV_PI/2));
    tmpr.copyTo(scratch.cpu_w("gaborkernelre"));
    tmpi.copyTo(scratch.cpu_w("gaborkernelim"));
    
    {
      double remean = cv::mean( scratch.cpu("gaborkernelre") )[0];
      cv::subtract( scratch.cpu("gaborkernelre"), remean, scratch.cpu_w("gaborkernelre") );
      salmap_rv::FiltMat squared;
      cv::multiply( scratch.cpu("gaborkernelre"), scratch.cpu("gaborkernelre"), squared );
      double sum = cv::sum( squared )[0];
      cv::divide( scratch.cpu("gaborkernelre"), sqrt(sum), scratch.cpu_w("gaborkernelre") );
    }
      
    {
      double remean = cv::mean( scratch.cpu("gaborkernelim") )[0];
      cv::subtract( scratch.cpu("gaborkernelim"), remean, scratch.cpu_w("gaborkernelim") );
      salmap_rv::FiltMat squared;
      cv::multiply( scratch.cpu("gaborkernelim"), scratch.cpu("gaborkernelim"), squared );
      double sum = cv::sum( squared )[0];
      cv::divide( scratch.cpu("gaborkernelim"), sqrt(sum), scratch.cpu_w("gaborkernelim") );
    }
    
    
    HELPPRINTF(stdout, "ORIENT_GABOR (%s): F0 cyc dva %lf (lamb %lf dva), cyc pix %lf, lamb pix %lf (img size %ld %ld)\n", nickname.c_str(), f0_cyc_dva ,f0_lambda_dva, f0_cyc_pix, f0_lambda_pix, i->size().width, i->size().height );
    
    
    
    attenuation_filter_cpu_wid( atten_size_prop*f0_lambda_pix, i->cpu(), scratch.cpu_w("atten") ); 
    
    
    
  }

cv::filter2D(i->cpu(), scratch.cpu_w("real"), -1, scratch.cpu("gaborkernelre") ); 
cv::filter2D(i->cpu(), scratch.cpu_w("imag"), -1, scratch.cpu("gaborkernelim") ); 
  
cv::magnitude( scratch.cpu("imag"), scratch.cpu("real"), output->cpu_w() );
  
cv::multiply( output->cpu_w(), scratch.cpu("atten"), output->cpu_w() ); 


output->setMetadata( i->get_time(), i->get_dva_wid(), i->get_dva_hei(), i->get_origaspectratio_wh() );




