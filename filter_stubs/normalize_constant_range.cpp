

auto i1 = GETINPUT("input");
auto output = GETOUTPUT("output");

if( false == scratch.initialized() )
  {
    scratch.float64( "norm_min" ) = param_as_float64( params, "min");
    scratch.float64( "norm_max" ) = param_as_float64( params, "max");
    
    if( scratch.float64("norm_min") == 0 && scratch.float64("norm_max") == 0 )
      {
	fprintf(stderr, "REV: error in normalize_constant_range_cpu, both min and max are zero...\n");
	exit(1);
      }
  }

zero_rectify_cpu( i1->cpu(), output->cpu_w() );
cv::normalize( output->cpu(), output->cpu_w(), scratch.float64("norm_min"), scratch.float64("norm_max"), cv::NORM_MINMAX );


output->setMetadata( i1->get_time(), i1->get_dva_wid(), i1->get_dva_hei(), i1->get_origaspectratio_wh() );


