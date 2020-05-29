#include <salmap_rv/include/featmapimplinst.hpp>





using namespace salmap_rv;

const bool salmap_rv::FeatMapImplInst::isDummy() const
{
  if( true == oncpu )
    {
      return cpu_data.empty();
    }
  else if( true == ongpu )
    {
      return gpu_data.empty();
    }
  else 
    {
      return true;
    }
}

void salmap_rv::FeatMapImplInst::instantiateIfVoid( std::shared_ptr<FeatMapImplInst>& p )
{
  if( !p )
    {
      p = std::make_shared<FeatMapImplInst>();
    }
}

void salmap_rv::FeatMapImplInst::setDummy()
{
  oncpu = false;
  ongpu = false;
}


void salmap_rv::FeatMapImplInst::setHasGpu( const bool val )
{
  ongpu = val;
}

void salmap_rv::FeatMapImplInst::setHasCpu( const bool val)
{
  oncpu = val;
}


const bool salmap_rv::FeatMapImplInst::hascpu() const
{
  if( true == oncpu )
    {
      return true;
    }
  return false;
}

const bool salmap_rv::FeatMapImplInst::hasgpu() const
{
  if( true == ongpu )
    {
      return true;
    }
  return false;
}

const cv::Size salmap_rv::FeatMapImplInst::size() const
{
  
  
  
  
  
  
  if( true == oncpu )
    {
      return cpu_data.size();
    }
  else if( true == ongpu )
    {
      return gpu_data.size();
    }
  else
    {
      return cv::Size(0,0);
    }
}




const double salmap_rv::FeatMapImplInst::get_dva_wid() const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting DVA WIDTH from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  return dva_wid;
}

const double salmap_rv::FeatMapImplInst::get_dva_hei( ) const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting DVA HEIGHT from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  return dva_hei;
}


void salmap_rv::FeatMapImplInst::set_dva_wid( const double& d )
{
  dva_wid = d;
}

void salmap_rv::FeatMapImplInst::set_dva_hei( const double& d )
{
  dva_hei = d;
}



const double salmap_rv::FeatMapImplInst::get_dva_per_pix_wid() const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting DVA/PIX WIDTH from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  auto mysize = size().width;
  if( mysize > 0 )
    {
      return dva_wid / mysize;
    }
  else
    {
      return -1;
    }
}


const double salmap_rv::FeatMapImplInst::get_dva_per_pix_hei() const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting DVA/PIX HEIGHT from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  auto mysize = size().height;
  if( mysize > 0 )
    {
      return dva_hei / mysize;
    }
  else
    {
      return -1;
    }
}



const double salmap_rv::FeatMapImplInst::get_pix_per_dva_wid() const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting PIX/DVA WIDTH from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  auto mysize = size().width;
  if( mysize > 0 )
    {
      return mysize / dva_wid;
    }
  else
    {
      return -1;
    }
}


const double salmap_rv::FeatMapImplInst::get_pix_per_dva_hei() const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting PIX/DVA HEIGHT from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  auto mysize = size().height;
  if( mysize > 0 )
    {
      return mysize / dva_hei;
    }
  else
    {
      return -1;
    }
}





const double salmap_rv::FeatMapImplInst::get_origaspectratio_wh() const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting Aspect from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  return origaspectratio_wh;
}

void salmap_rv::FeatMapImplInst::set_origaspectratio_wh( const double& d )
{
  origaspectratio_wh = d;
}


void salmap_rv::FeatMapImplInst::set_time( const int64_t& t )
{
  time_nsec = t;
}

const int64_t salmap_rv::FeatMapImplInst::get_time( ) const
{
  if( false == initialized )
    {
      fprintf(stderr, "Getting time from uninitialized FeatMapImplInst\n");
      exit(1);
    }
  return time_nsec;
}


const bool salmap_rv::FeatMapImplInst::empty() const
{
  if( false == ongpu && false == oncpu )
    {
      return true;
    }
  else
    {
      return false;
    }
}

