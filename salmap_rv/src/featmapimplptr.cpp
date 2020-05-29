
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/featmapimplset.hpp>
#include <salmap_rv/include/featmapimpl.hpp>

#include <salmap_rv/include/featmapimplcollection.hpp>





using namespace salmap_rv;

FeatMapImplPtr::FeatMapImplPtr()
{
  ptr=nullptr;
  mapset = nullptr;
}

FeatMapImplPtr::FeatMapImplPtr( const std::string& argn, const std::string& n, const FeatFilterImplSet& filtimpls, const FilterSet& fs, const std::shared_ptr<FeatMapImplSet>& mapsetptr )
{
  argname = argn; 
  name = n; 
  
  ptr = nullptr;
  mapset = mapsetptr;
  cm = compiled_map( name, filtimpls, fs );
  t = TIMETYPE( cm.m.timevaltype, cm.m.timeval );

  DEBUGPRINTF(stdout, "Creating featmapimplptr argname: [%s]  fullname: [%s] (cm=[%s])\n", argname.c_str(), name.c_str(), print().c_str() );
}


void FeatMapImplPtr::set_abs_curr_time( const int64_t& curr_time )
{
  t = TIMETYPE( curr_time );
}



bool FeatMapImplPtr::init( )
{  
  std::shared_ptr<FeatMapImplCollection> colptr; 

  if( nullptr == mapset )
    {
      fprintf(stderr, "mapset ptr is NULL! Exiting\n");
      exit(1);
    }

  bool colexisted = mapset->getImplCollection( cm.name_no_time_no_localname(), colptr );
  
  DEBUGPRINTF(stdout, "FeatMapImplPtr: Attempting to init: [%s]\n", print().c_str() );
  
  if( false == colexisted )
    {
      DEBUGPRINTF(stdout, "Collection didn't exist yet\n");
      return false; 
    }
  else
    {
      DEBUGPRINTF(stdout, "Collection Existed, now trying to find localname [%s]\n", cm.get_localname().c_str() );

      std::shared_ptr<FeatMapImpl> implptr;
      bool implexisted = colptr->getMapImpl( cm.get_localname(), implptr ); 
      if( false == implexisted )
	{
	  DEBUGPRINTF(stdout, "Returning FALSE: Failed to find localname [%s] in the collection...!!!\n", cm.get_localname().c_str());
	  return false; 
	}
      else
	{
	  
	  ptr = implptr;
	  DEBUGPRINTF(stdout, "Returning TRUE: Found localname [%s] (ptr is set to FilterMapImpl* featmapimpl name (nick)=[%s])!!!\n" , cm.get_localname().c_str(), ptr->name.c_str());
	  return true;
	}
    }
}

std::string FeatMapImplPtr::print( )
{
  return cm.get_localname() + " : " + cm.name_no_time_no_localname();
}






void FeatMapImplPtr::create_if_empty()
{
  if( !mapset )
    {
      fprintf(stderr, "create if empty, mapset ptr is null\n");
      exit(1);
    }
  if( nullptr == ptr )
    {
      
	
      DEBUGPRINTF(stdout, "Creating if empty! [%s]\n", name.c_str() );    
      
      
      std::shared_ptr<FeatMapImplCollection> colptr;
      bool colexisted = mapset->getImplCollection( cm.name_no_time_no_localname(), colptr );
      
      if( false == colexisted )
	{
	  fprintf(stdout, "ARG notimenolocalname (%s) : Collection didn't exist set.maps (create_if_empty in featmapimplptr)  so created collection. ADD name = (%s)\n", cm.name_no_time_no_localname().c_str(), name.c_str());
	}

      
      cm.set_localname(argname); 
      
      
      std::shared_ptr<FeatMapImpl> implptr;
      bool implexisted = colptr->getMapImpl( cm.get_localname(), implptr ); 
      ptr = implptr;
      
    }
}






















