auto i1 = GETINPUT("input");
auto output = GETOUTPUT("output");

if( false == scratch.initialized() )
  {
    scratch.int64( "iterations" ) = param_as_int64( params, "iterations"); 
    scratch.float64( "center_dva" ) = param_as_float64( params, "center_dva"); 
    scratch.float64( "surround_dva" ) = param_as_float64( params, "surround_dva");
    scratch.float64( "constinhib" ) = param_as_float64( params, "constinhib");
    scratch.float64( "center_pix" ) = scratch.float64("center_dva") / i1->get_dva_per_pix_wid(); 
    scratch.float64( "surround_pix" ) = scratch.float64("surround_dva") / i1->get_dva_per_pix_wid();

  
  
    scratch.float64( "cex" ) = param_as_float64( params, "cex" ); 
    scratch.float64( "cinh" ) = param_as_float64( params, "cinh" ); 
    
    
    
  
    
    scratch.float64( "center_const" ) = (scratch.float64("cex")*scratch.float64("cex")) / (2*CV_PI*scratch.float64("center_pix")*scratch.float64("center_pix")); 
    scratch.float64( "surround_const" ) = (scratch.float64("cinh")*scratch.float64("cinh")) / (2*CV_PI*scratch.float64("surround_pix")*scratch.float64("surround_pix")); 

    
  
    
    
    
    
    
    
    
    
    
    
    
    
    
    scratch.float64( "ex" ) = pow((1.0+scratch.float64("center_const")), scratch.int64("iterations") );
    scratch.float64( "maxval" ) = 1.0*scratch.float64("ex") - scratch.float64("constinhib") * (scratch.float64("ex") - 1.0) / scratch.float64("center_const");
    if( scratch.float64("maxval") <= 0 )
      {
	fprintf(stderr, "Wtf maxval <= 0 ? [%lf]n", scratch.float64("maxval") );
	exit(1);
      }

  }

#if DEBUG > 5
FAIL_EXIT_NANINF( i1->cpu(), "Input 1");
#endif

zero_rectify_cpu( i1->cpu(), output->cpu_w() );

cv::normalize( output->cpu(), output->cpu_w(), 0, 1, cv::NORM_MINMAX );

#if DEBUG > 5
FAIL_EXIT_NANINF( output->cpu(), "Norm");
#endif


cv::GaussianBlur( output->cpu(), scratch.cpu_w("cmat"), cv::Size(0,0), scratch.float64("center_pix") );


cv::multiply( scratch.cpu("cmat"), scratch.float64("center_const"), scratch.cpu_w("cmat") );


#if DEBUG > 5
FAIL_EXIT_NANINF( scratch.cpu("cmat"), "Blur 1");
#endif


cv::GaussianBlur( output->cpu(), scratch.cpu_w("smat"), cv::Size(0,0), scratch.float64("surround_pix") );


cv::multiply( scratch.cpu("smat"), scratch.float64("surround_const"), scratch.cpu_w("smat") );

#if DEBUG > 5
FAIL_EXIT_NANINF( scratch.cpu("smat"), "Blur 2");
#endif
  

cv::subtract( scratch.cpu("cmat"), scratch.cpu("smat"), scratch.cpu_w("cmat") );


cv::add( output->cpu(), scratch.cpu("cmat"), output->cpu_w() );


cv::subtract( output->cpu(), scratch.float64("constinhib"), output->cpu_w() );


zero_rectify_cpu( output->cpu(), output->cpu_w() );

#if DEBUG > 5
FAIL_EXIT_NANINF( output->cpu(), "Before first loop");
#endif


for( size_t i=1; i<scratch.int64("iterations"); ++i )
  {
    cv::GaussianBlur( output->cpu(), scratch.cpu_w("cmat"), cv::Size(0,0), scratch.float64("center_pix") );
    cv::multiply( scratch.cpu("cmat"), scratch.float64("center_const"), scratch.cpu_w("cmat") );
    
    
    cv::GaussianBlur( output->cpu(), scratch.cpu_w("smat"), cv::Size(0,0), scratch.float64("surround_pix") );
    cv::multiply( scratch.cpu("smat"), scratch.float64("surround_const"), scratch.cpu_w("smat") );
  
    
    cv::subtract( scratch.cpu("cmat"), scratch.cpu("smat"), scratch.cpu_w("cmat") );
    cv::add( output->cpu(), scratch.cpu("cmat"), output->cpu_w() );
    
    
    cv::subtract( output->cpu(), scratch.float64("constinhib"), output->cpu_w() );
  
    zero_rectify_cpu( output->cpu(), output->cpu_w() );

    
    
#if DEBUG > 5
    FAIL_EXIT_NANINF( output->cpu(), "End of loop " + std::to_string(i) );
#endif
  }



cv::divide( output->cpu(), scratch.float64("maxval"), output->cpu_w() );

#if DEBUG > 5
FAIL_EXIT_NANINF( output->cpu(), "Before div" );
#endif


#if DEBUG > 1
double curr_min, curr_max;
cv::minMaxLoc( output->cpu(), &curr_min, &curr_max, NULL, NULL ); 
if( curr_min < 0 || curr_max > 1 )
  {
    fprintf(stderr, "WARNING, DOG normalize, currmin<0 or currmax>1 [%lf] [%lf]. Won't exit.\n", curr_min, curr_max );
  }
#endif
#if DEBUG > 5
FAIL_EXIT_NANINF( output->cpu(), "After div" );
#endif



output->setMetadata( i1->get_time(), i1->get_dva_wid(), i1->get_dva_hei(), i1->get_origaspectratio_wh() );