void salmap_rv::FeatMapImplInst::cpu_to_gpu(
#ifdef GPU
		cv::cuda::Stream& stream
#endif
		)
{
  if( oncpu && ongpu )
    {
      
    }
  else if( ongpu && !oncpu)
    {
      
    }
  else if( oncpu && !ongpu) 
    {
#ifdef GPU
      
      gpu_data.upload( cpu_data, stream );
      stream.waitForCompletion();
      ongpu = true;
#endif
    }
  else 
    {
      
    }
}

void salmap_rv::FeatMapImplInst::gpu_to_cpu(
#ifdef GPU
		cv::cuda::Stream& stream
#endif
		)
{
  if( oncpu && ongpu )
    {
      
    }
  else if( ongpu && !oncpu)
    {
#ifdef GPU
      
      gpu_data.download( cpu_data, stream );
      stream.waitForCompletion();
      oncpu = true;
#endif
    }
  else if( oncpu && !ongpu) 
    {
      
    }
  else 
    {
      
    }
}


salmap_rv::FiltMat& salmap_rv::FeatMapImplInst::cpu_w(
#ifdef GPU
					   cv::cuda::Stream& stream
#endif
					   )
{
  
  gpu_to_cpu(
#ifdef GPU
	     stream
#endif
	     );
  
  
  ongpu = false;
  oncpu = true;
  return cpu_data; 
}











const salmap_rv::FiltMat& salmap_rv::FeatMapImplInst::cpu(
#ifdef GPU
					       cv::cuda::Stream& stream
#endif
					       ) 
{
  
  gpu_to_cpu(
#ifdef GPU
	     stream
#endif
	     );

  
  return cpu_data; 
}






cv::cuda::GpuMat& salmap_rv::FeatMapImplInst::gpu_w(
#ifdef GPU
						  cv::cuda::Stream& stream
#endif
						  )
{
  
  cpu_to_gpu(
#ifdef GPU
	     stream
#endif
	     );

  
  
  oncpu = false;
  ongpu = true;
  return gpu_data; 
}

const cv::cuda::GpuMat salmap_rv::FeatMapImplInst::gpu(
#ifdef GPU
					       cv::cuda::Stream& stream
#endif
					       )
{
  
  cpu_to_gpu(
#ifdef GPU
	     stream
#endif
	     );
  
  
  return gpu_data; 
}



salmap_rv::FeatMapImplInst::FeatMapImplInst()
{
  initialized=false;
  ongpu=false;
  oncpu=false;
  time_nsec = -1;
  dva_wid = -1;
  dva_hei = -1;
  origaspectratio_wh=-1;

  
  
  
}

 



salmap_rv::FeatMapImplInst::FeatMapImplInst( const int64_t& timensec, const float64_t& dvaw, const float64_t& dvah, const float64_t& aspect_wh  )
{
  initialized=true;
  ongpu=false;
  oncpu=false;
  time_nsec = timensec;
  dva_wid = dvaw;
  dva_hei = dvah;
  origaspectratio_wh= aspect_wh;

  
  
  
}

 

 




void salmap_rv::FeatMapImplInst::copyFromCpu( cv::InputArray from )
{
  from.copyTo( cpu_w() );
}

void salmap_rv::FeatMapImplInst::copyFromGpu( cv::InputArray from
#ifdef GPU
					 , cv::cuda::Stream& stream
#endif
					 )
{
  from.copyTo( gpu_w()
#ifdef GPU
	       ,  stream
#endif
	       );
}



