#include <salmap_rv/include/load_salmap.hpp>

#include <salmap_rv/include/salmap.hpp>
#include <salmap_rv/include/param_set.hpp>


#include <salmap_rv/include/algorithmimpl.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/featfilterimpl.hpp>

#include <salmap_rv/include/util_functs.hpp>


using namespace salmap_rv;

SalMap& salmap_rv::salmap_loader::current() {
    return *selected_salmap; 
  }
std::list<std::string> salmap_rv::salmap_loader::get_avail_maps() {
    std::list<std::string> result;
    for( auto iter=avail_maps.begin(); iter != avail_maps.end(); ++iter )
      {
	result.push_back( iter->first );
      }
    return result;
  }
 salmap_rv::salmap_loader::salmap_loader() {
    selected_salmap = NULL;
    selected_salmap_name = "";
    avail_maps.clear();
  }
void salmap_rv::salmap_loader::init( const size_t& nworkers, const uint64_t& dt_nsec, const float64_t& dva_per_pix, std::shared_ptr<ThreadPool> tp ) {
    selected_salmap = NULL;
    selected_salmap_name = "";
    avail_maps.clear();
    register_default_salmaps(nworkers, dt_nsec, dva_per_pix, tp );
  }
void salmap_rv::salmap_loader::select_salmap( const std::string& name ) {
    if( avail_maps.find( name ) == avail_maps.end() )
      {
	fprintf(stderr, "REV: requested salmap [%s] not available\n", name.c_str() );
	exit(1);
      }
    else
      {
	selected_salmap_name = name;
	selected_salmap = &( avail_maps[name] ); 
      }
  }
void salmap_rv::salmap_loader::register_default_salmaps( const size_t& nworkers, const uint64_t& dt_nsec, const float64_t& dva_per_pix, std::shared_ptr<ThreadPool> tp ) {
    fprintf(stdout, "Registering default salmaps\n");

    std::string mystring = "Default"; 
    avail_maps[ mystring ] = SalMap();
    
    param_set p = itti_formal_default_params(); 
    p.set<float64_t>( "input_dva_per_pix", dva_per_pix );
    p.set<int64_t>( "dt_nsec", dt_nsec );
    p.set<int64_t>("nsalthreads", nworkers );
    
    make_itti_dyadic_cpu_weighted_formal( p, avail_maps[mystring], tp );
#ifdef GPU
#endif
  }

