const int dtype=cv::INTER_AREA;
const int itype=cv::INTER_LINEAR;

if( inputmaps.size() < 1 )
  {
    fprintf(stderr, "REV: error in map avg\n");
    exit(1);
  }


std::vector<std::shared_ptr<FeatMapImplInst>> inputs;
std::vector<float64_t> weights;
size_t maxsize = 0;
cv::Size s;





auto flatmap = flat_raw_params_from_group( params, param_group("weight") );





float64_t weightsum=0;
for( auto it = flatmap.begin(); it != flatmap.end(); ++it )
  {
    std::string weightfor = it->first;
    std::string truename = it->second;
    float64_t value = param_as_float64( params, truename );
    
    weights.push_back(value);
    weightsum += value;
    
    inputs.push_back( GETINPUT( weightfor ) );
    
    if( inputs.back()->size().width > maxsize )
      {
	s = inputs.back()->size();
	maxsize = inputs.back()->size().width;
      }
  }


std::vector<std::string> names;

auto output = GETOUTPUT("output");

 






inputs[0]->resizeToCpuOnly( output, s, itype, dtype );
cv::multiply( output->cpu(), weights[0], output->cpu_w() );

for( size_t x=1; x<inputs.size(); ++x )
  {
    
    
    inputs[x]->resizeToCpuOnly( scratch.get_w("tmp"), s, itype, dtype );
    cv::addWeighted( output->cpu(), 1.0, scratch.cpu("tmp"), weights[x], 0, output->cpu_w() );
  }

cv::divide( output->cpu(), weightsum, output->cpu_w() );



output->setMetadata( output->get_time(), output->get_dva_wid(), output->get_dva_hei(), output->get_origaspectratio_wh() );