void salmap_rv::FeatMapImplInst::copyTo( std::shared_ptr<FeatMapImplInst>& myclone
#ifdef GPU
					 , cv::cuda::Stream& stream
#endif
					 ) 
{
  instantiateIfVoid( myclone );
  
  myclone->ongpu = ongpu;
  myclone->oncpu = oncpu;
  myclone->time_nsec = time_nsec;
  myclone->dva_wid = dva_wid;
  myclone->dva_hei = dva_hei;
  myclone->origaspectratio_wh = origaspectratio_wh;
  myclone->initialized = initialized;

#ifdef GPU
  gpu_data.copyTo( myclone->gpu_data, stream );
  stream.waitForCompletion();
#else
  gpu_data.copyTo( myclone->gpu_data ); 
#endif
  
  cpu_data.copyTo(myclone->cpu_data);
}



void salmap_rv::FeatMapImplInst::copyCpuTo( std::shared_ptr<FeatMapImplInst>& myclone
#ifdef GPU
					 , cv::cuda::Stream& stream
#endif
					 ) 
{
  instantiateIfVoid( myclone );
  
  myclone->ongpu = false;
  myclone->oncpu = oncpu;
  myclone->time_nsec = time_nsec;
  myclone->dva_wid = dva_wid;
  myclone->dva_hei = dva_hei;
  myclone->origaspectratio_wh = origaspectratio_wh;
  myclone->initialized = initialized;

  cpu_data.copyTo(myclone->cpu_data);
}



void salmap_rv::FeatMapImplInst::copyGpuTo( std::shared_ptr<FeatMapImplInst>& myclone
#ifdef GPU
					 , cv::cuda::Stream& stream
#endif
					 ) 
{
  instantiateIfVoid( myclone );
  
  myclone->ongpu = ongpu;
  myclone->oncpu = false;
  myclone->time_nsec = time_nsec;
  myclone->dva_wid = dva_wid;
  myclone->dva_hei = dva_hei;
  myclone->origaspectratio_wh = origaspectratio_wh;
  myclone->initialized = initialized;

#ifdef GPU
  gpu_data.copyTo( myclone->gpu_data, stream );
  stream.waitForCompletion();
#else
  gpu_data.copyTo( myclone->gpu_data ); 
#endif
  
}



void salmap_rv::FeatMapImplInst::copyCpuTo( salmap_rv::FiltMat& clonedmat )
{
  cpu().copyTo(clonedmat);
}

#ifdef USE_OPENCL
void salmap_rv::FeatMapImplInst::copyCpuTo( cv::Mat& clonedmat )
{
  cpu().copyTo(clonedmat);
}
#endif 

void salmap_rv::FeatMapImplInst::copyGpuTo( salmap_rv::FiltMat& clonedmat
#ifdef GPU
					 , cv::cuda::Stream& stream
#endif
					 ) 
{
  gpu().copyTo( clonedmat
#ifdef GPU
		, stream
#endif
		);
#ifdef GPU
    stream.waitForCompletion();
#endif    
}

void FeatMapImplInst::setMetadata( const int64_t& _timensec, const float64_t& dvaw, const float64_t& dvah, const float64_t& aspect_wh )
{
  initialized=true;

  set_time( _timensec );
  set_dva_wid( dvaw );
  set_dva_hei( dvah );
  set_origaspectratio_wh( aspect_wh );
}

void FeatMapImplInst::copyMetadataTo( std::shared_ptr<FeatMapImplInst>& ret )
{
  instantiateIfVoid( ret );
  
  ret->setMetadata( get_time(), get_dva_wid(), get_dva_hei(), get_origaspectratio_wh() );
		   
  
  
  
  
  
  
}








void FeatMapImplInst::resizeTo( std::shared_ptr<FeatMapImplInst>& ret, const cv::Size& newsize, const int itype, const int dtype
#ifdef GPU
				, cv::cuda::Stream& stream
#endif
		   ) 
    {
      instantiateIfVoid( ret );
      
      if( false == initialized )
	{
	  fprintf(stderr, "resizeTo -- not initialized FeatMapImplInst\n");
	  exit(1);
	}
	
      
      if( size().width == 0 || size().height == 0 )
	{
	  fprintf(stderr, "REV: error in resize, width or height is zero, probably due to more coarse levels being smaller than 1 pixel\n");
	  exit(1);
	}
      
      int interpol;
      if( newsize.width > size().width )
	{
	  interpol = itype;
	}
      else
	{
	  interpol = dtype;
	}

      copyMetadataTo( ret );
      
      if( true == hascpu() )
	{
	  cv::resize( cpu(), ret->cpu_w(), newsize, 0, 0, interpol );
	}
      if( true == hasgpu() )
	{
	  cv::resize( gpu(), ret->gpu_w(), newsize, 0, 0, interpol
#if GPU
		      , stream
#endif
		      );
	}

      
      ret->setHasCpu( hascpu() );
      ret->setHasGpu( hasgpu() );
      
    }


