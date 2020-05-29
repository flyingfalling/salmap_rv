#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>




namespace salmap_rv
{

  template <typename T>
  struct rotating_time_series
  {
    
  private:
    std::map<int64_t, std::shared_ptr<T>> timeseries_nsec;
    size_t max_size;

    bool get_ptr_to_target( const int64_t& target_time_nsec, std::shared_ptr<T>& ret );
    
    bool set_ptr_to_target( const int64_t& target_time_nsec, const std::shared_ptr<T>& ret );
    
  public:
    rotating_time_series( const size_t& maxelements );
    void reset();
    
    size_t size() const;
    
    
    bool getGetter( const int64_t& target_time_nsec, std::shared_ptr<T>& ret );
    
    
    
    
    
    bool getSetter( const int64_t& target_time_nsec, std::shared_ptr<T>& ret );
    
    bool setTo( const int64_t& target_time_nsec, const std::shared_ptr<T>& ret );

    std::string enum_times();
  
  };

}


#include <salmap_rv/templates/rotating_time_series.cpp>
