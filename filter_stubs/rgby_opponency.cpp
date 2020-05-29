
auto lumi = GETINPUT("lumin");
auto ri = GETINPUT("rin");
auto gi = GETINPUT("gin");
auto bi = GETINPUT("bin");
auto yi = GETINPUT("yin");

auto rgout = GETOUTPUT("rg");
auto byout = GETOUTPUT("by");

double mi,ma;

cv::minMaxLoc( lumi->cpu(), &mi, &ma );

if( false == scratch.initialized() )
  {
    float32_t maxlum = param_as_float64(params, "lum_thresh"); 
    scratch.float64("maxlum") = maxlum;
  }


float32_t maxlum = scratch.float64("maxlum");

cv::threshold( lumi->cpu(), scratch.cpu_w("mask"), maxlum, 1.0, cv::THRESH_BINARY );

ri->copyCpuTo( rgout->cpu_w() );
bi->copyCpuTo( byout->cpu_w() );


if( ma > maxlum )
  {
    cv::subtract( rgout->cpu(), gi->cpu(), rgout->cpu_w() );
    cv::multiply( rgout->cpu(), scratch.cpu("mask"), rgout->cpu_w() );

    cv::subtract( byout->cpu(), yi->cpu(), byout->cpu_w() );
    cv::multiply( byout->cpu(), scratch.cpu("mask"), byout->cpu_w() );
    
  }
 else
   {
     
     
     rgout->cpu_w().setTo( cv::Scalar(0) );
     byout->cpu_w().setTo( cv::Scalar(0) );
   }

rgout->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );
byout->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );



