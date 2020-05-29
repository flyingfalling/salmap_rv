
#include <salmap_rv/include/featmapimplset.hpp>

#include <salmap_rv/include/featmapimpl.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/compiled_map.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>

#include <salmap_rv/include/featmapimplcollection.hpp>

using namespace salmap_rv;



std::vector<std::string> salmap_rv::FeatMapImplSet::list_outputs( const std::string& filtname )
{
  std::vector<std::string> ret;
  auto it = collections.find( filtname );
  if( it != collections.end() )
    {
      ret = it->second->list_outputs();
    }
  else
    {
      DEBUGPRINTF(stderr, "WARNING/ERROR: Filter [%s] did not exist in Feat Map Impl Set\n", filtname.c_str())
    }
  
  if( ret.empty() )
    {
      fprintf(stderr, "------ REV: Wtf, filter [%s] has no outputs?\n", filtname.c_str() );
      for( auto it3=collections.begin(); it3!=collections.end(); ++it3)
	{
	  DEBUGPRINTF(stderr, "(Listing all FeatMapImplSet keys) FeatMapImplSet filter entry: [%s]\n", it3->first.c_str() );
	}
    }
  return ret;
}


void salmap_rv::FeatMapImplSet::reset()
{
  for( auto iter1 = collections.begin(); iter1 != collections.end(); ++iter1 )
    {
      iter1->second->reset();
    }
}


void salmap_rv::FeatMapImplSet::clear()
{
  collections.clear();
}


void salmap_rv::FeatMapImplSet::enum_maps()
{
  DEBUGPRINTF(stdout, "Enumerating Impl Maps Set:\n");
  for( auto iter = collections.begin(); iter != collections.end(); ++iter )
    {
      iter->second->enum_collection( iter->first );
    }
  DEBUGPRINTF(stdout, "**Finished Enum Impl Maps Set\n");
}









bool salmap_rv::FeatMapImplSet::getImplCollection( const std::string& filtname, std::shared_ptr<FeatMapImplCollection>& ret )
{
  bool existed=true;
  DEBUGPRINTF(stdout, "Attempting get impl collection (%s)\n", filtname.c_str());
  
  if( false == getdoneinit() )
    {
      implsetmux.lock();
      
      auto it = collections.find( filtname );
    
      if( collections.end() == it )
	{
	  DEBUGPRINTF(stdout, "mapset->getImplCollection, collections for [%s] didn't exist so creating with new\n", filtname.c_str() );
	  collections[ filtname ] = std::make_shared<FeatMapImplCollection>();
	  existed = false;
	  ret = collections[ filtname ];
	}
      else
	{
	  ret = it->second;
	}

      implsetmux.unlock();
    }
  else
    {
      auto it = collections.find( filtname );
      if( it == collections.end() )
	{
	  fprintf(stderr, "Fail, done init but not exist\n");
	  exit(1);
	}
      else
	{
	  ret = it->second;
	}
    }
  

  
  

  
  
  if( nullptr == ret )
    {
      fprintf(stderr, "Impl Set: wtf collection[filtname] is nullptr?\n");
      exit(1);
    }
  
  return existed;
}


const bool salmap_rv::FeatMapImplSet::getdoneinit() const
{
  return doneinit;
}

const void salmap_rv::FeatMapImplSet::setdoneinit()
{
  doneinit = true;
  for( auto it = collections.begin(); it != collections.end(); ++it )
    {
      it->second->setdoneinit();
    }
}











salmap_rv::FeatMapImplSet::FeatMapImplSet()
  : doneinit(false)
{ }























 
