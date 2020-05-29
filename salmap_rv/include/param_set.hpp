#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{

  struct param_set {
    
    param_set();
    std::map< std::string, std::variant<std::string, int64_t, float64_t, bool> > dict; 
    std::string header = "### F: float64, I: int64, B: boolean, S: string"; 
    std::string Fmark = "F"; 
    std::string Imark = "I"; 
    std::string Bmark = "B"; 
    std::string Smark = "S"; 
    template <typename T> void set( const std::string n, const T i);
    template <typename T> T get( const std::string n ) const;
    template <typename T> bool istype( const std::string n ) const;
    bool exists( const std::string n ) const;
    template <typename T> bool get( const std::string n, T& ret ) const;
    std::string tostring( const std::variant<std::string, int64_t, float64_t, bool> v ) const;
    void enumerate() const;
    void tofile( const std::string& fname );
    void fromfile( const std::string& fname );
  };
}


using namespace salmap_rv;

template <typename T>
void param_set::set( const std::string n, const T i)
{
  auto iter = dict.find( n );
  if( iter != dict.end() )
    {
      fprintf(stderr, "Warning: resetting existing variable in dict paramset [%s]\n", n.c_str() );
    }
  
  dict[n] = i;
}
template <typename T>
T param_set::get( const std::string n ) const
{
  auto iter = dict.find(n);
  if( iter == dict.end() )
    {
      fprintf(stderr, "REV: requested variable [%s] did not exist (of any type)\n", n.c_str());
      enumerate();
      exit(1);
    }
  
  try
    {
      T val = std::get<T>(iter->second);
      return val;
    }
  catch( const std::bad_variant_access& bva )
    {
      fprintf(stderr, "REV: tried to get for var [%s], wasn't correct type!\n", n.c_str());
      exit(1);
    }
}

template <typename T>
bool param_set::get( const std::string n, T& ret ) const
{
  auto iter = dict.find(n);
  if( iter == dict.end() )
    {
      return false;
    }
  
  try
    {
      std::get<T>(iter->second);
      return true;
    }
  catch( const std::bad_variant_access& bva )
    {
      fprintf(stderr, "REV: tried to get for var [%s], wasn't correct type!\n", n.c_str());
      return false; 
      
    }
}


template <typename T>
bool param_set::istype( const std::string n ) const
{
  auto iter = dict.find(n);
  if( iter == dict.end() )
    {
      fprintf(stderr, "REV: var [%s], doesn't exist!\n", n.c_str());
      enumerate();
      exit(1);
    }
  return std::holds_alternative<T>( iter->second );
}
