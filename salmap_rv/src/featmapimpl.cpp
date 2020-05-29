#include <salmap_rv/include/featmapimpl.hpp>

#include <salmap_rv/include/timetype.hpp>
#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>

using namespace salmap_rv;

void salmap_rv::FeatMapImpl::reset()
{
  map_time_series.reset();
}


 

salmap_rv::FeatMapImpl::FeatMapImpl( const std::string& _ln, const std::string& _name, const size_t& size ) : map_time_series( rotating_time_series<FeatMapImplInst>(size) )
{
  
  name = _name;
}

std::string salmap_rv::FeatMapImpl::enum_times()
{
  return map_time_series.enum_times();
}

bool salmap_rv::FeatMapImpl::getGetter( const int64_t& truetime, std::shared_ptr<FeatMapImplInst>& ret )
{
  
  bool found = map_time_series.getGetter( truetime, ret );
  DEBUGPRINTF(stdout, "FeatMapImpl (GET) ((output) map name=[%s])   Targ time [%ld], requesting truetime [%ld] (time series contains %ld elements -- returning (exists?) [%s])\n", name.c_str(), truetime, truetime, map_time_series.size(), found ? "true" : "false" );
  if( found )
    {
      DEBUGPRINTF(stdout, "FeatMapImpl (GET) -- since found, size was 	%d %d  (ptr=[%p])\n" , ret->size().width, ret->size().height, ret.get() );
      DEBUGPRINTF(stdout, "FeatMapImpl (GET)	%d %d\n" , ret->size().width, ret->size().height );
    }
  return found;
}


bool salmap_rv::FeatMapImpl::getSetter( const int64_t& truetime, std::shared_ptr<FeatMapImplInst>& ret )
{
  size_t size_before = map_time_series.size();
  bool found = map_time_series.getSetter( truetime, ret );
  DEBUGPRINTF(stdout, "FeatMapImpl (SET) ((output) map name=[%s])   Targ time [%ld], requesting truetime [%ld] (time series contains %ld elements -- returning (will write over?) [%s] (note size before was %ld))  (hint -- size %d %d) (ptr=[%p])\n", name.c_str(), truetime, truetime, map_time_series.size(), found ? "true" : "false", size_before, ret->size().width, ret->size().height, ret.get() );
  return found;
}

bool salmap_rv::FeatMapImpl::getGetter( const TIMETYPE& t, const int64_t& curr_time, std::shared_ptr<FeatMapImplInst>& ret )
{
  int64_t truetime = t.time_from_current(curr_time);
  bool found = getGetter( truetime, ret ); 
  
  return found;
}

bool salmap_rv::FeatMapImpl::getSetter( const TIMETYPE& t, const int64_t& curr_time, std::shared_ptr<FeatMapImplInst>& ret )
{
  int64_t truetime = t.time_from_current(curr_time);
  bool found = getSetter( truetime, ret ); 
  
  return found;
}


bool salmap_rv::FeatMapImpl::setTo( const int64_t& truetime, const std::shared_ptr<FeatMapImplInst>& ret )
{
  if( nullptr == ret )
    {
      fprintf(stderr, "featmapimpl -- REV: user is trying to pass nullptr to setTo -- disallow\n");
      exit(1);
    }
  
  bool found = map_time_series.setTo( truetime, ret );
  DEBUGPRINTF(stdout, "FeatMapImpl (SET) Targ time [%ld], requesting truetime [%ld] (time series contains %ld elements -- returning (wrote over?) [%s])\n", truetime, truetime, map_time_series.size(), found ? "true" : "false" );
  return found;
}


bool salmap_rv::FeatMapImpl::setTo( const TIMETYPE& t, const int64_t& curr_time, const std::shared_ptr<FeatMapImplInst> & ret )
{
  int64_t truetime = t.time_from_current(curr_time);
  bool found = setTo( truetime, ret ); 
  DEBUGPRINTF(stdout, "FeatMapImpl (SET) Current time [%ld], requesting truetime [%ld] (time series contains %ld elements -- returning (wrote over?) [%s])\n", curr_time, truetime, map_time_series.size(), found ? "true" : "false" );
  return found;
}

 
