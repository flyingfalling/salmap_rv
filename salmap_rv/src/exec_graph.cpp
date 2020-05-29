#include <salmap_rv/include/exec_graph.hpp>

#include <salmap_rv/include/featfilterimpl.hpp>
using namespace salmap_rv;


void salmap_rv::exec_graph::fill_atomic_filters()
{
  atomic_filters = deps.atomic_filters();
}
  
void salmap_rv::exec_graph::clear()
{
  deps.clear();
  atomic_filters.clear();
  avail_filters.clear();
  curr_processing.clear();
}

const int64_t salmap_rv::exec_graph::get_max_tdelta() const
{
  return deps.get_max_tdelta();
}

salmap_rv::exec_graph::exec_graph()
{
}





void salmap_rv::exec_graph::reset_avail_filters()
{
  avail_filters = atomic_filters;
}
  
void salmap_rv::exec_graph::init( std::map<std::string, FeatFilter>& mymap ) 
{
  deps.init( mymap );
  fill_atomic_filters();
    
  reset();
}

void salmap_rv::exec_graph::reset_updated()
{
  deps.reset_updated();
}

void salmap_rv::exec_graph::reset_empty()
{
  reset();
  avail_filters.clear();
}
  
void salmap_rv::exec_graph::reset()
{
  curr_processing.clear();
  reset_avail_filters();
  reset_updated();
}

size_t salmap_rv::exec_graph::currproc()
{
  return curr_processing.size();
}

std::vector<std::string> salmap_rv::exec_graph::get_curr_proc()
{
  std::vector<std::string> ret;
  for( auto& a : curr_processing )
    {
      ret.push_back(a->name.c_str());
    }
  return ret;
}
  
void salmap_rv::exec_graph::mark_finished( FeatFilterImpl* iter )
{
  deps.mark_updated( iter );
  curr_processing.remove( iter );
  std::list<FeatFilterImpl*> nexts = deps.get_nexts( iter );
  
  
  avail_filters.splice( avail_filters.end(), nexts );
}

bool salmap_rv::exec_graph::nextfilter( FeatFilterImpl*& iter )
{
  if( avail_filters.size() > 0 )
    {
      iter = avail_filters.front();
      avail_filters.pop_front(); 
      curr_processing.push_back( iter );
      return true;
    }
  else
    {
      return false;
    }
}

bool salmap_rv::exec_graph::done()
{
  if( avail_filters.empty() && curr_processing.empty() )
    {
      return true;
    }
  return false;
}
