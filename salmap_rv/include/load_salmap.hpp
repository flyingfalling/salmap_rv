#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>



#include <itti_salmap.hpp>

namespace salmap_rv
{
  
  struct salmap_loader {
    std::map<std::string, SalMap> avail_maps; 
    SalMap* selected_salmap; 
    std::string selected_salmap_name; 
    SalMap & current();
    std::list<std::string> get_avail_maps();
    salmap_loader();
    void init( const size_t& nworkers, const uint64_t& dt_nsec, const float64_t& dva_per_pix, std::shared_ptr<ThreadPool> tp=nullptr );
    void select_salmap( const std::string& name );
    void register_default_salmaps( const size_t& nworkers, const uint64_t& dt_nsec, const float64_t& dva_per_pix, std::shared_ptr<ThreadPool> tp );
  };
}
