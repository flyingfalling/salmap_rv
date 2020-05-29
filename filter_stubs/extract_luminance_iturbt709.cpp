
auto ri = GETINPUT("rin");
auto gi= GETINPUT("gin");
auto bi = GETINPUT("bin");

auto output = GETOUTPUT("output");










double rweight = 0.2126;
double gweight = 0.7152;
double bweight = 0.0722;

cv::multiply( rweight, ri->cpu(), scratch.cpu_w("tmp") );
output->copyFromCpu( scratch.cpu("tmp") );
cv::multiply( gweight, gi->cpu(), scratch.cpu_w("tmp") );
cv::add( scratch.cpu_w("tmp"), output->cpu(),  output->cpu_w() );
cv::multiply( bweight, bi->cpu(), scratch.cpu_w("tmp") );
cv::add( scratch.cpu_w("tmp"), output->cpu(),  output->cpu_w() );


output->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );


