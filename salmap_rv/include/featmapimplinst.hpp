







#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{
  struct FeatMapImplInst
  {

  public:
    

    bool initialized;
    
    salmap_rv::FiltMat cpu_data; 
    cv::cuda::GpuMat gpu_data;
    bool ongpu; 
    bool oncpu; 
    
    int64_t time_nsec; 

    float64_t dva_wid;
    float64_t dva_hei;

    float64_t origaspectratio_wh;
    
    static void instantiateIfVoid( std::shared_ptr<FeatMapImplInst>& p );
    
  public:

    

    const bool isDummy() const;

    void setDummy();
    
    void setHasGpu( const bool val );
    void setHasCpu( const bool val );
    
    
    const bool hascpu() const;

    const bool hasgpu() const;

    const cv::Size size() const;
    

    const double get_dva_wid() const;

    const double get_dva_hei( ) const;


    void set_dva_wid( const double& d );

    void set_dva_hei( const double& d );


    const double get_dva_per_pix_wid() const;

    const double get_dva_per_pix_hei() const;


    const double get_pix_per_dva_wid() const;

    const double get_pix_per_dva_hei() const;
    
    
    const double get_origaspectratio_wh() const;
    
    void set_origaspectratio_wh( const double& d );


    void set_time( const int64_t& t );

    const int64_t get_time( ) const;


    const bool empty() const;

    void cpu_to_gpu(
#ifdef GPU
		    cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		    );


    void gpu_to_cpu(
#ifdef GPU
		    cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		    );

    salmap_rv::FiltMat& cpu_w(
#ifdef GPU
						      cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
						      );

    const salmap_rv::FiltMat& cpu(
#ifdef GPU
						   cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		       ) ;

    cv::cuda::GpuMat& gpu_w(
#ifdef GPU
						      cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
						      );

    const cv::cuda::GpuMat gpu(
#ifdef GPU
						   cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		       ) ;


    FeatMapImplInst( );

    FeatMapImplInst( const int64_t& timensec, const float64_t& dvaw, const float64_t& dvah, const float64_t& aspect_wh  );
    
     

    
    void copyFromCpu( cv::InputArray from );
    
    
    void copyFromGpu( cv::InputArray from
#ifdef GPU
						  , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
						  );


    void copyTo( std::shared_ptr<FeatMapImplInst>& myclone
#ifdef GPU
					     , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
					     );
    void copyCpuTo( std::shared_ptr<FeatMapImplInst>& myclone
#ifdef GPU
					     , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
					     );
    void copyGpuTo( std::shared_ptr<FeatMapImplInst>& myclone
#ifdef GPU
					     , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
					     );

    
    void copyCpuTo( salmap_rv::FiltMat& clonedmat ) ;

#ifdef USE_OPENCL
    void copyCpuTo( cv::Mat& clonedmat ) ;
#endif 
    
    void copyGpuTo( salmap_rv::FiltMat& clonedmat
#ifdef GPU
					      , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		    ) ;
    
    void setMetadata( const int64_t& timensec, const float64_t& dvaw, const float64_t& dvah, const float64_t& aspect_wh );
      
    void copyMetadataTo( std::shared_ptr<FeatMapImplInst>& ret ) ;


    
    void resizeTo( std::shared_ptr<FeatMapImplInst>& ret, const cv::Size& newsize, const int itype, const int dtype
#ifdef GPU
		   , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		   ) ;
    void resizeToGpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const cv::Size& newsize, const int itype, const int dtype
#ifdef GPU
		   , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
			  ) ;
    
    
    void resizeToCpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const cv::Size& newsize, const int itype, const int dtype		   );
    
    void resizeTo( std::shared_ptr<FeatMapImplInst>& ret, const float64_t& scale_w, const float64_t& scale_h, const int itype, const int dtype
#ifdef GPU
		   , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		   ) ;


    void resizeToGpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const float64_t& scale_w, const float64_t& scale_h, const int itype, const int dtype
#ifdef GPU
		   , cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
			  ) ;

    
    void resizeToCpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const float64_t& scale_w, const float64_t& scale_h, const int itype, const int dtype  ) ;
    
    cv::Mat copyAsRealCVMat(
#ifdef GPU
			    cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
			    );
    
  };
}