void FeatMapImplPtr::setFrom( const std::shared_ptr<FeatMapImplInst>& arg, const int64_t& curr_time )
{
  if( nullptr == mapset )
    {
      fprintf(stderr, "setFrom -- in implptr -- mapset is null, exiting\n");
      exit(1);
    }
  if( nullptr == arg )
    {
      fprintf(stderr, "ImplPtr -> setFrom -- user passed ptr arg is nullptr, disallow! localname (%s), (%s)\n", argname.c_str(), name.c_str());
      exit(1);
    }
  if( nullptr == ptr )
    {
      DEBUGPRINTF(stdout, "Attempting to Map Impl Ptr -> SET (create if empty in:! localname (%s), (%s) )\n", argname.c_str(), name.c_str());
      if( false == init() )
	{
	  create_if_empty();
	}
      if( false == init() )
	{
	  fprintf(stderr, "This should never happne\n");
	  exit(1);
	}
    }
  fprintf(stdout, "Finished init etc...now setting using getSetter? PTR BETTER BE GOOD!\n");
  if( !ptr)
    {
      fprintf(stderr, "Super warning weird err? Ptr is still null after creating..\n");
    }
  
  ptr->setTo( t, curr_time, arg );
}

const std::string FeatMapImplPtr::arg_name_I_represent() const
{
  return argname;
}

void FeatMapImplPtr::copyFrom( const std::shared_ptr<FeatMapImplInst>& arg, const int64_t& curr_time
#ifdef GPU
			  , cv::cuda::Stream& stream
#endif
			  )
{
  if( nullptr == mapset )
    {
      exit(1);
    }

  if( nullptr == arg )
    {
      fprintf(stderr, "ImplPtr -> copyFrom -- user passed ptr arg is nullptr, disallow! (attempting to copy will cause segfault -- user's mistake) localname (%s), (%s)\n", argname.c_str(), name.c_str());
      exit(1);
    }
  
  if( nullptr == ptr )
    {
      DEBUGPRINTF(stdout, "Attempting to Map Impl Ptr -> COPYTO (create if empty in:! localname (%s), (%s) )\n", argname.c_str(), name.c_str());
      if( false == init() )
	{
	  create_if_empty();
	}
      if( false == init() )
	{
	  fprintf(stderr, "This should never happne\n");
	  exit(1);
	}
    }

  std::shared_ptr<FeatMapImplInst> tmp;
  
  bool timeexisted = ptr->getSetter(t, curr_time, tmp); 
  arg->copyTo( tmp
#ifdef GPU
	      , stream
#endif 
	      ); 
}


bool FeatMapImplPtr::setTo( std::shared_ptr<FeatMapImplInst>& arg, const int64_t& curr_time
#ifdef GPU
			  , cv::cuda::Stream& stream
#endif
			  )
{
  if( nullptr == mapset )
    {
      exit(1);
    }
  
  if( nullptr == ptr )
    {
      DEBUGPRINTF(stdout, "Attempting to Map Impl Ptr -> COPYTO (create if empty in:! localname (%s), (%s) )\n", argname.c_str(), name.c_str());
      if( false == init() )
	{
	  create_if_empty();
	}
      if( false == init() )
	{
	  fprintf(stderr, "This should never happne\n");
	  exit(1);
	}
    }

  bool timeexisted = ptr->getSetter(t, curr_time, arg); 
  
  return timeexisted; 
}



bool FeatMapImplPtr::getTo( std::shared_ptr<FeatMapImplInst>& ret,  const int64_t& curr_time )
{
  if( nullptr == mapset )
    {
      exit(1);
    }
  
  DEBUGPRINTF(stdout, "REV: attempting get() in FeatMapImplPtr (note argname [%s], name [%s]\n", argname.c_str(), name.c_str());
  
  if( nullptr == ptr )
    {
      DEBUGPRINTF(stdout, "REV: GET() in FeatMapImplPtr: ptr is not yet set! (note localname [%s], name [%s])\n", argname.c_str(), name.c_str());
      if( false == init( ) )
	{
	  create_if_empty();
	}
      if( false == init() )
	{
	  fprintf(stderr, "This should never happnen\n");
	  exit(1);
	}
    }
  
  
  bool timeexisted = ptr->getGetter( t, curr_time, ret ); 
    
  return timeexisted;
}
