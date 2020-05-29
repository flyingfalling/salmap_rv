
auto i1 = GETINPUT("input1");
auto i2 = GETINPUT("input2");

auto output = GETOUTPUT("output");

if( i1->size() != i2->size() )
  {
    fprintf(stderr, "REV: filter sum, map sizes not same\n");
    exit(1);
  }

cv::add( i1->cpu(), i2->cpu(), output->cpu_w() );

output->setMetadata( i1->get_time(), i1->get_dva_wid(), i1->get_dva_hei(), i1->get_origaspectratio_wh() );