void FeatMapImplInst::resizeToGpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const cv::Size& newsize, const int itype, const int dtype
#ifdef GPU
				       , cv::cuda::Stream& stream
#endif
		   ) 
    {
      instantiateIfVoid( ret );
      if( false == initialized )
	{
	  fprintf(stderr, "resizeTo -- not initialized FeatMapImplInst\n");
	  exit(1);
	}
      if( size().width == 0 || size().height == 0 )
	{
	  fprintf(stderr, "REV: error in resize, width or height is zero, probably due to more coarse levels being smaller than 1 pixel\n");
	  exit(1);
	}
      
      int interpol;
      if( newsize.width > size().width )
	{
	  interpol = itype;
	}
      else
	{
	  interpol = dtype;
	}

      copyMetadataTo( ret );
      
      
      if( true == hasgpu() )
	{
	  cv::resize( gpu(), ret->gpu_w(), newsize, 0, 0, interpol
#if GPU
		      , stream
#endif
		      );
	}
      
    }


void FeatMapImplInst::resizeToCpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const cv::Size& newsize, const int itype, const int dtype	 ) 
    {
      instantiateIfVoid( ret );
      if( false == initialized )
	{
	  fprintf(stderr, "resizeTo -- not initialized FeatMapImplInst\n");
	  exit(1);
	}
      
      if( size().width == 0 || size().height == 0 )
	{
	  fprintf(stderr, "REV: error in resize, width or height is zero, probably due to more coarse levels being smaller than 1 pixel\n");
	  exit(1);
	}
      
      int interpol;
      if( newsize.width > size().width )
	{
	  interpol = itype;
	}
      else
	{
	  interpol = dtype;
	}

      copyMetadataTo( ret );
      
      if( true == hascpu() )
	{
	  cv::resize( cpu(), ret->cpu_w(), newsize, 0, 0, interpol );
	}
      
    }





void FeatMapImplInst::resizeTo( std::shared_ptr<FeatMapImplInst>& ret, const float64_t& scale_w, const float64_t& scale_h, const int itype, const int dtype
#ifdef GPU
				, cv::cuda::Stream& stream
#endif
		   ) 
    {
      instantiateIfVoid( ret );
      float64_t realscale_h = (scale_h <= 0) ? scale_w : scale_h;
      float64_t realscale_w = (scale_w <= 0) ? scale_h : scale_w;

      if( false == initialized )
	{
	  fprintf(stderr, "resizeTo -- not initialized FeatMapImplInst\n");
	  exit(1);
	}
      if( realscale_h <= 0 && realscale_w <= 0 )
	{
	  fprintf(stderr, "resizeTo (with scale), scale_h and scale_w both <= 0.\n");
	  exit(1);
	}
      
      if( (realscale_w < 1 && realscale_h > 1) ||
	  (realscale_w > 1 && realscale_h < 1) )
	{
	  fprintf(stderr, "resizeTo (with scale), scale_h and scale_w are opposite sides of 1...don't know whether to integrate or decimate...\n");
	  exit(1);
	}

      
      int interpol;
      cv::Size newsize( size().width*realscale_w, size().height*realscale_h );
      if( newsize.width > size().width ||
	  newsize.height > size().height )
	{
	  interpol = itype;
	}
      else
	{
	  interpol = dtype;
	}

      copyMetadataTo( ret );
      
      if( true == hascpu() )
	{
	  cv::resize( cpu(), ret->cpu_w(), newsize, 0, 0, interpol );
	}
      if( true == hasgpu() )
	{
	  cv::resize( gpu(), ret->gpu_w(), newsize, 0, 0, interpol
#if GPU
		      , stream
#endif
		      );
	}

      
      ret->setHasCpu( hascpu() );
      ret->setHasGpu( hasgpu() );
      
    }
    

     void FeatMapImplInst::resizeToGpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const float64_t& scale_w, const float64_t& scale_h, const int itype, const int dtype
