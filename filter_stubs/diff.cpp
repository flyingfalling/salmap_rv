
auto i1 = GETINPUT("input1");
auto i2 = GETINPUT("input2");

auto s1 = i1->size();
auto s2 = i2->size();

std::shared_ptr<FeatMapImplInst> output = GETOUTPUT("output");

if( s1 != s2 )
  {
    fprintf(stderr, "DIFF input heights not equal...(%ld %ld) vs (%ld %ld)\n", s1.width, s1.height, s2.width, s2.height); 
    exit(1);
  }

cv::subtract( i1->cpu(), i2->cpu(), output->cpu_w() );

output->setMetadata( i1->get_time(), i1->get_dva_wid(), i1->get_dva_hei(), i1->get_origaspectratio_wh() );



