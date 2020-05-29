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

auto output = GETOUTPUT("output");

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




double weight = param_as_float64( params, ("weight" + tostring<size_t>(1)) );
inputs[0]->resizeToCpuOnly( output, s, itype, dtype );
cv::multiply( output->cpu(), weight, output->cpu_w() );

double weightsum = weight;
for( size_t x=1; x<inputs.size(); ++x )
  {
    weight = param_as_float64( params, ("weight" + tostring(x+1)) );
    weightsum += weight;
    inputs[x]->resizeToCpuOnly( scratch.get_w("tmp"), s, itype, dtype );
    cv::addWeighted( output->cpu(), 1.0, scratch.cpu("tmp"), weight, 0, output->cpu_w() );
  }

cv::divide( output->cpu(), weightsum, output->cpu_w() );



output->setMetadata( output->get_time(), output->get_dva_wid(), output->get_dva_hei(), output->get_origaspectratio_wh() );




