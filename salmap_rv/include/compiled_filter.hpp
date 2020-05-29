#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/featfilter.hpp>


namespace salmap_rv
{
  struct compiled_filter
  {
    FeatFilter f;
    
    compiled_filter();
    
    compiled_filter( const std::string& filt );
    
  };
}
