#pragma once



#define CONSOLIDATE_FILTERS
#define ALLOW_NONEXISTENT_MAP_INPUTS


#define GAUSS_USE_KERNEL

#define USE_REAL_TIMEDELTA

#define MUXLOCK_ALL_MAPIMPL



#define TRUE  (1==1)
#define FALSE (!TRUE)

#define MULTITHREAD TRUE 
#define DEBUGLEVEL 0



#define HELPLEVEL 6
#define TIMINGLEVEL 0

#define SEC_TO_NSEC 1e9

#if DEBUGLEVEL > 5
#define DEBUGPRINTF( arg1, ... )		\
    { fprintf( arg1, __VA_ARGS__ ); }  
  
#else
#define DEBUGPRINTF( arg1, arg2, ... ) 
#endif


#if HELPLEVEL > 0
#define HELPPRINTF( arg1, ... )		\
    { fprintf( arg1, __VA_ARGS__ ); }
#else
#define HELPPRINTF( arg1, ... )
#endif

#define INTERPOL cv::INTER_NEAREST

#ifdef GPU
#define SALMAP_FUNCT( name )  void name( localmapdict& inputmaps, std::shared_ptr<FeatMapImplCollection>& outputcollection, void*& userdata, scratchmats& scratch, const std::map<std::string,std::string>& params, const int64_t& curr_time , const std::string& nickname, const std::string& desc, cv::cuda::Stream& stream  )
#else
#define SALMAP_FUNCT( name )  void name( localmapdict& inputmaps, std::shared_ptr<FeatMapImplCollection>& outputcollection, void*& userdata, scratchmats& scratch, const std::map<std::string,std::string>& params, const int64_t& curr_time , const std::string& nickname, const std::string& desc )
#endif




#define SAL_DO_CAT(A, B) A##B
#define SAL_CAT(A, B) SAL_DO_CAT(A, B)



 

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)



#define REGISTER_SAL_FILT_FUNC(NAME) namespace salmap_rv { namespace filter_functs { salmap_rv::detail::FuncPtrRegistrar \
					 SAL_CAT(SAL_FUNC_, NAME) (&NAME, #NAME); } }
  

  

#define GETINPUT( name ) \
  get_from_dict_or_fail( inputmaps, name )

#define GETOUTPUT( name ) \
  get_from_collection_or_fail( outputcollection, name, curr_time )


#define INPUT_ALG_DUMMY_NAME "INPUT"

#define STR_FLOAT_PRECISION 10
