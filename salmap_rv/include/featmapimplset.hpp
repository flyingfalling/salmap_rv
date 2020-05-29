#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

namespace salmap_rv
{

  struct FeatMapImplSet
  {
  private:
    std::map< std::string, std::shared_ptr<FeatMapImplCollection> > collections;
    std::mutex implsetmux;
    bool doneinit;
    
    
    
  public:
    std::vector<std::string> list_outputs( const std::string& filtname );
    void reset();
    void clear();
    
    
    void enum_maps();
    bool getImplCollection(const std::string& filtname, std::shared_ptr<FeatMapImplCollection>& ret);

    FeatMapImplSet();
    const void setdoneinit();
    const bool getdoneinit() const;
    
     
  };
}
