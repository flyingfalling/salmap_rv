const int dtype=cv::INTER_AREA;
const int itype=cv::INTER_LINEAR;

std::shared_ptr<FeatMapImplInst> i1 = GETINPUT( "input1" );
std::shared_ptr<FeatMapImplInst> i2 = GETINPUT( "input2" );

std::shared_ptr<FeatMapImplInst> output = GETOUTPUT("output");


if( false == scratch.initialized() )
  {
    cv::Size targsize = i1->size();
    
    if( i1->size().width < i2->size().width )
      {
	targsize = i2->size();
      }

    scratch.int64("wid") = targsize.width;
    scratch.int64("hei") = targsize.height;
    
    
    float64_t thresh=param_as_float64(params, "thresh");
    scratch.float64("thresh") = thresh;
  }

i1->resizeToCpuOnly( scratch.get_w("resized1"), cv::Size(scratch.int64("wid"), scratch.int64("hei")), itype, dtype );

i2->resizeToCpuOnly( output, cv::Size(scratch.int64("wid"), scratch.int64("hei")), itype, dtype );



cv::absdiff( scratch.cpu("resized1"), output->cpu(), output->cpu_w() );




float64_t irrelevant=-1;
float64_t thresh=scratch.float64("thresh");
if( thresh > 0 )
  {
    
    cv::threshold( output->cpu(), output->cpu_w(), thresh, irrelevant, cv::THRESH_TOZERO );
  }


output->setMetadata( i1->get_time(), i1->get_dva_wid(), i1->get_dva_hei(), i1->get_origaspectratio_wh() );


