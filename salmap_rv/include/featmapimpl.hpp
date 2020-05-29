#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/rotating_time_series.hpp>



namespace salmap_rv
{
  struct FeatMapImpl
  {
  private:
    rotating_time_series<FeatMapImplInst> map_time_series; 
    
  public:
    
    std::string name;

    
    
    
    
    bool getSetter( const int64_t& truetime, std::shared_ptr<FeatMapImplInst>& ret );
    bool getSetter( const TIMETYPE& t, const int64_t& curr_time, std::shared_ptr<FeatMapImplInst>& ret );

    
    bool getGetter( const int64_t& truetime, std::shared_ptr<FeatMapImplInst>& ret );
    bool getGetter( const TIMETYPE& t, const int64_t& curr_time, std::shared_ptr<FeatMapImplInst>& ret );
    
    
    
    
    bool setTo( const int64_t& truetime, const std::shared_ptr<FeatMapImplInst> & ret );
    bool setTo( const TIMETYPE& t, const int64_t& curr_time, const std::shared_ptr<FeatMapImplInst> & ret );
    
    void reset();
    
    FeatMapImpl( const std::string& _ln, const std::string& _name, const size_t& size=3 );
    std::string enum_times();
  };
}
