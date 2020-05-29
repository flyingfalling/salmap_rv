#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/algorithmimpl.hpp>

namespace salmap_rv
{
struct algorithmImplSet
{
  std::map<std::string, algorithmImpl> algorithms;
  
 
  void clear();
  
  void register_algorithm( const std::string& name, const algoFunct_t updatef, const algoFunct_t initf = defaultInit, const algoFunct_t destroyf = defaultDestroy );

  bool get( const std::string& name, algorithmImpl& reta );
  
}; 
} 
