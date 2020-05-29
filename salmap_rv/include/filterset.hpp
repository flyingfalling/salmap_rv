#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{
  struct FilterSet
  {
    std::map<std::string,FeatFilter> filters; 
    size_t nickname_ctr; 
    std::string nickname_string = "__NO_NICK"; 
    std::string newnick();
    FilterSet();
    void clear();
    std::string add_filter( FeatFilter& f );
    std::string enum_filters(); 
    void check_filters(const bool consolidate ); 
    std::string recursive_replace_nick( const std::string& input, const std::string& oldnick, const std::string& newnick, phraseparser& pp ); 
    void substitute_instances( const std::string oldnick, const std::string newnick, phraseparser& pp ); 
    void consolidate_filters( );
    FeatFilter& getFullFilter( const std::string arg ); 
    void reinit_filters(); 
    bool add_filter_by_string( const std::string& s, std::string& filtername ); 
  };
}
