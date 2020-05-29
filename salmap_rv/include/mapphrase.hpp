#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{

  struct mapphrase {
    std::string localname; 
    std::string algorithm; 
    inputmap inputs; 
    parammap params; 
    std::string timevaltype; 
    std::string timeval; 
    std::string nickname; 
    std::string desc; 
    bool is_filter();
    mapphrase();
    mapphrase( const std::string& s ); 
    mapphrase( FeatMap& map ); 
    mapphrase( FeatFilter& filt ); 
    FeatMap toMap(); 
    FeatFilter toFilter(); 
    std::map<std::string,std::string> inputs_as_str_str();
    std::string enum_inputs( std::map< std::string, mapphrase >& m );
    std::string enum_params( std::map< std::string, std::string >& m );
    std::string print();
    std::string print_pretty( const size_t& tablevel=0 );
  };

 

  
}
