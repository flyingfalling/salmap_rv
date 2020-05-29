#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


#include <salmap_rv/include/filter_dep_set.hpp>

namespace salmap_rv
{
struct exec_graph
{
  filter_dep_set deps;

  std::list< FeatFilterImpl* > atomic_filters;
  
  std::list< FeatFilterImpl* > avail_filters;
  
  std::list< FeatFilterImpl* > curr_processing;
  
  void fill_atomic_filters();
  
  void clear();
    
  exec_graph();

  const int64_t get_max_tdelta() const;
  
  
  
  
  
  void reset_avail_filters();
  
  void init( std::map<std::string, FeatFilter>& mymap );
  void reset_updated();
  void reset_empty();
  
  void reset();

  size_t currproc();

  std::vector<std::string> get_curr_proc();
  
  void mark_finished( FeatFilterImpl* iter );

  bool nextfilter( FeatFilterImpl*& iter );

  bool done();
  
}; 
}
