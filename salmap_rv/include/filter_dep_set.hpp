#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/filter_dep.hpp>


namespace salmap_rv
{
  struct filter_dep_set
  {
    std::map<FeatFilterImpl*, filter_dep> deps;
    int64_t max_tdelta;

    const int64_t get_max_tdelta() const;
    void set_max_tdelta( const int64_t tval );
    
    void clear();
    filter_dep_set();
    std::list<FeatFilterImpl*> get_nexts( FeatFilterImpl* ptr );
    bool check_ready( FeatFilterImpl* ptr );
    bool check_updated( FeatFilterImpl* ptr );
    void mark_updated( FeatFilterImpl* ptr );
    void reset_updated();
    std::list<FeatFilterImpl*> atomic_filters();
    void init( std::map<std::string, FeatFilter>& mymap );
    void print_deps();
  };
}
