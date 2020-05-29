#include <salmap_rv/include/featfilterimpl.hpp>
#include <salmap_rv/include/filterset.hpp>

#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/featmapimplcollection.hpp>


using namespace salmap_rv;
std::string salmap_rv::FeatFilterImpl::toString( const FilterSet& fs ) const
{
  auto myf = fs.filters.find( name );
  if( myf == fs.filters.end() )
    {
      fprintf(stderr, " featfilterimpl [%s] has no corresp fitler...\n", name.c_str() );
      exit(1);
    }
  else
    {
      return myf->second.toString(); 
    }
}

void salmap_rv::FeatFilterImpl::clear()
{
  name="";
  inputs.clear();
  
  ptrinputs.clear();
  inputs_compiled = false;
  collection_compiled = false;
}

salmap_rv::FeatFilterImpl::FeatFilterImpl() : inputs_compiled(false), collection_compiled(false)
{
}


void salmap_rv::FeatFilterImpl::compile( const FeatFilterImplSet& filtimpls, const FilterSet& fs, const std::shared_ptr<FeatMapImplSet>& mapsetptr )
{
  if( false == inputs_compiled || false == collection_compiled )
    {
      auto it = fs.filters.find(name);
      if( it == fs.filters.end() )
	{
	  fprintf(stderr, "REV: huh this is weird...\n");
	  exit(1);
	}
      std::string mystr = it->second.toString();

      if( false == collection_compiled )
	{
	  DEBUGPRINTF(stdout, "Outputs (Collection) not yet compiled, so attempting to compile collection for filtimpl [%s]...\n", name.c_str());
	  
	  ptrcollection = FeatMapCollectionPtr( mystr, filtimpls, fs, mapsetptr );
	  collection_compiled = true;
	}

      if( false == inputs_compiled )
	{
	  DEBUGPRINTF(stdout, "Inputs not yet compiled, so attempting to compile inputs for filtimpl [%s]...\n", name.c_str());
	  
	  

	  
	  auto it = fs.filters.find(name);
	  if( it == fs.filters.end() )
	    {
	      fprintf(stderr, "REV: huh this is weird...\n");
	      exit(1);
	    }
	  std::string mystr = it->second.toString();
	  
	  DEBUGPRINTF(stdout, "Will compile name for [%s] (note full filters name is [%s])\n", name.c_str(), mystr.c_str() );
	  compiled_name = compiled_map( mystr, filtimpls, fs ); 
	  DEBUGPRINTF(stdout, "Finished compile name for [%s] (note full filters name is [%s])\n", name.c_str(), mystr.c_str() );
	  
	  for( auto iter = inputs.begin(); iter != inputs.end(); ++iter )
	    {
	      
	      
	      
	      DEBUGPRINTF(stdout, "About to pushback to ptrinputs from inputs of me (name=%s)..[%s] = [%s]\n", name.c_str(), iter->first.c_str(), iter->second.c_str());

	      ptrinputs.push_back( FeatMapImplPtr( iter->first, iter->second, filtimpls, fs, mapsetptr ) );
	    
	      DEBUGPRINTF(stdout, "FINISHED pushback to ptrinputs from inputs of me (name=%s)..[%s] = [%s]\n", name.c_str(), iter->first.c_str(), iter->second.c_str());
	    }
	
	  inputs_compiled = true;
	  DEBUGPRINTF(stdout, "Finished compiling inputs for filtimpl [%s]\n", name.c_str());
	}
    }
}

bool salmap_rv::FeatFilterImpl::check_fill_inputs( const std::shared_ptr<FeatMapImplSet>& mapsetptr, const uint64_t& curr_time, localmapdict& retlocalinputs, const FeatFilterImplSet& filtimpls, const FilterSet& fs
#ifdef GPU
						   , cv::cuda::Stream& stream
#endif
						   )
{
  DEBUGPRINTF(stdout, "FeatFilterImpl (%s): check_fill_inputs: will exec compile_inputs...\n", name.c_str());
  
  
  
  bool success=true;
        
  for( size_t x=0; x<ptrinputs.size(); ++x)
    {

#ifdef ALLOW_NONEXISTENT_MAP_INPUTS
      bool found = ptrinputs[x].setTo( retlocalinputs[ptrinputs[x].arg_name_I_represent() ], curr_time ); 
      
      
      if( false == found )
	{
	  if( false == retlocalinputs[ptrinputs[x].arg_name_I_represent()]->isDummy() )
	    {
	      fprintf(stderr, "REV: huh, even though found==false, fm is not dummy...weird?\n");
	      exit(1);
	    }
	  else
	    {
	      DEBUGPRINTF("NOTICE: check_fill_inputs, [%s]: input [%s] is NOT FOUND!\n", name.c_str(), ptrinputs[x].arg_name_I_represent().c_str());
	    }
	}
      else
	{
	  
	  cv::Size tmp = retlocalinputs[ptrinputs[x].arg_name_I_represent() ]->size();
	  if( tmp.width <= 0 || tmp.height <= 0 )
	    {
	      fprintf(stderr, "REV: WARNING: input check_fill_inputs, [%s]: input [%s] is SIZE ZERO (dummy?)!\n", name.c_str(), ptrinputs[x].arg_name_I_represent().c_str() );
	      
	    }
	}
      
#else
      bool found = ptrinputs[x].getTo( retlocalinputs[ptrinputs[x].arg_name_I_represent() ], curr_time ); 
      if( false == found )
	{
	  success = false;
	}
#endif
      
    } 
  
  return success;
}




bool salmap_rv::FeatFilterImpl::update( const std::shared_ptr<FeatMapImplSet>& mapsetptr, const uint64_t& curr_time, const FeatFilterImplSet& filtimpls, const FilterSet& fs
#ifdef GPU
					, cv::cuda::Stream& stream
#endif
					)
{
  
  compile( filtimpls, fs, mapsetptr ); 
  
  std::shared_ptr<FeatMapImplCollection> mycol = ptrcollection.getCollection();
  
  Timer tx;
#if DEBUG > 5
  Timer ta;
#endif

  
  bool inputsexist = check_fill_inputs( mapsetptr, curr_time, localinputs, filtimpls, fs
#ifdef GPU
					, stream
#endif
					);

#if TIMINGLEVEL > 0
  fprintf(stdout, "-- Checkinput [%3.6lf] msec [%s]\n", tx.elapsed()*1e3, name.c_str() );
  tx.reset();
#endif
  
  if( inputsexist )
    {
      DEBUGPRINTF(stdout, "Attempting update algo [%s]\n", algorithm.name.c_str() );
      algorithm.update( localinputs, mycol, curr_time, name
#ifdef GPU
			, stream
#endif
			);

      

      
      
#if TIMINGLEVEL > 0
      fprintf(stdout, "++ Updated in [%3.6lf] msec : [%s] algo: [%s]\n", tx.elapsed()*1e3 , name.c_str(), algorithm.name.c_str() );
#endif
      
      return true;
    }
  else
    {
      DEBUGPRINTF(stdout, "Inputs did not yet exist to update algo [%s]\n", algorithm.name.c_str() );
      return false;
    }
    
}



  
  

  
  
  

  
  

  
  
  
  
  

  
