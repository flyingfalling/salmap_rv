#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>



namespace salmap_rv
{

struct FeatFilter
{
  std::string __asstring;
  std::string algorithm;
  std::map<std::string,std::string> params;
  std::map<std::string,std::string> inputs;
  std::string nickname; 
  
  
  
  
  std::string desc; 
  std::map< std::string, FeatFilterImpl>::iterator impliter;
  bool iterset;
  

  FeatFilter();
  
  
  std::string getdesc();
 



    

  std::map< std::string, FeatFilterImpl>::iterator getImplIter();
  
  void setImplIter( const std::map< std::string, FeatFilterImpl>::iterator newiter );
  
    
  
  size_t estimate_size_in_memory();
  

  void add_param( const std::string& s, const std::string& v );
  
  
  void set_algorithm( const std::string& s );
  
  void add_input( const std::string& s, FeatMap& f );

  
  void add_input( const std::string& s, const std::string& v );

  std::string toStringNoNick() const;
  
  std::string toStringNoNickStub() const;
  
  std::string toStringStub() const;

  std::string toString() const;

}; 
} 
