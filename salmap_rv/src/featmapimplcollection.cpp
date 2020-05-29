#include <salmap_rv/include/featmapimplcollection.hpp>

#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featmapimpl.hpp>


salmap_rv::FeatMapImplCollection::FeatMapImplCollection()
  : doneinit(false)
{ }

const bool salmap_rv::FeatMapImplCollection::getdoneinit() const
{
  return doneinit;
}

const void salmap_rv::FeatMapImplCollection::setdoneinit()
{
  doneinit = true;
}


bool salmap_rv::FeatMapImplCollection::getMapImpl( const std::string& mapname, std::shared_ptr<FeatMapImpl>& ret )
{

  bool existed = true;
  
  
  

  if( false == getdoneinit() )
    {
      collectionmux.lock();
      auto it = mymaps.find( mapname );
      if( mymaps.end() == it )
	{
	  mymaps[ mapname ] = std::make_shared<FeatMapImpl>( "EMPTY_LOCAL_NAME", mapname ); 
	  existed = false;
	  ret = mymaps[mapname];
	}
      else
	{
	  ret = it->second;
	}
      collectionmux.unlock();
    }
  else
    {
      auto it = mymaps.find( mapname );
      if( mymaps.end() == it )
	{
	  fprintf(stderr, "Err coll end\n");
	  exit(1);
	}
      ret = it->second;
    }
  
  
  if( nullptr == ret )
    {
      fprintf(stderr, "Impl Collection: wtf mymaps[mapname] is nullptr?\n");
      exit(1);
    }
  
  
  return existed;
}

    
bool salmap_rv::FeatMapImplCollection::getSetterFromMapImpl( const std::string& mapname, const int64_t& t, std::shared_ptr<FeatMapImplInst>& ret )
{
  std::shared_ptr<FeatMapImpl> impl = nullptr;
  bool itemexisted = getMapImpl( mapname, impl );
  if( nullptr == impl )
    {
      fprintf(stderr, "FeatMapImplCollection: wtf get setter from map impl nullptr?\n");
      exit(1);
    }
    
  bool writeover = impl->getSetter( t, ret );
  if( nullptr == ret )
    {
      fprintf(stderr, "FeatMapImplCollection: getSetter should never return nullptr...\n");
      exit(1);
    }

  return writeover;
  
}

bool salmap_rv::FeatMapImplCollection::getGetterFromMapImpl( const std::string& mapname, const int64_t& t, std::shared_ptr<FeatMapImplInst>& ret )
{
  std::shared_ptr<FeatMapImpl> impl = nullptr;
  bool itemexisted = getMapImpl( mapname, impl );
  if( nullptr == impl )
    {
      fprintf(stderr, "REV :wtf get getter from map impl nullptr?\n");
      exit(1);
    }
  
  return impl->getGetter( t, ret ); 
}


std::vector<std::string> salmap_rv::FeatMapImplCollection::list_outputs()
{
  std::vector<std::string> ret;
  for( auto it = mymaps.begin(); it != mymaps.end(); ++it )
    {
      ret.push_back(it->first);
    }
  return ret;
}

void  salmap_rv::FeatMapImplCollection::reset()
{
  
  mymaps.clear();
}

void salmap_rv::FeatMapImplCollection::enum_collection( const std::string& filtname ) const
{
  for( auto it=mymaps.begin(); it != mymaps.end(); ++it )
    {
      DEBUGPRINTF(stdout, "FeatMapImplCollection ENUM : output key: [%s] in [%s]  in filter:  [%s]\n", it->first.c_str(), it->second->name.c_str(), filtname.c_str() );
    }
  
}
