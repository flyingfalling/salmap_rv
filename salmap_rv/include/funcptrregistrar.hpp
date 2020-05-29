#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


#include <salmap_rv/include/funcptrmap.hpp>

namespace salmap_rv
{
  namespace detail
  {
    
    struct FuncPtrRegistrar
    {
      FuncPtrRegistrar( FuncPtrMap::FuncPtr funcPtr, const std::string funcName);
    };
  }
}



 
