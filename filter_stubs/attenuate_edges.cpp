
std::shared_ptr<FeatMapImplInst> i = GETINPUT("input");

std::shared_ptr<FeatMapImplInst> output = GETOUTPUT("output");

if( false == scratch.initialized() )
  
  {
    attenuation_filter_cpu( param_as_float64(params, "atten_size_prop"), i->cpu(), scratch.cpu_w("atten") );
  }

cv::multiply( i->cpu(), scratch.cpu("atten"), output->cpu_w() );

output->setMetadata( i->get_time(), i->get_dva_wid(), i->get_dva_hei(), i->get_origaspectratio_wh() );


