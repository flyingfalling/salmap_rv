#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/featmap.hpp>


namespace salmap_rv
{
  struct compiled_map
  {
    
    FeatMap m;
    FeatMap m2;
  
    void set_localname( const std::string& ln );
  
    std::string name_no_time_no_localname() const;

    std::string get_localname() const;
  
    compiled_map( );

    
    
    
    
    compiled_map( const std::string& name, const FeatFilterImplSet& filtimpls, const FilterSet& fs  );

    
    compiled_map( const std::string& name );
  
  };
}
