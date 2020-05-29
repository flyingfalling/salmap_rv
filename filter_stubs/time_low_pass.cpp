

auto cui = GETINPUT("curr");
auto pui = GETINPUT("prev");

auto output = GETOUTPUT("output");

if( true ==  pui->isDummy() )
  {
    cui->copyCpuTo( output );
    
    output->setMetadata( cui->get_time(), cui->get_dva_wid(), cui->get_dva_hei(), cui->get_origaspectratio_wh() );
    
    return;
  }

if( false == scratch.initialized() )
  {
    int64_t timedelta_nsec = cui->get_time() - pui->get_time();
    scratch.float64("time_const_nsec") = param_as_float64(params,"time_const_nsec");
    scratch.float64("fact") = exp( -timedelta_nsec / scratch.float64("time_const_nsec") );
    
    
  }

#ifdef USE_REAL_TIMEDELTA
const int64_t timedelta_nsec = cui->get_time() - pui->get_time();
const float64_t fact = exp( -timedelta_nsec / scratch.float64("time_const_nsec") );
#else
const float64_t fact = scratch.float64("fact");
#endif


cv::addWeighted( pui->cpu(), fact, cui->cpu(), (1-fact), 0, output->cpu_w() );


output->setMetadata( cui->get_time(), cui->get_dva_wid(), cui->get_dva_hei(), cui->get_origaspectratio_wh() );
  

