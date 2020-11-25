#include <salmap_rv/include/salmap_rv_interface.hpp>

#include <salmap_rv/include/salmap.hpp>
#include <salmap_rv/include/itti_salmap.hpp>
#include <salmap_rv/include/param_set.hpp>
#include <salmap_rv/include/util_functs.hpp>


using namespace salmap_rv;

int salmap_vers( )
{
  return salmap_rv::SalMap::VERSION;
}

void* salmap_ptr_init( const std::string& _paramset_fname, const float64_t input_dva_wid, const float64_t input_dva_hei )
  {
    std::string paramset_fname(_paramset_fname);
    param_set p = itti_formal_default_params();
    
    
    
    if( false == paramset_fname.empty() )
      {
	p.fromfile( paramset_fname );
      }
    
    
    p.set<int64_t>("dt_nsec", 1 );
    p.set<float64_t>("input_dva_wid", input_dva_wid);
    p.set<float64_t>("input_dva_hei", input_dva_hei);
    
    p.enumerate();
  
    SalMap* cpusalmap = new SalMap();
    make_itti_dyadic_cpu_weighted_formal( p, *cpusalmap, nullptr );

    return (void*)cpusalmap;
  }

  void salmap_ptr_uninit( void* _salmap )
  {
    SalMap* salmap = (SalMap*)_salmap ;
    delete salmap;
    
  }

  void salmap_add_input( void* _salmap, const std::string& _inputmapname, cv::Mat& inputmat )
  {
    std::string inputmapname( _inputmapname );
    SalMap* salmap = (SalMap*) _salmap;
    
    
    salmap->add_input_direct_realtime( inputmapname, inputmat, salmap->get_realtime_sec() ); 
  }

  int64_t salmap_update( void* _salmap )
  {
    SalMap* salmap = (SalMap*) _salmap;
    bool updated = salmap->update();
    int64_t time = -1;
    if( updated )
      {
	time = salmap->get_time_nsec(); 
      }
    
    return time;
  }

  
  cv::Mat salmap_get_map_now_nickname( void* _salmap, const std::string& _filtername, const std::string& _mapname )
  {
    
    
    SalMap* salmap = (SalMap*) _salmap;


    cv::Mat ret = salmap->get_map_now_nickname( _filtername, _mapname, cv::Size(0,0) );
    
    return ret;
  }

  
  cv::Mat salmap_get_map_pretty( void* _salmap, const std::string& _filtername, const std::string& _mapname, const std::string& _overlaywithinputmap, const float64_t alpha, const int resize_wid_pix )
  {
    
    
    
    SalMap* salmap = (SalMap*) _salmap;
    
    cv::Mat raw = salmap->get_map_now_nickname( salmap_rv::SalMap::INPUT_FILTER_NICKNAME, _overlaywithinputmap );
    
    cv::Mat sal = salmap->get_map_now_nickname( _filtername, _mapname );
    
    fflush(stderr);

    cv::Mat ret;
    if( raw.empty() || sal.empty() )
      {
	fprintf(stderr, "REV: LOLOLOLOL raw or sal was empty?\n");
	return ret;
      }
    
    int interp = cv::INTER_LINEAR;
    cv::Mat rraw = resize_wid( raw, resize_wid_pix, interp );
    cv::Mat rsal = resize_wid( sal, resize_wid_pix, interp );
    cv::Mat colorsal = apply_color( rsal );
    ret = overlay_salmap( rraw, colorsal, alpha, interp );
    
    return ret;
  }
