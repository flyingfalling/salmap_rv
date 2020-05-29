#include <salmap_rv/include/featmapcollectionptr.hpp>
#include <salmap_rv/include/featmapimplset.hpp>
#include <salmap_rv/include/featmapimpl.hpp>

using namespace salmap_rv;

FeatMapCollectionPtr::FeatMapCollectionPtr()
{
  ptr=nullptr;
  mapset = nullptr;
}

FeatMapCollectionPtr::FeatMapCollectionPtr( const std::string& n, const FeatFilterImplSet& filtimpls, const FilterSet& fs, const std::shared_ptr<FeatMapImplSet>& mapsetptr )
{
  name = n; 
  
  ptr = nullptr;
  mapset = mapsetptr;
  cm = compiled_map( name, filtimpls, fs );
}

std::string FeatMapCollectionPtr::print( )
{
  return cm.get_localname() + " : " + cm.name_no_time_no_localname();
}

bool FeatMapCollectionPtr::init()
{
  DEBUGPRINTF(stdout, "FeatMapCollectionPtr: Attempting to init: [%s] (%s)\n", name.c_str(), print().c_str() );
  bool colexisted = mapset->getImplCollection( cm.name_no_time_no_localname(), ptr );
  return colexisted;
}
    
std::shared_ptr< FeatMapImplCollection > FeatMapCollectionPtr::getCollection( )
{
  if( nullptr == mapset )
    {
      exit(1);
    }

  DEBUGPRINTF(stdout, "REV: attempting get() in FeatMapCollectionPtr (note name [%s]\n",  name.c_str());

  if( nullptr == ptr )
    {
      DEBUGPRINTF(stdout, "REV: GET() in FeatMapCollectionPtr: ptr is not yet set! (note  name [%s])\n", name.c_str());
      if( false == init( ) )
	{
	  DEBUGPRINTF(stdout, "REV: GET() in FeatMapCollectionPtr: COLLECTION did not exist (created I guess)! (note name [%s])\n", name.c_str());
	}
      else
	{
	  DEBUGPRINTF(stdout, "REV: GET() in FeatMapCollectionPtr: COLLECTION ***did*** exist already (note  name [%s])\n",  name.c_str());
	}
    }
       
  return ptr;
}
