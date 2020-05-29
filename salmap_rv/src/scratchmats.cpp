
#include <salmap_rv/include/scratchmats.hpp>

#include <salmap_rv/include/featmapimplinst.hpp>

using namespace salmap_rv;

#ifdef GPU
cv::Ptr<cv::cuda::Filter>& scratchmats::gpufilter( const std::string& s )
{
  auto iter = gpufilters.find( s );
  if( iter == gpufilters.end() )
    {
      gpufilters[s] = cv::Ptr<cv::cuda::Filter>();
      return gpufilters[s];
    }
  else
    {
      return iter->second;
    }
}
#endif

scratchmats::scratchmats() : isinit(false)
{  }

void scratchmats::reset()
{
  isinit = false;
  maps.clear();
  float64s.clear();
  int64s.clear();
#ifdef GPU
  gpufilters.clear();
#endif
}

bool scratchmats::initialized()
{
  if( true == isinit )
      {
	return true;
      }
  else 
    {
      
      isinit = true;
      return false;
    }
}

float64_t& scratchmats::float64( const std::string& s )
{
  auto iter = float64s.find( s );
  if( iter == float64s.end() )
    {
      float64s[s] = 0;
      return float64s[s];
    }
  else
    {
      return iter->second;
    }
}

cv::Size& scratchmats::size( const std::string& s )
{
  auto iter = sizes.find( s );
  if( iter == sizes.end() )
    {
      sizes[s] = cv::Size();
      return sizes[s];
    }
  else
    {
      return iter->second;
    }
}

int64_t& scratchmats::int64( const std::string& s )
{
  auto iter = int64s.find( s );
  if( iter == int64s.end() )
    {
      int64s[s] = 0;
      return int64s[s];
    }
  else
    {
      return iter->second;
    }
}

std::vector<int64_t>& scratchmats::int64v( const std::string& s )
{
  auto iter = int64vs.find( s );
  if( iter == int64vs.end() )
    {
      int64vs[s] = std::vector<int64_t>();
      return int64vs[s];
    }
  else
    {
      return iter->second;
    }
}

std::vector<float64_t>& scratchmats::float64v( const std::string& s )
{
  auto iter = float64vs.find( s );
  if( iter == float64vs.end() )
    {
      float64vs[s] = std::vector<float64_t>();
      return float64vs[s];
    }
  else
    {
      return iter->second;
    }
}

std::vector<cv::Size>& scratchmats::sizev( const std::string& s )
{
  auto iter = sizevs.find( s );
  if( iter == sizevs.end() )
    {
      sizevs[s] = std::vector<cv::Size>();
      return sizevs[s];
    }
  else
    {
      return iter->second;
    }
}

const std::shared_ptr<FeatMapImplInst>& scratchmats::get( const std::string& s )
{
  auto iter = maps.find( s );
  
  if( iter == maps.end() )
    {
      fprintf(stderr, "REV: ERROR: scratchmats::get() [%s] doesn't exist\n", s.c_str());
      exit(1);
    }
  
  if( nullptr == iter->second )
    {
      fprintf(stderr, "REV: wtf scratchmats get(%s) -- nullptr?!\n", s.c_str() );
      exit(1);
    }
  return iter->second;
}


std::shared_ptr<FeatMapImplInst>& scratchmats::get_w( const std::string& s )
{
  auto iter = maps.find( s );
  
  if( iter == maps.end() )
    {
      maps[s] = std::make_shared<FeatMapImplInst>( );
    }

  iter = maps.find( s );
  if( maps.end() == iter || nullptr == iter->second )
    {
      fprintf(stderr, "REV: wtf scratchmats get(%s) -- nullptr (or iter not exist)?!\n", s.c_str() );
      exit(1);
    }
  return iter->second;
}


const salmap_rv::FiltMat& scratchmats::cpu( const std::string& s )
{
  return get(s)->cpu();
}

const cv::cuda::GpuMat& scratchmats::gpu( const std::string& s )
{
  return get(s)->gpu();
}

salmap_rv::FiltMat& scratchmats::cpu_w( const std::string& s )
{
  return get_w(s)->cpu_w();
}

cv::cuda::GpuMat& scratchmats::gpu_w( const std::string& s )
{
  return get_w(s)->gpu_w();
}
