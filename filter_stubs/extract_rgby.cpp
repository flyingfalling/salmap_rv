  
auto ri = GETINPUT("r");
auto gi = GETINPUT("g");
auto bi = GETINPUT("b");

auto rout = GETOUTPUT("r");
auto gout = GETOUTPUT("g");
auto bout = GETOUTPUT("b");
auto yout = GETOUTPUT("y");

double SMALLNUM=0.01;


cv::add( ri->cpu(), gi->cpu(), scratch.cpu_w("lum3") );
cv::add( scratch.cpu("lum3"), bi->cpu(), scratch.cpu_w("lum3") );
cv::add( scratch.cpu("lum3"), SMALLNUM, scratch.cpu_w("lum3") );


ri->copyCpuTo( rout->cpu_w() );
bi->copyCpuTo( bout->cpu_w() );
gi->copyCpuTo( gout->cpu_w() );

cv::addWeighted( gi->cpu(), 0.5, bi->cpu(), 0.5, 0, scratch.cpu_w("g+b/2") );
cv::addWeighted( ri->cpu(), 0.5, bi->cpu(), 0.5, 0, scratch.cpu_w("r+b/2") );
cv::addWeighted( ri->cpu(), 0.5, gi->cpu(), 0.5, 0, scratch.cpu_w("r+g/2") );

cv::subtract( ri->cpu(), scratch.cpu("g+b/2"), rout->cpu_w() );
cv::subtract( gi->cpu(), scratch.cpu("r+b/2"), gout->cpu_w() );
cv::subtract( bi->cpu(), scratch.cpu("r+g/2"), bout->cpu_w() );

cv::absdiff( ri->cpu(), gi->cpu(), scratch.cpu_w( "abs(r-g)" ) );

cv::addWeighted( bout->cpu(), -1.0, scratch.cpu("abs(r-g)"), -1.0, 0, yout->cpu_w() );

cv::divide( rout->cpu(), scratch.cpu("lum3"), rout->cpu_w() );
cv::divide( gout->cpu(), scratch.cpu("lum3"), gout->cpu_w() );
cv::divide( bout->cpu(), scratch.cpu("lum3"), bout->cpu_w() );
cv::divide( yout->cpu(), scratch.cpu("lum3"), yout->cpu_w() );


cv::max( rout->cpu(), 0, rout->cpu_w());
cv::max( gout->cpu(), 0, gout->cpu_w());
cv::max( bout->cpu(), 0, bout->cpu_w());
cv::max( yout->cpu(), 0, yout->cpu_w());

  

rout->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );
gout->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );
bout->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );
yout->setMetadata( ri->get_time(), ri->get_dva_wid(), ri->get_dva_hei(), ri->get_origaspectratio_wh() );


    




  
