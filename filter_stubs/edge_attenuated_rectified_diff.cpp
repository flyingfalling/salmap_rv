const int dtype=cv::INTER_LINEAR;
const int itype=cv::INTER_AREA;

auto i1 = GETINPUT("input1");
auto i2 = GETINPUT("input2");


auto output = GETOUTPUT("output");


if( false == scratch.initialized() )
  {
    cv::Size targsize = i1->size();
    
    i1->cpu().copyTo(scratch.cpu_w("atten"));
    if( i1->size().width < i2->size().width )
      {
	i2->cpu().copyTo(scratch.cpu_w("atten"));
	targsize = i2->size();
      }

    scratch.int64("wid") = targsize.width;
    scratch.int64("hei") = targsize.height;
    
    
    float64_t attenprop = param_as_float64( params, "attenuation_width_proportion" );
    attenuation_filter_cpu( attenprop, scratch.cpu("atten"), scratch.cpu_w("atten") );
    
  }

i1->resizeToCpuOnly( scratch.get_w("resized1"), cv::Size(scratch.int64("wid"), scratch.int64("hei")), itype, dtype );
i2->resizeToCpuOnly( output, cv::Size(scratch.int64("wid"), scratch.int64("hei")), itype, dtype );

cv::subtract( scratch.cpu("resized1"), output->cpu(), output->cpu_w() ); 




zero_rectify_cpu( output->cpu(), output->cpu_w() );

cv::multiply( output->cpu(), scratch.cpu("atten"), output->cpu_w() );

output->setMetadata( i1->get_time(), output->get_dva_wid(), output->get_dva_hei(), output->get_origaspectratio_wh() );


