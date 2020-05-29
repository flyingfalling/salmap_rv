

auto i = GETINPUT("input");

auto output = GETOUTPUT("output");

if( false == scratch.initialized() )
  {
    float64_t xsigma_dva = param_as_float64( params, "sigma_dva" );
    float64_t xsigma_pix = xsigma_dva / i->get_dva_per_pix_wid(); 
    HELPPRINTF(stdout, "LPF (%s): Gaussian LPF will be applied of pixel width sigma [%lf] (input size %d %d) (dva/pix=%lf)\n", nickname.c_str(), xsigma_pix, i->size().width, i->size().height, i->get_dva_per_pix_wid() );
    scratch.float64( "xsigma_pix" ) = xsigma_pix;
    if( xsigma_pix <= 0.001 )
      {
	fprintf(stderr, "ERROR, (LPF_GAUSS_SIGMA) [%s] xsigma_pix is way too small? [%lf] (input size: %d %d). Exiting\n", nickname.c_str(), xsigma_pix, i->size().width, i->size().height);
	exit(1);
      }

#ifdef GAUSS_USE_KERNEL
    
    int ksize_pix = ksize_compute( xsigma_pix );
    
    cv::Mat kern = cv::getGaussianKernel( ksize_pix, xsigma_pix );
    kern.copyTo(scratch.cpu_w("kern"));
#endif
  }


#ifdef GAUSS_USE_KERNEL
cv::sepFilter2D( i->cpu(), output->cpu_w(), -1, scratch.cpu("kern"), scratch.cpu("kern") );
#else
cv::GaussianBlur( i->cpu(), output->cpu_w(), cv::Size(0,0), scratch.float64("xsigma_pix") );
#endif


output->setMetadata( i->get_time(), i->get_dva_wid(), i->get_dva_hei(), i->get_origaspectratio_wh() );




