auto ri = GETINPUT("rin");
auto gi= GETINPUT("gin");
auto bi = GETINPUT("bin");

auto output = GETOUTPUT("output");










cv::multiply( ri->cpu(), ri->cpu(), scratch.cpu_w("r2"));
cv::multiply( gi->cpu(), gi->cpu(), scratch.cpu_w("g2"));
cv::multiply( bi->cpu(), bi->cpu(), scratch.cpu_w("b2"));



double rweight = 0.299;
double gweight = 0.587;
double bweight = 0.114;

cv::multiply( rweight, scratch.cpu("r2"), scratch.cpu_w("tmp") );
output->copyFromCpu( scratch.cpu("tmp") );
cv::multiply( gweight, scratch.cpu("g2"), scratch.cpu_w("tmp") );
cv::add( scratch.cpu_w("tmp"), output->cpu(),  output->cpu_w() );
cv::multiply( bweight, scratch.cpu("b2"), scratch.cpu_w("tmp") );
cv::add( scratch.cpu_w("tmp"), output->cpu(),  output->cpu_w() );


output->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );



