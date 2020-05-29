

#define DOWNSIZE_RECURSIVE






























































const int dinterp=cv::INTER_AREA; 
const int uinterp=cv::INTER_LINEAR;

auto i = GETINPUT("input");

auto output = GETOUTPUT("output");

if( false == scratch.initialized() )
  {
    
    
    
    float64_t targ_nyquist_dva = param_as_float64( params, "targ_nyquist_dva" );

    
  
    
      
    cv::Size pixsize = i->size();
    float64_t curr_nyqf = nyqf_from_dvaperpix( i->get_dva_per_pix_wid() ); 
    
    
    
    
      
      
    float64_t resample_ratio = targ_nyquist_dva / curr_nyqf; 
    
    float64_t allowederr = param_as_float64(params, "resample_grace" );
    int retwidpix=-1;
    int retheipix=-1;

    float64_t reterr = resize_image_w_ratio( pixsize.width, pixsize.height, resample_ratio, allowederr, i->get_origaspectratio_wh(),
					     retwidpix, retheipix);
      
    if( reterr > allowederr)
      {
	fprintf(stderr, "REV: something seriously wrong downsize...\n");
	exit(1);
      }
    cv::Size targsize( retwidpix, retheipix );
    
    scratch.int64( "retwidpix" ) = retwidpix;
    scratch.int64( "retheipix" ) = retheipix;
    float64_t newdvaperpixw = i->get_dva_wid() / retwidpix ;

    
    
    float64_t newnyquistdva =  nyqf_from_dvaperpix( newdvaperpixw ); 
    
    
    
    
    float64_t newsigmafdva = newnyquistdva/param_as_float64(params, "downsamp_nyquist_mult"); 
    float64_t newsigmadva = sigma_from_sigmaf( newsigmafdva );
    float64_t xsigma_pix = dva_to_pix(  newsigmadva, i->get_dva_per_pix_wid() ); 
    
    
    
    
    HELPPRINTF(stdout, "RESAMPLE (%s): (nyqmult %lf  -  target nyq %lf) : New wid/hei %d %d (%lf %lf of input)   -- Will (maybe) blur with sigma [%lf] pix (resample ratio=%lf)\n", nickname.c_str(), param_as_float64(params, "downsamp_nyquist_mult"), targ_nyquist_dva, retwidpix, retheipix, (float64_t)retwidpix/pixsize.width, (float64_t)retheipix/pixsize.height, xsigma_pix, resample_ratio );
    if( xsigma_pix < 0.001 || xsigma_pix > 100 )
      {
	fprintf(stderr, "Whoa...warning: newsigmapix is [%lf]\n", xsigma_pix );
      }
    
    
    
    
    
    if(resample_ratio < 1) 
      {
	scratch.float64("xsigma_pix") = xsigma_pix; 
	
	
	
	
	double min_perstep_ratio = 0.5; 
	double max_perstep_ratio = 0.7;
	auto sizes = recursive_downsample( i->size(), targsize, min_perstep_ratio, max_perstep_ratio, i->get_origaspectratio_wh(), allowederr );

	scratch.sizev("sizes") = sizes;
	

	double fullsigmawidth=0; 
	if(sizes.size() > 1 )
	  {
	    std::vector<float64_t> pixperdvas;
	    auto blurwidths_dva = recursive_gauss_lpf_sigmas( sizes, i->get_pix_per_dva_wid(), param_as_float64(params, "downsamp_nyquist_mult"), pixperdvas );
	    if( sizes.size() != blurwidths_dva.size() + 1)
	      {
		fprintf(stderr, "Uh, blurwidths/sizes not correctly sized?\n");
		exit(1);
	      }

	    scratch.float64v("pixperdvas") = pixperdvas;
	    scratch.float64v("blurwidths_dva") = blurwidths_dva;
	     
	  }

	
#ifdef GAUSS_USE_KERNEL
	
	int ksize_pix = ksize_compute( xsigma_pix ); 
	cv::Mat kern = cv::getGaussianKernel( ksize_pix, xsigma_pix );
	kern.copyTo(scratch.cpu_w("kern"));
#endif
      }
    else
      {
	scratch.float64("xsigma_pix") = -1; 
      }
  }









if( scratch.float64("xsigma_pix") > 0 )
  {
    
    
    Timer t;

#ifdef DOWNSIZE_RECURSIVE
    recursive_downsize_2mat_optim( i->cpu(), scratch.cpu_w("blurred"), output->cpu_w(),
				   scratch.sizev("sizes"), scratch.float64v("pixperdvas"), scratch.float64v("blurwidths_dva"),
				   dinterp);
    if( output->size().height <= 0 || output->size().width <= 0 )
      {
	fprintf(stderr, "REV: resize, output is size 0...\n");
	exit(1);
      }
#else

    
#ifdef GAUSS_USE_KERNEL
    cv::sepFilter2D( i->cpu(), scratch.cpu_w("blurred"), -1, scratch.cpu("kern"), scratch.cpu("kern") );
#else
    cv::GaussianBlur( i->cpu(), scratch.cpu_w("blurred"), cv::Size(0,0), scratch.float64("xsigma_pix") );
#endif
    
    double e1 = t.elapsed();
    cv::resize( scratch.cpu("blurred"), output->cpu_w(), cv::Size( scratch.int64("retwidpix"), scratch.int64("retheipix") ), 0, 0, dinterp );
    double e2 = t.elapsed() - e1;
    DEBUGPRINTF(stdout, "REV: resample (down) minnyquist, BLUR (std=%lf pix) took [%lf] msec,  RESIZE (%d -> %d) took [%lf] msec\n", scratch.float64("xsigma_pix"), e1*1e3, scratch.cpu("blurred").size().width, output->cpu().size().width, e2*1e3 );

#endif 
  }
 else
   {
     Timer t;
     cv::resize( i->cpu(), output->cpu_w(), cv::Size( scratch.int64("retwidpix"), scratch.int64("retheipix") ), 0, 0, uinterp );
     double e1 = t.elapsed();
     DEBUGPRINTF(stdout, "REV: resample (up) minnyquist, RESIZE (%d -> %d) took [%lf] msec\n", i->cpu().size().width, output->cpu().size().width, e1*1e3 );
   }

output->setMetadata( i->get_time(), i->get_dva_wid(), i->get_dva_hei(), i->get_origaspectratio_wh() );


	  


