#include <salmap_rv/include/algorithmimpl.hpp>

#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/Timer.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>


using namespace salmap_rv;

size_t salmap_rv::algorithmImpl::estimate_size_in_memory()
  {
    size_t namesize = name.size();
    size_t paramssize = size_in_mem_strstrmap( params );
    return (namesize + paramssize);
  }
  
  void salmap_rv::algorithmImpl::setParams( std::map<std::string, std::string>& p )
  {
    params = p;
    setparams = true;
  }
  
  void salmap_rv::algorithmImpl::init( localmapdict& inputmaps, const uint64_t& curr_time
	     #ifdef GPU
	     , cv::cuda::Stream& stream
	     #endif
	     )
  {
    if( false == setparams )
      {
	fprintf(stderr, "Calling init, but params not set?! [%s]\n", name.c_str() );
	std::string p = pretty_params( params, 0 );
	DEBUGPRINTF(stdout, "%s\n", p.c_str() );
	
	exit(1);
      }
    if( true == initset )
      {
	std::shared_ptr<FeatMapImplCollection> tmpmap;
	userinit( inputmaps, tmpmap, userdata, myscratch, params, curr_time, nickname, desc
#ifdef GPU
		  , stream
#endif
		  );
      }
    calledinit = true;
  }

void salmap_rv::algorithmImpl::update( localmapdict& inputmaps, std::shared_ptr<FeatMapImplCollection>& outputcollection, const uint64_t& curr_time, const std::string& argname
	       #ifdef GPU
	       , cv::cuda::Stream& stream
	       #endif
	       )
  {
    Timer t;
    
    if( false == calledinit )
      {
	init( inputmaps, 0
	      #ifdef GPU
	      , stream
	      #endif
	      );
      }
    
    if( true == updateset )
      {
	
	
	userupdate( inputmaps, outputcollection, userdata, myscratch, params, curr_time, argname, desc
#ifdef GPU
		    , stream
#endif
		    );
      }

  }


void salmap_rv::algorithmImpl::destroy(
#ifdef GPU
	       cv::cuda::Stream& stream=cv::cuda::Stream::Null()
#endif
	       )
  {
    if( true == destroyset )
      {
	localmapdict tmpmaps1;
	std::shared_ptr<FeatMapImplCollection> tmpmaps2;
	userdestroy( tmpmaps1, tmpmaps2, userdata, myscratch, params, 0, nickname, desc
#ifdef GPU
		     , stream
#endif
		     ); 
      }
  }


  void salmap_rv::algorithmImpl::setInit( const algoFunct_t f )
  {
    initset = true;
    userinit = f;
  }

  void salmap_rv::algorithmImpl::setUpdate( const algoFunct_t f )
  {
    updateset = true;
    userupdate = f;
  }

  void salmap_rv::algorithmImpl::setDestroy( const algoFunct_t f )
  {
    destroyset = true;
    userdestroy = f;
  }

  salmap_rv::algorithmImpl::algorithmImpl()
  {
    userdata = NULL;
    name = "__UNINIT_ALGO";
    updateset = false;
    initset = false;
    destroyset = false;
    calledinit = false;
    nickname="DEFAULT_ALGO_NICK";
    desc="DEFAULT_ALGO_DESC";
  }
  
  salmap_rv::algorithmImpl::algorithmImpl( const std::string& n )
  {
    userdata = NULL;
    name = n;
    updateset = false;
    initset = false;
    destroyset = false;
    calledinit = false;
    nickname="DEFAULT_ALGO_NICK";
    desc="DEFAULT_ALGO_DESC";
  }

  
  salmap_rv::algorithmImpl::algorithmImpl( const std::string& n, const std::string& nick, const std::string& prettydesc )
  {
    userdata = NULL;
    name = n;
    updateset = false;
    initset = false;
    destroyset = false;
    calledinit = false;
    nickname=nick;
    desc=prettydesc;
    }

  salmap_rv::algorithmImpl::~algorithmImpl()
  {
    
    destroy();

    
  }

SALMAP_FUNCT( salmap_rv::defaultInit ) {}

SALMAP_FUNCT( salmap_rv::defaultDestroy ) {}

SALMAP_FUNCT( salmap_rv::defaultUpdate ) {}
