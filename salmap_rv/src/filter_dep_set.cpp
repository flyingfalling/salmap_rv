
#include <salmap_rv/include/filter_dep_set.hpp>

#include <salmap_rv/include/featfilterimpl.hpp>

using namespace salmap_rv;
void salmap_rv::filter_dep_set::clear()
{
  max_tdelta = 0;
  deps.clear();
}

salmap_rv::filter_dep_set::filter_dep_set()
  : max_tdelta(0)
 {
 }

std::list<FeatFilterImpl*> salmap_rv::filter_dep_set::get_nexts( FeatFilterImpl* ptr )
{
  std::list<FeatFilterImpl*> ret;
  auto it = deps.find(ptr);
  if( it == deps.end() ) { exit(1); }
  filter_dep& d = it->second;
  for( auto& rb : d.unique_required_by )
    {
      if( true == check_ready( rb.first ) )
	{
	  if( rb.second == 0 )  
	    {
	      ret.push_back( rb.first );
	    }
	}
    }
  return ret;
}

bool salmap_rv::filter_dep_set::check_ready( FeatFilterImpl* ptr )
{
  bool ready = true;
  
  auto it = deps.find(ptr);
  if( it == deps.end() ) { exit(1); }
  filter_dep& d = it->second;
  
  
  for( auto& r : d.unique_requires )
    {
      if( r.second == 0 &&
	  false == deps[ r.first ].check_updated() )
	{
	  ready = false;
	}
    }
  return ready;
}

bool salmap_rv::filter_dep_set::check_updated( FeatFilterImpl* ptr )
{
  
  auto it = deps.find(ptr);
  if( it == deps.end() ) { exit(1); };
  return it->second.check_updated();
}


void salmap_rv::filter_dep_set::mark_updated( FeatFilterImpl* ptr )
{
  auto it = deps.find(ptr);
  if( it == deps.end() ) { exit(1); };
  it->second.mark_updated();
  
}

void salmap_rv::filter_dep_set::reset_updated()
{
  for( auto iter=deps.begin(); iter != deps.end(); ++iter )
    {
      iter->second.updated = false;
    }
      
}


std::list<FeatFilterImpl*> salmap_rv::filter_dep_set::atomic_filters()
{
  std::list<FeatFilterImpl*> ret;

  for( auto iter=deps.begin(); iter != deps.end(); ++iter )
    {
      if( iter->second.is_atomic() )
	{
	  ret.push_back( iter->first );
	}
    }
  return ret;
}

void salmap_rv::filter_dep_set::init( std::map<std::string, FeatFilter>& mymap )
{
  for( auto iter=mymap.begin(); iter!=mymap.end(); ++iter )
    {
      
      
      
      deps[ &(iter->second.getImplIter()->second ) ] = filter_dep(iter->second, mymap);
    }

  for( auto iter = deps.begin(); iter != deps.end(); ++iter )
    {
      for( auto iter2 = deps.begin(); iter2 != deps.end(); ++iter2 )
	{
	  auto myidxs = iter2->second.check_requires( iter->first, false ); 
	  for( auto& idx : myidxs )
	    {
	      iter->second.add_required_by( iter2->first, iter2->second, idx );  
	    }
	}
    }

  for( auto& d : deps )
    {
      d.second.build_unique_requires();
      d.second.build_unique_required_by();
      set_max_tdelta( d.second.get_max_tdelta() );
    }
}


void salmap_rv::filter_dep_set::set_max_tdelta( const int64_t tval )
{
  if( tval < max_tdelta )
    {
      max_tdelta = tval;
    }
}

const int64_t salmap_rv::filter_dep_set::get_max_tdelta() const
{
  return max_tdelta;
}

void salmap_rv::filter_dep_set::print_deps()
{
  
  
  fprintf(stdout, "Filter deps (total: %ld filters)\n", deps.size());
  for( auto it=deps.begin(); it != deps.end(); ++it )
    {
      fprintf(stdout, "FilterImpl [%s]:\n", it->first->name.c_str());
      fprintf(stdout, "   Requires: ");
      
      for( size_t x=0; x<it->second.requires.size(); ++x )
	{
	  
	  fprintf(stdout, " [%s]=[%s]@[%s](T=%ld)", it->second.input_mapping[x].c_str(), it->second.output_mapping[x].c_str(), it->second.requires[x]->name.c_str(), it->second.tdelta[x]);
	}
      fprintf(stdout, "\n   Required by: ");
      for( size_t x=0; x<it->second.required_by.size(); ++x )
	{
	  
	  fprintf(stdout, " [%s]=[%s]@[%s](T=%ld)", it->second.rinput_mapping[x].c_str(), it->second.routput_mapping[x].c_str(), it->second.required_by[x]->name.c_str(), it->second.rtdelta[x]);
	}
      fprintf(stdout, "\n");
    }
  fprintf(stdout, "\n");
  fprintf(stdout, "Finished printing dependencies\n");
}
