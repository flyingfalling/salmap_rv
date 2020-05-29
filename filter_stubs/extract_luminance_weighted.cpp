
auto ri = GETINPUT("rin");
auto gi= GETINPUT("gin");
auto bi = GETINPUT("bin");

auto output = GETOUTPUT("output");

if( false == scratch.initialized() )
  {
    scratch.float64("rw") = param_as_float64(params, "r_weight");
    scratch.float64("gw") = param_as_float64(params, "g_weight");
    scratch.float64("bw") = param_as_float64(params, "b_weight");
  }


cv::multiply( scratch.float64("rw"), ri->cpu(), scratch.cpu_w("tmp") );
output->copyFromCpu( scratch.cpu("tmp") );
cv::multiply( scratch.float64("gw"), gi->cpu(), scratch.cpu_w("tmp") );
cv::add( scratch.cpu_w("tmp"), output->cpu(),  output->cpu_w() );
cv::multiply( scratch.float64("bw"), bi->cpu(), scratch.cpu_w("tmp") );
cv::add( scratch.cpu_w("tmp"), output->cpu(),  output->cpu_w() );


output->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );


