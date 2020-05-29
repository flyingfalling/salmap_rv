#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/compiled_map.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/timetype.hpp>


namespace salmap_rv
{

  struct FeatMapImplPtr
  {
  private:
    compiled_map cm; 
    std::string argname; 
    std::string name; 
    std::shared_ptr< FeatMapImpl > ptr;
    std::shared_ptr< FeatMapImplSet > mapset;
    
    TIMETYPE t; 
    bool init( );
    void create_if_empty( );
    
  public:

    const std::string arg_name_I_represent() const;
    
    
    FeatMapImplPtr();
    FeatMapImplPtr(  const std::string& argn, const std::string& n, const FeatFilterImplSet& filtimpls, const FilterSet& fs, const std::shared_ptr<FeatMapImplSet>& mapsetptr );
    
    void set_abs_curr_time( const int64_t& curr_time );
    std::string print( );
    
    void setFrom( const std::shared_ptr<FeatMapImplInst>& arg, const int64_t& curr_time
#ifdef GPU
		  , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		  );

    void copyFrom( const std::shared_ptr<FeatMapImplInst>& arg, const int64_t& curr_time
#ifdef GPU
		   , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		   );

    bool getTo( std::shared_ptr<FeatMapImplInst>& ret, const int64_t& curr_time );
    bool setTo( std::shared_ptr<FeatMapImplInst>& ret, const int64_t& curr_time );
  };
}
