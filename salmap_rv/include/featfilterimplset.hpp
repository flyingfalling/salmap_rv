#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{
  struct FeatFilterImplSet
  {
    std::map<std::string, FeatFilterImpl> filters; 
    void clear();
    std::map<std::string,FeatFilterImpl>::iterator addFilter( FeatFilter& f, algorithmImplSet& algs );
    bool getFilter( FeatFilter& f, FeatFilterImpl& fret );
    bool getFilter( FeatFilter& f, FeatFilterImpl*& fret );
    bool getFilter( const std::string& n, FeatFilterImpl& fret );
    bool getFilter( const std::string& n, FeatFilterImpl*& fret );
  };
}
