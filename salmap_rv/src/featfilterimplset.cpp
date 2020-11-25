#include <salmap_rv/include/featfilterimplset.hpp>

#include <salmap_rv/include/featfilterimpl.hpp>
#include <salmap_rv/include/featfilter.hpp>
#include <salmap_rv/include/algorithmimplset.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
using namespace salmap_rv;

void salmap_rv::FeatFilterImplSet::clear()
{
  filters.clear();
}









std::map<std::string,FeatFilterImpl>::iterator salmap_rv::FeatFilterImplSet::addFilter( FeatFilter& f, algorithmImplSet& algs )
{
  FeatFilterImpl fi;
    
  
  fi.name = f.nickname; 
  
  
  fi.inputs = f.inputs;
    
  bool found = algs.get( f.algorithm, fi.algorithm ); 
  if( false == found )
    {
      fprintf(stderr, "Error addFilter [%s], alg [%s] not found\n", f.toString().c_str(), f.algorithm.c_str() );
      exit(1);
    }
  fi.algorithm.setParams( f.params );

  
  std::map<std::string,FeatFilterImpl>::iterator iter = filters.find(fi.name);
    
  if( iter != filters.end() )
    {
      fprintf(stderr, "FilterImpl of name [%s] already exists, error\n", fi.name.c_str());
      exit(1);
    }
  else
    {
      filters[fi.name] = fi; 
      auto ret = filters.find( fi.name );
      return ret;
    }
}

bool salmap_rv::FeatFilterImplSet::getFilter( FeatFilter& f, FeatFilterImpl& fret ) {
    return getFilter( f.toString(), fret );
  }
bool salmap_rv::FeatFilterImplSet::getFilter( FeatFilter& f, FeatFilterImpl*& fret ) {
    return getFilter( f.toString(), fret );
  }
bool salmap_rv::FeatFilterImplSet::getFilter( const std::string& n, FeatFilterImpl& fret ) {
    auto iter = filters.find( n );
    if( iter == filters.end() )
      {
	return false;
      }
    else
      {
	fret = iter->second;
	return true;
      }
  }
bool salmap_rv::FeatFilterImplSet::getFilter( const std::string& n, FeatFilterImpl*& fret ) {
    auto iter = filters.find( n );
    if( iter == filters.end() )
      {
	return false;
      }
    else
      {
	fret = &(iter->second);
	
	return true;
      }
  }