#ifdef GPU
					    , cv::cuda::Stream& stream
#endif
		   ) 
    {
      instantiateIfVoid( ret );
      float64_t realscale_h = (scale_h <= 0) ? scale_w : scale_h;
      float64_t realscale_w = (scale_w <= 0) ? scale_h : scale_w;

      if( false == initialized )
	{
	  fprintf(stderr, "resizeTo -- not initialized FeatMapImplInst\n");
	  exit(1);
	}
      if( realscale_h <= 0 && realscale_w <= 0 )
	{
	  fprintf(stderr, "resizeTo (with scale), scale_h and scale_w both <= 0.\n");
	  exit(1);
	}
      
      if( (realscale_w < 1 && realscale_h > 1) ||
	  (realscale_w > 1 && realscale_h < 1) )
	{
	  fprintf(stderr, "resizeTo (with scale), scale_h and scale_w are opposite sides of 1...don't know whether to integrate or decimate...\n");
	  exit(1);
	}

      
      int interpol;
      cv::Size newsize( size().width*realscale_w, size().height*realscale_h );
      if( newsize.width > size().width ||
	  newsize.height > size().height )
	{
	  interpol = itype;
	}
      else
	{
	  interpol = dtype;
	}

      copyMetadataTo( ret );
      if( true == hasgpu() )
	{
	  cv::resize( gpu(), ret->gpu_w(), newsize, 0, 0, interpol
#if GPU
		      , stream
#endif
		      );
	}
    }

     void FeatMapImplInst::resizeToCpuOnly( std::shared_ptr<FeatMapImplInst>& ret, const float64_t& scale_w, const float64_t& scale_h, const int itype, const int dtype  ) 
    {
      instantiateIfVoid( ret );
      float64_t realscale_h = (scale_h <= 0) ? scale_w : scale_h;
      float64_t realscale_w = (scale_w <= 0) ? scale_h : scale_w;

      if( false == initialized )
	{
	  fprintf(stderr, "resizeTo -- not initialized FeatMapImplInst\n");
	  exit(1);
	}
      if( realscale_h <= 0 && realscale_w <= 0 )
	{
	  fprintf(stderr, "resizeTo (with scale), scale_h and scale_w both <= 0.\n");
	  exit(1);
	}
      
      if( (realscale_w < 1 && realscale_h > 1) ||
	  (realscale_w > 1 && realscale_h < 1) )
	{
	  fprintf(stderr, "resizeTo (with scale), scale_h and scale_w are opposite sides of 1...don't know whether to integrate or decimate...\n");
	  exit(1);
	}

      
      int interpol;
      cv::Size newsize( size().width*realscale_w, size().height*realscale_h );
      if( newsize.width > size().width ||
	  newsize.height > size().height )
	{
	  interpol = itype;
	}
      else
	{
	  interpol = dtype;
	}

      copyMetadataTo( ret );
      if( true == hascpu() )
	{
	  cv::resize( cpu(), ret->cpu_w(), newsize, 0, 0, interpol );
	}
    }

cv::Mat salmap_rv::FeatMapImplInst::copyAsRealCVMat(
#ifdef GPU
			cv::cuda::Stream& stream
#endif
			)
{
  cv::Mat newmat;
  copyCpuTo( newmat
#ifdef GPU
	     , stream
#endif
		);
  return newmat;
}














































