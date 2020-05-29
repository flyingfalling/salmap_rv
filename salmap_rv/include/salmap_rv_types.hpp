#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>



namespace salmap_rv
{
#ifdef USE_OPENCL
  typedef cv::UMat FiltMat;
#else
  typedef cv::Mat FiltMat;
#endif

  
  namespace fusion = boost::fusion;
  namespace phoenix = boost::phoenix;
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;
  
  
  
  struct mapphrase;
  struct FeatMap;
  struct FeatFilter;
  struct FeatMapImpl;
  struct FeatFilterImpl;
  struct FeatFilterImplSet;
  struct phraseparser;
  struct FeatMapImplPtr;
  struct FeatMapImplInst;
  struct FeatMapImplSet;
  struct FilterSet;
  struct scratchmats;
  struct exec_graph;
  struct filter_dep;
  struct filter_dep_set;
  struct compiled_map;
  struct compiled_filter;
  struct algorithmImplSet;
  struct algorithmImpl;
  struct TIMETYPE;
  struct ThreadPool;
  struct param_set;
  struct taggraph;
  struct SalMap;
  struct FeatMapImplCollection;
  struct FeatMapCollectionPtr;

  struct FuncPtr;
  struct FuncPtrMap;
  struct FuncPtrRegistrar;

  
  
  namespace filter_func_dummy
  {
    
    void DUMMY_FUNC_FOR_LINKER(); 
  }

  
  template <typename T> struct rotating_time_series;
  template <typename Iterator> struct mapphrase_grammar;
  
  typedef std::map<std::string,std::string> parammap;
  typedef std::map<std::string,mapphrase> inputmap;
  typedef std::map<std::string,std::shared_ptr<FeatMapImplInst>> localmapdict;

  typedef std::function< void( localmapdict& inputmaps, std::shared_ptr<FeatMapImplCollection>& outputcollection, void*& userdata, scratchmats& scratch, const std::map<std::string,std::string>& params, const uint64_t& curr_time, const std::string& nickname, const std::string& desc
#ifdef GPU
			       , cv::cuda::Stream& stream
#endif
			       ) >   algoFunct_t;
  
  
}
