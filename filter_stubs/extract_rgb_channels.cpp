


auto fi = GETINPUT("bgrin");



if( fi->cpu().type() != CV_8UC3 )
  {
    fprintf(stderr, "REV: extract_rgb_channels not in expected 3-channel 8-bit format...\n");
    exit(1);
  }


auto rout = GETOUTPUT("r");
auto gout = GETOUTPUT("g");
auto bout = GETOUTPUT("b");



std::vector< salmap_rv::FiltMat > grayPlanes(3);
grayPlanes[0] = scratch.cpu_w("rb");
grayPlanes[1] = scratch.cpu_w("rg");
grayPlanes[2] = scratch.cpu_w("rr");

cv::split( fi->cpu(), grayPlanes );

grayPlanes[0].convertTo( bout->cpu_w(), CV_32FC1, 1.f/255);
grayPlanes[1].convertTo( gout->cpu_w(), CV_32FC1, 1.f/255);
grayPlanes[2].convertTo( rout->cpu_w(), CV_32FC1, 1.f/255);

rout->setMetadata( fi->get_time(), fi->get_dva_wid(), fi->get_dva_hei(), fi->get_origaspectratio_wh() );
gout->setMetadata( fi->get_time(), fi->get_dva_wid(), fi->get_dva_hei(), fi->get_origaspectratio_wh() );
bout->setMetadata( fi->get_time(), fi->get_dva_wid(), fi->get_dva_hei(), fi->get_origaspectratio_wh() );





  
