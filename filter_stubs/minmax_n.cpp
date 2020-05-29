
if( inputmaps.size() < 1 )
  {
    fprintf(stderr, "REV: error in minmax n\n");
    exit(1);
  }


auto outmin = GETOUTPUT("min");
auto outmax = GETOUTPUT("max");



double min=1e12, max=-1e12;

for( auto iter=inputmaps.begin(); iter != inputmaps.end(); ++iter )
  {
    double tmin, tmax;
    cv::minMaxLoc( iter->second->cpu(), &tmin, &tmax, NULL, NULL );
    if( tmin < min )
      {
	min = tmin;
      }
    if( tmax > max )
      {
	max = tmax;
      }
  }

if( max > 1+1e-6 || min < 0-1e-6 )
  {
    fprintf(stderr, "MIN/MAX_N : REV: min/max was outside of range [0,1] (min=%lf max=%lf)\n", min, max );
    exit(1);
  }
if( min < 0 )
  {
    min = 0;
  }
if( max > 1 )
  {
    max = 1;
  }

outmin->cpu_w() = salmap_rv::FiltMat( cv::Size(1,1), CV_64FC1, min );
outmax->cpu_w() = salmap_rv::FiltMat( cv::Size(1,1), CV_64FC1, max );

outmin->setMetadata( inputmaps[0]->get_time(), -1, -1, -1 );
outmax->setMetadata( inputmaps[0]->get_time(), -1, -1, -1 );
