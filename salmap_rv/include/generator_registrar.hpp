#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>




namespace salmap_rv
{
  using json = nlohmann::json;
  struct salmap_generator_base_class
  {
    salmap_generator_base_class(){};
    
    json params;
    SalMap* mysalmap; 

    
    
    
    
    template <typename T>
    T get( const std::string& s );


    void generate( SalMap*& sal, const std::string& params_json )
    {
      mysalmap = sal;
      params = json::parse( params_json );
      
      generate_funct();
    }
    
    
    void generate_funct()
    {
      fprintf(stderr, "REV: calling generate_funct() of base class.\n");
    }


    
    
    
    
    template <class ...Args>
    std::string add_sal_filter( const std::string& algname, const std::string& nickname, const std::string& prettydesc, const Args&... args )
    {
      if( mysalmap != NULL )
	{
	  return mysalmap->add_sal_filter( algname, nickname, prettydesc, args ); 
	}
      else
	{
	  fprintf(stderr, "REV: error add_sal_filter in generator_registrar...mysalmap ptr is null.\n");
	  exit(1);
	}
    } 


    
    
    
    

    
    
    
    
    
    
    
  };

  
  template <typename T>
  T salmap_generator_base_class::get( const std::string& s )
  {
    try
      {
	return params[s];
      }
    catch
      {
	fprintf(stderr, "REV: (error in JSON parsing below): enumerating params.\n");
	std::cerr << params.dump(4) << std::endl;
	fprintf(stderr, "REV: error in JSON parsing. Variable [%s] does either not exist or is of a non-compatible type. Exiting with code 1.\n", s.c_str() );
	exit(1);
      }
    
  }
  
  
  struct salmap_generator_registrar
  {
  public:

    
    
    
    algorithm_registrar(salmap_rv::GeneratorCreatorFunctionPtr funcPtr, const std::string& funcName)
    {
      salmap_rv::salmap_generator_registrar::getWritableMap().emplace(funcName, funcPtr);
    }
    
    static std::map<std::string,GeneratorCreatorFunctionPtr> const& getMap()
    {
      return getWritableMap();
    }
    
  private:
    
    static std::map<std::string, GeneratorCreatorFunctionPtr>& getWritableMap()
    {
      
      
      static std::map<std::string, GeneratorCreatorFunctionPtr> theMap;
      
      return theMap;
    }
    
  };
}
