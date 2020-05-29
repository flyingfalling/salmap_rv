#include <salmap_rv/include/filter_dep.hpp>

#include <salmap_rv/include/featfilter.hpp>
#include <salmap_rv/include/featfilterimpl.hpp>
#include <salmap_rv/include/mapphrase.hpp>

#include <salmap_rv/include/phraseparser.hpp>
#include <salmap_rv/include/featmap.hpp>


using namespace salmap_rv;
bool salmap_rv::filter_dep::check_updated() {
    return updated;
  }

void salmap_rv::filter_dep::mark_updated() {
    updated = true;
  }

bool salmap_rv::filter_dep::is_atomic() {
    if( 0 == requires.size() )
      {
	return true;
      }
    else
      {
	return false;
      }
  }

salmap_rv::filter_dep::filter_dep()
  : max_tdelta(0), updated(false)
{  }


salmap_rv::filter_dep::filter_dep( const FeatFilter& f, std::map<std::string, FeatFilter>& mymap )
  : max_tdelta(0), updated(false)
{
  
  for( auto iter = f.inputs.begin(); iter != f.inputs.end(); ++iter )
    {
      phraseparser p;
      mapphrase mp = p.parse( iter->second );
      FeatMap mm = mp.toMap();
      std::string timeval = mm.timeval;
      
	
	
      std::stringstream ss(timeval);
      int64_t itimeval;
      if( !(ss >> itimeval) )
	{
	  fprintf(stderr, "itimeval ss does not contain int64 wtf\n");
	  exit(1);
	}
	
	
      if( false == mm.is_relative_time() )
	{
	  fprintf(stderr, "REV: non-relative timings are not implemented yet (or broken)\n");
	  exit(1);
	}

      
      
      

	
      std::string localinputname = iter->first;
      
      

      
      std::string localoutputname = mm.localname;
	    
      
      

      
      
      auto i2 = mymap.find( mm.filter.nickname ); 
      if( i2 == mymap.end() )
	{
	  fprintf(stderr, "REV: filter_dep for filter nick (%s), required input [%s] (inputname=%s, outputname=%s) doesn't exist, exiting...(my map has %ld filts)\n", f.nickname.c_str(), iter->second.c_str(), localinputname.c_str(), localoutputname.c_str(), mymap.size() );
	  for( auto it=mymap.begin(); it != mymap.end(); ++it )
	    {
	      fprintf(stderr, "[%s]\n", it->first.c_str() ); 
	    }
	  exit(1);
	}

	    
	    
      requires.push_back( & ( i2->second.getImplIter()->second ) );
      input_mapping.push_back( localinputname );
      output_mapping.push_back( localoutputname );
      tdelta.push_back( itimeval );
      
      set_max_tdelta( itimeval );
	
    }
}


void salmap_rv::filter_dep::set_max_tdelta( const int64_t tval )
{
  if( tval < max_tdelta )
    {
      max_tdelta = tval;
    }
}

const int64_t salmap_rv::filter_dep::get_max_tdelta( ) const
{
  return max_tdelta;
}


bool salmap_rv::filter_dep::check_requires_zero( FeatFilterImpl* ptr ) {
    size_t nfound=unique_requires.count( std::pair<FeatFilterImpl*,int64_t>(ptr,0) );
    if( nfound > 0 )
      {
	return true;
      }
    else
      {
	return false;
      }
  }
bool salmap_rv::filter_dep::check_required_by_zero( FeatFilterImpl* ptr ) {
    size_t nfound=unique_required_by.count( std::pair<FeatFilterImpl*,int64_t>(ptr,0) );
    if( nfound > 0 )
      {
	return true;
      }
    else
      {
	return false;
      }
  }
void salmap_rv::filter_dep::build_unique_requires() {
    unique_requires.clear();
    for( size_t x=0; x<requires.size(); ++x )
      {
	unique_requires.emplace( requires[x], tdelta[x] );
      }
  }

void salmap_rv::filter_dep::build_unique_required_by()
{
  unique_required_by.clear();
  for( size_t x=0; x<required_by.size(); ++x )
    {
      unique_required_by.emplace( required_by[x], rtdelta[x] );
    }
}

std::vector<size_t> salmap_rv::filter_dep::check_requires( FeatFilterImpl* ptr, bool onlyzero )
{
  std::vector<size_t> ret;
  for( size_t x=0; x<requires.size(); ++x )
    {
      if( ptr == requires[x] )
	{
	  if( true == onlyzero &&
	      0 != tdelta[x] )
	    {
	    }
	  else
	    {
	      ret.push_back(x);
	    }
	}
    }
  return ret;
}

std::vector<size_t> salmap_rv::filter_dep::check_required_by( FeatFilterImpl* ptr, bool onlyzero )
{
  std::vector<size_t> ret;
  for( size_t x=0; x<required_by.size(); ++x )
    {
      if( ptr == required_by[x] ) 
	{
	  if( true == onlyzero &&
	      0 != rtdelta[x] )
	    {
	    }
	  else
	    {
	      ret.push_back(x);
	    }
	}
    }
  return ret;
}

void salmap_rv::filter_dep::add_required_by( FeatFilterImpl* ptr, const filter_dep& dep, const size_t& idx )
{
  
  required_by.push_back( ptr );
  rtdelta.push_back( -1 * dep.tdelta[idx] );
  routput_mapping.push_back( dep.output_mapping[idx] );
  rinput_mapping.push_back( dep.input_mapping[idx] );
}
