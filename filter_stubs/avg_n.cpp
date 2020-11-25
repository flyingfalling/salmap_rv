const int dtype=cv::INTER_AREA;
const int itype=cv::INTER_LINEAR;

if( inputmaps.size() < 1 )
  {
    fprintf(stderr, "REV: error in map avg\n");
    exit(1);
  }

std::vector<std::shared_ptr<FeatMapImplInst>> inputs;
size_t maxsize = 0;
cv::Size s;
std::vector<std::string> names;

std::shared_ptr<FeatMapImplInst> output = GETOUTPUT("output");


  


for( auto iter=inputmaps.begin(); iter != inputmaps.end(); ++iter )
  {
    
    if( iter->second->size().width > maxsize )
      {
	s = iter->second->size();
	
	maxsize = iter->second->size().width;
      }
    inputs.push_back( iter->second );
    names.push_back( iter->first );
  }







inputs[0]->resizeToCpuOnly( output, s, itype, dtype );








for( size_t x=1; x<inputs.size(); ++x )
  {
    
    inputs[x]->resizeToCpuOnly( scratch.get_w("tmp"), s, itype, dtype );

    
    
    
    
    cv::add( output->cpu(), scratch.cpu("tmp"), output->cpu_w() );
    
    
  }

cv::divide( output->cpu(), (float64_t)inputs.size(), output->cpu_w() );


output->setMetadata( inputs[0]->get_time(), output->get_dva_wid(), output->get_dva_hei(), output->get_origaspectratio_wh() );



