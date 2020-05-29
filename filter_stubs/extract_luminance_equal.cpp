
auto ri = GETINPUT("rin");
auto gi = GETINPUT("gin");
auto bi = GETINPUT("bin");

auto output = GETOUTPUT("output");


const double rweight = 1.0 / 3;
const double gweight = 1.0 / 3;
const double bweight = 1.0 / 3;


cv::add( ri->cpu(), gi->cpu(), output->cpu_w() );
cv::add( bi->cpu(), output->cpu(), output->cpu_w() );
cv::divide( output->cpu(), 3, output->cpu_w() );



 

output->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );


