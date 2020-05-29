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

  struct FeatMapCollectionPtr
  {
    std::shared_ptr< FeatMapImplCollection > ptr; 
    compiled_map cm; 
    std::string name;
    
    std::shared_ptr<FeatMapImplSet> mapset;
        
    FeatMapCollectionPtr();
    FeatMapCollectionPtr( const std::string& n, const FeatFilterImplSet& filtimpls, const FilterSet& fs, const std::shared_ptr<FeatMapImplSet>& mapsetptr );
    
    bool init( );
    std::string print( );
    
    std::shared_ptr< FeatMapImplCollection > getCollection( );
  };
}
