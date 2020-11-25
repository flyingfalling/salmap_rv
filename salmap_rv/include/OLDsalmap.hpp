#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/featfilterimplset.hpp>
#include <salmap_rv/include/algorithmimplset.hpp>
#include <salmap_rv/include/featmapimplset.hpp>
#include <salmap_rv/include/filterset.hpp>
#include <salmap_rv/include/compiled_map.hpp>
#include <salmap_rv/include/exec_graph.hpp>

#include <salmap_rv/include/funcptrmap.hpp>



namespace salmap_rv
{
  
  
  struct SalMap
  {
    
    
    
    
  public:
    FilterSet filtset; 
    std::vector<std::string> outputs; 

    
    inline static const std::string INPUT_FILTER_NICKNAME = "rawinput";
    inline static const int VERSION = 0;

    const float64_t get_realtime_sec() const;

    
  private:
    FeatFilterImplSet filtimpls; 
    algorithmImplSet algimpls; 
    
    
    std::map<std::string,FeatMapImplPtr> ptroriginputs;
    std::map<std::string,FeatMapImplInst> ptroriginputinsts;
    std::shared_ptr<FeatMapImplSet> mapimpls; 
  
    compiled_map compiled_input;
    bool input_compiled;
    bool deps_built;
    bool registered_algos;
    bool registered_generators;
    bool successfully_updated;

    int64_t time_idx;
    int64_t time_nsec;
    int64_t dt_nsec;
    int64_t trial_timesteps;

    exec_graph filtergraph;

    bool consolidate_filters;
  
    double input_dva_wid;
    double input_dva_hei;
    double input_pix_per_dva;

    Timer realtime;
  
#ifdef GPU
    std::vector<cv::cuda::Stream> streams;
#endif
  
    std::shared_ptr<ThreadPool> tp;
  
    std::map<std::string, FeatMapImplPtr> all_map_accessors;
    std::map<std::string, std::string> all_map_descs;
    
    void register_generators();
    void register_generator( const std::string& s, GeneratorCreatorFunctionPtr fp );
    std::map<std::string,std::shared_ptr<salmap_generator_base_class>> generators;
    
  public:
  
    
    
    
    

    const bool was_updated() const;
    
    const int64_t get_dt_nsec() const;
    const int64_t get_time_nsec() const;

    const int64_t get_most_recent_update_time_nsec() const;
    const int64_t get_time_idx() const;
    const int64_t get_trial_timesteps() const;
  
    void advance_time();
  
    
    void reset();
  
    std::vector<std::string> get_outputs_of_filter( FeatFilter& f );
  
    
    std::vector<std::string> list_available_filters();
  
    std::vector<std::string> list_all_available_filters();

  
  
    void fill_map_accessors( );

    
    std::map<std::string, cv::Mat> return_all_maps_resized( const size_t w, const size_t h );

  
    void list_filters_and_descs( std::vector<std::string>& fs, std::vector<std::string>& descs );

    
    FeatMapImplPtr getMapAccessor( const std::string& localname, const std::string& filttag );
  
    bool mapFromAccessor( FeatMapImplPtr& accessor, std::shared_ptr<FeatMapImplInst>& ret, const int64_t& time );
  
  
    void rebalance();  

    void register_algorithms();
    
    
    

    
    cv::Mat get_map_now_nickname( const std::string& filternick, const std::string& mapname, const cv::Size& desiredsize=cv::Size(0,0) );
  

    void init( const param_set& p, std::shared_ptr<ThreadPool> _tp=nullptr );
  
    
    
    void compile_orig_input( const std::string& localname);
  
    void restart();
  
    std::string input_dummy_name( const std::string& localname );
  
    
    void register_algorithm( const std::string& name, const algoFunct_t updatef, const algoFunct_t initf = defaultInit, const algoFunct_t destroyf = defaultDestroy );
  
    
    
    
    std::string add_filter( FeatFilter& f );
  
    
    std::string add_filter( const std::string& name );
  
    
    void build_dependencies();
  

    
    void add_input_copy( const std::string& localname, const cv::Mat& data );

    
    void add_input_direct( const std::string& localname, const cv::Mat& data );

    void add_input_copy_realtime( const std::string& localname, const cv::Mat& data, const float64_t realtime_sec );

    
    void add_input_direct_realtime( const std::string& localname, const cv::Mat& data, const float64_t realtime_sec);

    
    bool insert_raw( const std::string& filtname, const std::string& outputname, const std::string& targdir, const double dva_w,  const double dva_h, const double aspect );
  
    void insert_raw_direct( const std::string& filtname, const std::string& outputname, cv::InputArray m, const double dva_w,  const double dva_h, const double aspect );
  
    
    
    
    void save_to_file( const std::string& filtname, const std::string& outputname, const std::string& targdir );
  
    bool update();

    bool update_single_thread( const int64_t& dt_in_nsec );
  
    bool update_multi_thread( const int64_t& dt_in_nsec );
  
    bool update( const int64_t& dt_in_nsec );
    void estimate_size_in_memory();

    
  
  
    
    template <class ...Args>
    std::string add_sal_filter( const std::string& algname, const std::string& nickname, const std::string& prettydesc, const Args&... args );

    
    
    
    
    
    
    
    
    taggraph taggraph_from_salmap();

  private:
    void setup_input( const std::string& localname, const cv::Mat& data );
    void set_direct(const std::string& localname, const cv::Mat& data);
    void set_copy(const std::string& localname, const cv::Mat& data);

    void set_direct_realtime(const std::string& localname, const cv::Mat& data, const float64_t realtime_sec);
    void set_copy_realtime(const std::string& localname, const cv::Mat& data, const float64_t realtime_sec);
  
  }; 

} 




template <typename T>
void salmap_rv::SalMap::set_filt_input_params( FeatFilter& f, const T& t )
{
  
  set_filt_input_param( f, t ); 
}


template <typename T, typename T2, typename ...Rest>
void salmap_rv::SalMap::set_filt_input_params( FeatFilter& f, const T& t, const T2& t2, Rest... rest)
{
  
  
  set_filt_input_param( f, t );
  set_filt_input_params( f, t2, rest... );
}
  
  

template <class ...Args>
std::string salmap_rv::SalMap::add_sal_filter( const std::string& algname, const std::string& nickname, const std::string& prettydesc, const Args&... args )
{
  
  

  
  
  auto const& FunctionPointersMap = salmap_rv::detail::FuncPtrMap::getMap();
  auto iter = FunctionPointersMap.find(algname);
  if( iter == FunctionPointersMap.end() )
    {
      fprintf(stderr, "REV: requested salmap filter function [%s] does not exist.\n", algname.c_str() );
      
      salmap_rv::detail::FuncPtrMap::enumerate();
      exit(1);
    }

  FeatFilter f;
  f.nickname = nickname;
  f.desc = prettydesc;
  f.algorithm = algname;
  
  set_filt_input_params( f, args... );
	
  add_filter( f );
  return f.nickname;
}
