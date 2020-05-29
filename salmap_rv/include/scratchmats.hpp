#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{
  struct scratchmats
  {
    std::map< std::string, std::shared_ptr<FeatMapImplInst> > maps; 
    std::map< std::string, float64_t > float64s; 
    std::map< std::string, int64_t > int64s;
    std::map< std::string, cv::Size > sizes;
    std::map< std::string, std::vector<float64_t> > float64vs;
    std::map< std::string, std::vector<int64_t> > int64vs;
    std::map< std::string, std::vector<cv::Size> > sizevs;

    
    
    
    bool isinit;
  
#ifdef GPU
    std::map< std::string, cv::Ptr<cv::cuda::Filter> > gpufilters; 
    cv::Ptr<cv::cuda::Filter> & gpufilter( const std::string& s );
#endif
    
    scratchmats();
    void reset();
    bool initialized();

    float64_t & float64( const std::string& s );
    int64_t & int64( const std::string& s );
    cv::Size & size( const std::string& s );
    
    std::vector<float64_t> & float64v( const std::string& s );
    std::vector<int64_t> & int64v( const std::string& s );
    std::vector<cv::Size> & sizev( const std::string& s );
    
    const std::shared_ptr<FeatMapImplInst>& get( const std::string& s );
    std::shared_ptr<FeatMapImplInst>& get_w( const std::string& s );

    const salmap_rv::FiltMat& cpu( const std::string& s );
    salmap_rv::FiltMat& cpu_w( const std::string& s );
    
    const cv::cuda::GpuMat& gpu( const std::string& s );
    cv::cuda::GpuMat& gpu_w( const std::string& s );
  };
}
