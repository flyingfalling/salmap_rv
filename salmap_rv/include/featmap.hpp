#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/featfilter.hpp>


namespace salmap_rv
{
struct FeatMap
{
  std::string localname; 
  FeatFilter filter;

  std::string timevaltype;
  std::string timeval;

  
  void set_relative_time( const int64_t& offset );

  void set_absolute_time( const int64_t& offset );

  void set_absolute_index( const int64_t& offset );
  
  void set_relative_index( const int64_t& offset );

  bool is_relative_index();

  bool is_absolute_index();
  
  bool is_relative_time();

  bool is_absolute_time();

  std::string toString() const;

  std::string toStringNoNick() const;

}; 
}
