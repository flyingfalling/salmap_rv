#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>



namespace salmap_rv
{
  struct filter_dep
  {
  
    std::set< std::pair<FeatFilterImpl*, int64_t> > unique_requires; 
    std::set< std::pair<FeatFilterImpl*, int64_t> > unique_required_by; 
    std::vector< FeatFilterImpl* > requires; 
    std::vector< std::string> output_mapping; 
    std::vector< std::string> input_mapping; 
    std::vector< int64_t> tdelta; 
    std::vector< FeatFilterImpl* > required_by; 
    std::vector< std::string> routput_mapping; 
    std::vector< std::string> rinput_mapping; 
    std::vector< int64_t> rtdelta; 
    bool updated;
    int64_t max_tdelta;
    void set_max_tdelta( const int64_t tval );
    const int64_t get_max_tdelta( ) const;
    bool check_updated();
    void mark_updated();
    bool is_atomic();
    filter_dep();
    filter_dep( const FeatFilter& f, std::map<std::string, FeatFilter>& mymap );
    bool check_requires_zero( FeatFilterImpl* ptr );
    bool check_required_by_zero( FeatFilterImpl* ptr );
    void build_unique_requires();
    void build_unique_required_by();
    std::vector<size_t> check_requires( FeatFilterImpl* ptr, bool onlyzero );
    std::vector<size_t> check_required_by( FeatFilterImpl* ptr, bool onlyzero );
    void add_required_by( FeatFilterImpl* ptr, const filter_dep& dep, const size_t& idx );
  };
}
