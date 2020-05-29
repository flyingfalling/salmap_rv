#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{
  
  
  struct FeatMapImplCollection
  {
  private:
    std::map< std::string, std::shared_ptr<FeatMapImpl> > mymaps;
    std::mutex collectionmux;
    bool doneinit;
    
  public:
    
    
    bool getMapImpl( const std::string& mapname, std::shared_ptr<FeatMapImpl>& ret );
    
    
    bool getSetterFromMapImpl( const std::string& mapname, const int64_t& t, std::shared_ptr<FeatMapImplInst>& ret );

    
    bool getGetterFromMapImpl( const std::string& mapname, const int64_t& t, std::shared_ptr<FeatMapImplInst>& ret );
    
    std::vector<std::string> list_outputs();

    const bool getdoneinit() const;
    const void setdoneinit();
    FeatMapImplCollection();
    
    void reset();

    void enum_collection( const std::string& filtname ) const;
  };
}
