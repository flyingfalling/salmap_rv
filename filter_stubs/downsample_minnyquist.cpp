auto i = GETINPUT("input");

int interp=cv::INTER_AREA;

std::shared_ptr<FeatMapImplInst> output = GETOUTPUT("output");


if( false == scratch.initialized() )
  {
    
    
    
    float64_t targ_nyquist_dva = param_as_float64( params, "targ_nyquist_dva" );


    
  
    

    
    
    cv::Size pixsize = i->size();

    if( pixsize.width <= 0 || pixsize.height <= 0 )
      {
	fprintf(stderr, "DOWNSAMPLE MINNYQUIST [%s], pixsizes are <= 0...\n", nickname.c_str());
	exit(1);
      }
    float64_t curr_nyqf = nyqf_from_dvaperpix(  i->get_dva_per_pix_wid() ); 

    
    
    
  
  
    float64_t downsample_ratio = targ_nyquist_dva / curr_nyqf;

    
    if( downsample_ratio >= 1 )
      {
	fprintf(stderr, "DOWNSAMPLE WARNING: [%s] downsample ratio targ nyq / curr nyq >= 1 ratio : %lf, curr (input) nyq: %lf, target: %lf (will create identical wid/hei)\n", nickname.c_str(), downsample_ratio, curr_nyqf, targ_nyquist_dva );
	
	scratch.int64( "retwidpix" ) = pixsize.width;
	scratch.int64( "retheipix" ) = pixsize.height;
	
	
      }
    else
      {
	float64_t allowederr = param_as_float64(params, "resample_grace"); 
	int retwidpix=-1;
	int retheipix=-1;

	
	float64_t reterr = downsize_image_w_ratio( pixsize.width, pixsize.height, downsample_ratio, allowederr, i->get_origaspectratio_wh(),
						   retwidpix, retheipix);
	  
	if( reterr > allowederr)
	  {
	    fprintf(stderr, "REV: something seriously wrong downsize...\n");
	    exit(1);
	  }

	 
	  
	HELPPRINTF(stdout, "DOWNSAMPLE (%s) (target nyq %lf) : New wid/hei %d %d (%lf %lf of input)\n", nickname.c_str(), targ_nyquist_dva, retwidpix, retheipix, (float64_t)retwidpix/pixsize.width, (float64_t)retheipix/pixsize.height );
	scratch.int64( "retwidpix" ) = retwidpix;
	scratch.int64( "retheipix" ) = retheipix;
	
      }
  }




Timer t;
cv::resize( i->cpu(), output->cpu_w(), cv::Size( scratch.int64("retwidpix"), scratch.int64("retheipix") ), 0, 0, interp );
double e1 = t.elapsed();
DEBUGPRINTF(stdout, "REV: DOWNSAMPLE minnyquist, RESIZE (%d -> %d) took [%lf] msec\n", i->cpu().size().width, output->cpu().size().width, e1*1e3 );
  

output->setMetadata( i->get_time(), i->get_dva_wid(), i->get_dva_hei(), i->get_origaspectratio_wh() );




