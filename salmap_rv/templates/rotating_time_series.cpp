
using namespace salmap_rv;

#include <salmap_rv/include/util_functs.hpp>







template <typename T>
rotating_time_series<T>::rotating_time_series( const size_t& maxelements )
    : max_size(maxelements)
  {
    if( max_size < 1 )
      {
	fprintf(stderr, "REV: setting max size of time series to 0 elements, i.e. no elements...exiting.\n");
	exit(1);
      }
  }


template <typename T>
void rotating_time_series<T>::reset()
{
  timeseries_nsec.clear();
}

template <typename T>
size_t rotating_time_series<T>::size() const
{
  return timeseries_nsec.size();
}




template <typename T>
bool rotating_time_series<T>::getGetter( const int64_t& target_time_nsec, std::shared_ptr<T>& ret )
{
  auto a = timeseries_nsec.find(target_time_nsec);
  if ( a ==  timeseries_nsec.end() )
    {
      ret = nullptr; 
      return false;
    }
  else
    {
      ret = (a->second); 
      if( nullptr == ret )
	{
	  fprintf(stderr, "REV: something is wrong, item existed but is nullptr (either I'm not instantiting automatically correctly, *OR* user could have used setTo() to set it to a nullptr (good argument for forcing user to always copy INTO this)...\n");
	  exit(1);
	}
      return true;
    }
}




template <typename T>
bool rotating_time_series<T>::set_ptr_to_target( const int64_t& target_time_nsec, const std::shared_ptr<T>& ret )
{
  if( max_size < 1 )
    {
      fprintf(stderr, "Sanity check -- max size < 1\n");
      exit(1);
    }
  
  bool wroteover = false;
  auto it = timeseries_nsec.find( target_time_nsec );
  if( timeseries_nsec.end() != it )
    {
      
      it->second = ret;
      wroteover = true;
    }
  else
    {
      
      if( timeseries_nsec.size() < max_size )
	{
	  
	  timeseries_nsec[ target_time_nsec ] = ret;
	  wroteover = false;
	}
      else if( timeseries_nsec.size() == max_size )
	{
	  
	  
	  
	  
#define EXTRACT_METHOD
#ifdef EXTRACT_METHOD
	  auto extractednode = timeseries_nsec.extract(timeseries_nsec.begin());
	  extractednode.key() = target_time_nsec;
	  auto newiter = timeseries_nsec.insert(timeseries_nsec.end(), std::move(extractednode));
	  
	  newiter->second = ret;
#else
	  timeseries_nsec.erase( timeseries_nsec.begin() );
	  timeseries[ target_time_nsec ] = ret;
#endif
	  wroteover = false;
	}
      else
	{
	  fprintf(stderr, "ERROR: REV: size of rotating time series map should never be more than maxsize [%ld]: now [%ld]\n", max_size, timeseries_nsec.size());
	  exit(1);
	}
    }
  return wroteover;
}



template <typename T>
bool rotating_time_series<T>::get_ptr_to_target( const int64_t& target_time_nsec, std::shared_ptr<T>& ret )
{
  if( max_size < 1 )
    {
      fprintf(stderr, "Sanity check -- max size < 1\n");
      exit(1);
    }

  bool timeexisted=false;
  auto it = timeseries_nsec.find( target_time_nsec );
  if( timeseries_nsec.end() != it )
    {
      
      ret = it->second;
      DEBUGPRINTF(stdout, "Rot Time Series: get_ptr_to_target, Time [%ld] EXISTED -- (ptr is [%p])\n", target_time_nsec, ret.get() );
      timeexisted = true;
    }
  else
    {
      
      if( timeseries_nsec.size() < max_size )
	{
	  
	  
	  ret = std::make_shared<T>(); 
	  DEBUGPRINTF(stdout, "Rot Time Series: get_ptr_to_target, Time [%ld] (Didn't exist). NOT FULL so creating new! (ptr is [%p])\n", target_time_nsec, ret.get() );
	  timeseries_nsec[ target_time_nsec ] = ret;
	  timeexisted = false;
	}
      else if( timeseries_nsec.size() == max_size )
	{
	  
	  ret = timeseries_nsec.begin()->second; 
	  
	  
	  
#define EXTRACT_METHOD
#ifdef EXTRACT_METHOD
	  auto extractednode = timeseries_nsec.extract(timeseries_nsec.begin());
	  extractednode.key() = target_time_nsec;
	  timeseries_nsec.insert(timeseries_nsec.end(), std::move(extractednode));
#else
	  timeseries_nsec.erase( timeseries_nsec.begin() );
	  timeseries[ target_time_nsec ] = ret; 
#endif
	  
	  DEBUGPRINTF(stdout, "Rot Time Series: get_ptr_to_target, Time [%ld] (Didn't exist). FULL so creating new and reaping! (ptr is [%p])\n", target_time_nsec, ret.get() );
	  timeexisted = false;
	}
      else
	{
	  fprintf(stderr, "ERROR: REV: size of rotating time series map should never be more than maxsize [%ld]: now [%ld]\n", max_size, timeseries_nsec.size());
	  exit(1);
	}
    }
  return timeexisted;
}



template <typename T>
bool rotating_time_series<T>::getSetter( const int64_t& target_time_nsec, std::shared_ptr<T>& ret )
{
  return get_ptr_to_target( target_time_nsec, ret );
}



template <typename T>
bool rotating_time_series<T>::setTo( const int64_t& target_time_nsec, const std::shared_ptr<T>& ret )
{
  if( nullptr == ret )
    {
      fprintf(stderr, "ROTATING_TIME_SERIES -- REV: don't allow user to setTo to a nullptr...\n");
      exit(1);
    }
  return set_ptr_to_target( target_time_nsec, ret);
}

 


template <typename T>
std::string rotating_time_series<T>::enum_times()
{
  std::string times="";
  for( auto iter = timeseries_nsec.begin(); iter != timeseries_nsec.end(); ++iter)
    {
      times += "[" + tostring(iter->first) + "]";
    }
  return times;
}
  
