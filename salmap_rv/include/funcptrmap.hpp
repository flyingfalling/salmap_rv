#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/algorithmimpl.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/featfilterimpl.hpp>







namespace salmap_rv
{
  namespace detail
  {
    struct FuncPtrMap
    {
      friend struct FuncPtrRegistrar;
      
      using FuncPtr = algoFunct_t; 
      
      static std::map<std::string, FuncPtr> const& getMap()
      {
	return getWritableMap();
      }
    
      static void const enumerate()
      {
	DEBUGPRINTF(stdout, "Enumerating registered filter function pointers:\n");
	auto mymap = getMap();
	for( auto& x : mymap )
	  {
	    DEBUGPRINTF( stdout, "%s\n", x.first.c_str() );
	  }
      }
    
    private:
      
      static std::map<std::string, FuncPtr>& getWritableMap()
      {
	
	
	static std::map<std::string, FuncPtr> theMap;
	
	return theMap;
      }
    };
  }
}
