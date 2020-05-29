#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/scratchmats.hpp>


namespace salmap_rv
{
  

  
  struct algorithmImpl
  {
    std::string name;
    std::map<std::string,std::string> params;
  
    void* userdata;
    algoFunct_t userinit;
    algoFunct_t userupdate;
    algoFunct_t userdestroy;

    scratchmats myscratch;
  
    bool calledinit;
  
    bool initset;
    bool destroyset;
    bool updateset;
  
    bool setparams;

    std::string nickname;
    std::string desc;
  
    size_t estimate_size_in_memory();
  
    void setParams( std::map<std::string, std::string>& p );
  
    void init( localmapdict& inputmaps, const uint64_t& curr_time
#ifdef GPU
	       , cv::cuda::Stream& stream
#endif
	       );

    void update( localmapdict& inputmaps, std::shared_ptr<FeatMapImplCollection>& outputcollection, const uint64_t& curr_time, const std::string& argname
#ifdef GPU
		 , cv::cuda::Stream& stream
#endif
		 );

    void destroy(
#ifdef GPU
		 cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
		 );


    void setInit( const algoFunct_t f );

    void setUpdate( const algoFunct_t f );

    void setDestroy( const algoFunct_t f );

    algorithmImpl();
  
    algorithmImpl( const std::string& n );

  
    algorithmImpl( const std::string& n, const std::string& nick, const std::string& prettydesc );

    ~algorithmImpl();
  

  }; 

  
      
  SALMAP_FUNCT( defaultInit );

  SALMAP_FUNCT( defaultDestroy );
  
  SALMAP_FUNCT( defaultUpdate );

  
} 
