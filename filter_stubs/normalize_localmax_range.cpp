auto i1 = GETINPUT("input");


auto output = GETOUTPUT("output");

if( false == scratch.initialized() )
  {
    const float64_t norm_min = param_as_float64( params, "min");
    const float64_t norm_max = param_as_float64( params, "max");
    scratch.float64("min") = norm_min;
    scratch.float64("max") = norm_max;
    size_t s = 3;
    cv::Mat tmp = cv::getStructuringElement( cv::MORPH_CROSS, cv::Size(s,s) ); 
    tmp.copyTo(scratch.cpu_w("structelem"));
    if( norm_min == 0 && norm_max == 0 )
      {
	fprintf(stderr, "REV: error in normalize_localmax_range_cpu, both min and max are zero...\n");
	exit(1);
      }
  }




zero_rectify_cpu( i1->cpu(), output->cpu_w() );

float32_t thresh=0;
float64_t curr_min;
float64_t curr_max;

cv::normalize( output->cpu(), output->cpu_w(), scratch.float64("min"), scratch.float64("max"), cv::NORM_MINMAX );


cv::minMaxLoc( output->cpu(), &curr_min, &curr_max, NULL, NULL ); 



float64_t currscale = curr_max - curr_min;

if( currscale > 0 )
  {
    
    
    if( currscale > 1.0 + 1e-6 || currscale < 1.0 - 1e-6)
      {
	fprintf(stderr, "REV: WARNING: curr scale is not 1! It is %lf (note size of mat is %ld %ld)\n", currscale, scratch.cpu("output").size().width, scratch.cpu("output").size().height );
	
      }
    
    {
      thresh = curr_min + (0.1 * currscale );
      
      
      float32_t irrelevant=-1;
      output->copyCpuTo( scratch.cpu_w("image2") );
      
      
      
      
      
#if DEBUG>5
      double min,max;
      cv::minMaxLoc( scratch.cpu("image2"), &min, &max, NULL, NULL ); 
	 
      if( min < 0 )
	{
	  fprintf(stderr, "REV: threshold is returning values less than zero, min is [%lf]\n", min );
	  exit(1);
	}
#endif
	 
      
      int nonzero = cv::countNonZero( scratch.cpu("image2") );
      float32_t multiple=0;
      if( nonzero == 0 )
	{
	  fprintf(stderr, "REV: whole image is black in normalize, just returning...\n");
	}
      else
	{
	  cv::dilate(scratch.cpu("image2"), scratch.cpu_w("mask"), scratch.cpu("structelem"));
	  cv::compare(scratch.cpu("image2"), scratch.cpu("mask"), scratch.cpu_w("mask2"), cv::CMP_GE);
	  
	  float32_t scale = 1.f/255;
	  int dtype = CV_32F;
	  cv::multiply( scratch.cpu("image2"), scratch.cpu("mask2"), scratch.cpu_w("image2"), scale, dtype ); 
	  
	  
	  
	  int nummaxes;
	  double avg_localmax = avg_nonzero_cpu( scratch.cpu("image2"), nummaxes );
#if DEBUG > 5
	  if( avg_localmax < 0-1e-6 || avg_localmax > 1+1e-6 )
	    {
	      fprintf(stderr, "Whoa avg max < 0 or >1 [%lf]\n", avg_localmax);
	      exit(1);
	    }
#endif
	  if( avg_localmax < 0 )
	    {
	      avg_localmax = 0;
	    }
	   
	  
	  if( nummaxes == 1 )
	    {
	      multiple = 1;
	    }
	  else
	    {
	      multiple = (curr_max - avg_localmax); 
	    }
	  if( multiple < 0 )
	    {
	      multiple = 0;
	    }
	   
	   
	  
	  multiple *= multiple;
	  if( curr_max > 0 )
	    {
	      multiple /= curr_max;
	    }
	} 
      cv::multiply( output->cpu(), multiple, output->cpu_w() );
    }

    
    
    
    
    
#if DEBUG>5
    cv::minMaxLoc( output->cpu(), &curr_min, &curr_max, NULL, NULL ); 
    if( curr_min < 0-1e-6 || curr_max > 1+1e-6 )
      {
	fprintf(stderr, "After normalization, out of range. %lf %lf\n", curr_min, curr_max );
	exit(1);
      }
#endif
  }
 else
   {
     output->cpu_w().setTo(cv::Scalar(0.0));
   }



output->setMetadata( i1->get_time(), i1->get_dva_wid(), i1->get_dva_hei(), i1->get_origaspectratio_wh() );


