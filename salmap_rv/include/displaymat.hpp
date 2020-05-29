#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/featfilter.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/salmap.hpp>

namespace salmap_rv
{
  struct displaymat
  {
    cv::Mat mymat; 
    cv::Mat mymatraw; 
    std::string selected_model; 
    bool isselected; 
    FeatFilter f; 
    std::shared_ptr<FeatMapImplInst> fm; 
    FeatMapImplPtr fp; 
    std::string outputname; 
    bool hasmat; 
    bool hasprettymat; 
    bool pretty; 
    std::mutex mux; 
    bool gotoutputs; 
    std::string desc; 
    std::vector<std::string> outputs; 
    void reset();
    displaymat();
    void set_filter( SalMap& salmap, const std::string& name );
    void update( SalMap& salmap );
    void make_pretty( const cv::Mat& raw, const double alpha, const bool drawEye, const int eyeres, const bool drawTime, const bool drawName, const float eyex, const float eyey, const int64_t time, const std::string name );
    void getMat( cv::Mat& ret );
  };
}
