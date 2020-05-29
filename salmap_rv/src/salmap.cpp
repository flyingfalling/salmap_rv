#include <salmap_rv/include/salmap.hpp>

#include <salmap_rv/include/ThreadPool.hpp>
#include <salmap_rv/include/Timer.hpp>

#include <salmap_rv/include/taggraph.hpp>
#include <salmap_rv/include/util_functs.hpp>


#include <salmap_rv/include/featfilterimpl.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>

#include <salmap_rv/include/funcptrregistrar.hpp>

#include <salmap_rv/include/taggraph.hpp>
#include <salmap_rv/include/param_set.hpp>


using namespace salmap_rv;


void salmap_rv::SalMap::advance_time()
{
  time_nsec += get_dt_nsec();
  time_idx  ++;
}

void salmap_rv::SalMap::reset()
{
  mapimpls.reset();
}

std::vector<std::string> salmap_rv::SalMap::get_outputs_of_filter( FeatFilter& f )
{
  std::vector<std::string> op = mapimpls->list_outputs( f.toString() );
  return op;
}

std::vector<std::string> salmap_rv::SalMap::list_available_filters()
{
  std::vector<std::string> res;
  for( auto iter=filtset.filters.begin(); iter != filtset.filters.end(); ++iter )
    {
      res.push_back( iter->first );
    }
  return res;
}
  
std::vector<std::string> salmap_rv::SalMap::list_all_available_filters()
{
  std::vector<std::string> res;

  for( auto iter=outputs.begin(); iter != outputs.end(); ++iter )
    {
      res.push_back( *iter );
    }
    
  for( auto iter=filtset.filters.begin(); iter != filtset.filters.end(); ++iter )
    {
      
      if( std::find( res.begin(), res.end(), iter->first ) == res.end() )
	{
	  res.push_back( iter->first );
	}
    }
    
  return res;
}

 void salmap_rv::SalMap::fill_map_accessors( )
  {
    all_map_accessors.clear();
    all_map_descs.clear();
    
    
    for( auto iter=filtset.filters.begin(); iter != filtset.filters.end(); ++iter )
      {
	std::string filtname = iter->first;
	FeatFilter filterinst = iter->second;
	std::vector<std::string> op = get_outputs_of_filter( filterinst );
	for( auto& o : op )
	  {
	    std::string outfiltname = o + "@" + filtname;
	    auto i = all_map_accessors.find( outfiltname );
	    if( i == all_map_accessors.end() )
	      {
		
		all_map_accessors[outfiltname] = getMapAccessor( o, filtname );
		all_map_descs[outfiltname] = o + " @ " + iter->second.desc;
	      }
	    else
	      {
		fprintf(stderr, "REV: error double for [%s] in fill_map_accessors\n", outfiltname.c_str() );
		exit(1);
	      }
	  }
      }
    
  }



 std::map<std::string, cv::Mat> salmap_rv::SalMap::return_all_maps_resized( const size_t w, const size_t h )
  {
    fill_map_accessors();
    
    std::map<std::string, cv::Mat> ret;
    const int interp = cv::INTER_LINEAR;
    
    int64_t sampletime_nsec = get_most_recent_update_time_nsec();
    
    std::string bufstr = print_time( sampletime_nsec, 15 );
    
    
    
    std::shared_ptr<FeatMapImplInst> fm;
    
    for( auto it=all_map_accessors.begin(); it != all_map_accessors.end(); ++it )
      {
	bool got = mapFromAccessor( it->second, fm, sampletime_nsec );
	
	if( true == got )
	  {
	    std::string fname = it->first + "@" + bufstr;
	    if( false == fm->empty() )
	      {
		
		
		
		ret[fname] = salmap_rv::resize( fm->cpu(), cv::Size(w,h), interp ); 
	      }
	    else
	      {
		fprintf(stderr, "WARNING, [%s] is empty MAT\n", fname.c_str() );
	      }
	  }
      }
    
    return ret;
  }


void salmap_rv::SalMap::list_filters_and_descs( std::vector<std::string>& fs, std::vector<std::string>& descs )
  {
    fill_map_accessors(); 

    fs.clear();
    descs.clear();
    
    
    for( auto iter=outputs.begin(); iter != outputs.end(); ++iter )
      {
	fprintf(stdout, "Looking for output: [%s]\n", (*iter).c_str());
	auto it = filtset.filters.find( *iter );
	if( it == filtset.filters.end() )
	  {
	    fprintf(stderr, "REV: list_filters_and_descs...output nickname [%s] does not have a corresp filter!\n", (*iter).c_str() );
	    for( auto& a : filtset.filters )
	      {
		fprintf(stdout, "%s\n", a.first.c_str() );
	      }
	    exit(1);
	  }
	else
	  {
 	    fs.push_back( "output@" + *iter );
	    
	    descs.push_back( "Output @ " + it->second.getdesc( ) );
	  }
      }
    
    fprintf(stdout, "Map accessors size: %ld\n", all_map_accessors.size() );
    
    for( auto iter=all_map_accessors.begin(); iter != all_map_accessors.end(); ++iter )
      {
	
	if( std::find( fs.begin(), fs.end(), iter->first ) == fs.end() )
	  {
	    fs.push_back( iter->first );
	    descs.push_back( all_map_descs[iter->first] ); 
	  }
      }
  }


FeatMapImplPtr salmap_rv::SalMap::getMapAccessor( const std::string& localname, const std::string& filttag )
  {
    
    auto it = filtset.filters.find( filttag );
    if( it == filtset.filters.end() )
      {
	fprintf(stderr, "REV: get map accessor failed...filter [%s] doesn't exist in filts...\n", filttag.c_str() );
	exit(1);
      }
    
    FeatMapImplPtr fp( localname, map_from_filter_abstime( it->second, localname, 0 ), filtimpls, filtset, mapimpls );
    
    return fp;
  }





bool salmap_rv::SalMap::mapFromAccessor( FeatMapImplPtr& accessor, std::shared_ptr<FeatMapImplInst>& ret, const int64_t& time )
  {
    accessor.set_abs_curr_time( time );
    return accessor.getTo( ret, time );
  }


void salmap_rv::SalMap::register_algorithms()
  {
    if( true == registered_algos )
      {
	return;
      }
    DEBUGPRINTF(stdout, "REV: Auto-registering all filter update algorithms\n");
    auto const& fpm = detail::FuncPtrMap::getMap();
    for( auto it=fpm.begin(); it != fpm.end(); ++it )
      {
	DEBUGPRINTF( stdout, "Auto-registering algorithm: [%s]\n", it->first.c_str() );
	register_algorithm( it->first, it->second ); 
      }
    DEBUGPRINTF(stdout, "Finished registering [%ld] algos.\n", fpm.size() );
    registered_algos = true;
  }






cv::Mat salmap_rv::SalMap::get_map_now_nickname( const std::string& filtername, const std::string& outmapname, const cv::Size& desiredsize )
{
  cv::Mat ret;
  FeatMapImplPtr fp = getMapAccessor( outmapname, filtername );
  std::shared_ptr<FeatMapImplInst> fi;
  int64_t targtime_nsec = get_most_recent_update_time_nsec();
  bool got = mapFromAccessor( fp, fi, targtime_nsec );
  if( true == got )
    {
      
      ret = salmap_rv::resize( fi->cpu(), desiredsize, cv::INTER_NEAREST );
      
      return ret;
    }
  else
    {
      fprintf(stdout, "Filter map [%s]@[%s] @ time nsec [%ld] did not exist (returning empty cv::Mat).\n", outmapname.c_str(), filtername.c_str(), targtime_nsec );
      return ret;
    }
}



void salmap_rv::SalMap::init( const param_set& p, std::shared_ptr<ThreadPool> _tp )
{
    
  
  salmap_rv::filter_func_dummy::DUMMY_FUNC_FOR_LINKER();
    
  filtergraph.clear();
    
  
  filtimpls.clear(); 
  algimpls.clear(); 
  if( mapimpls )
    {
      mapimpls->clear(); 
    }
  else
    {
      mapimpls = std::make_shared<FeatMapImplSet>();
    }
  
  
  filtset.clear(); 
  outputs.clear(); 
  filtergraph.clear(); 
    
  successfully_updated = false;
  input_compiled = false;
  deps_built=false;
  registered_algos=false;
  time_nsec = 0;
  time_idx = 0;
  trial_timesteps = 0; 
  dt_nsec = p.get<int64_t>("dt_nsec");
  input_dva_wid = p.get<float64_t>("input_dva_wid");
  input_dva_hei = p.get<float64_t>("input_dva_hei");

  consolidate_filters = p.get<bool>("consolidate_filters");

  
  
  
    
  register_algorithms();
  if( dt_nsec <= 1e6 )
    {
      fprintf(stderr, "WARNING init salmap, dt is in NANOSECONDS, you have set dt to be [%lf] seconds (triggered at 1/1000 sec i.e. 1 msec )\n", dt_nsec / 1e9 );
      
    }
        
    
  if( _tp == nullptr )
    {
	
      size_t nworker_threads = p.get<int64_t>("nsalthreads");
      fprintf(stdout, "Initializing new thredpool with [%ld] threads\n", nworker_threads);
      tp = std::make_shared<ThreadPool>( nworker_threads ); 
    }
  else
    {
      tp = _tp;
      fprintf(stdout, "Using existing threadpool of size [%ld] threads\n", tp->get_size() );
    }
#ifdef GPU
  streams.clear();
  for(size_t x=0; x<tp->get_size(); ++x)
    {
      cv::cuda::Stream s;
      streams.push_back( s );
    }
  
#endif
  
  
} 

  
  
void salmap_rv::SalMap::compile_orig_input( const std::string& localname)
{
  if( false == input_compiled )
    {
      
      
      
      
      

      
      
      

      
      
      compiled_input = compiled_map( input_dummy_name(localname), filtimpls, filtset );
      input_compiled = true;
    }
    
}
  
void salmap_rv::SalMap::restart()
{
  successfully_updated = false;
  time_nsec = 0;
}

std::string salmap_rv::SalMap::input_dummy_name( const std::string& localname )
{
  FeatFilter f;
  f.set_algorithm( INPUT_ALG_DUMMY_NAME );
  f.nickname = SalMap::INPUT_FILTER_NICKNAME;
  f.desc = "Raw Input";
  FeatMap m;
  m.filter = f;
  m.localname = localname;
  m.set_relative_time( 0 );
  return m.toString();
}
  

void salmap_rv::SalMap::register_algorithm( const std::string& name, const algoFunct_t updatef, const algoFunct_t initf, const algoFunct_t destroyf )
{
  algimpls.register_algorithm( name, updatef, initf, destroyf );
}
  





std::string salmap_rv::SalMap::add_filter( FeatFilter& f )
{
  return filtset.add_filter( f );
  
}


  

std::string salmap_rv::SalMap::add_filter( const std::string& name )
{
  std::string retfiltname;
  bool added = filtset.add_filter_by_string( name, retfiltname );
  return retfiltname;
}

  

void salmap_rv::SalMap::build_dependencies()
{
  if( false == deps_built )
    {
      
      register_algorithm( INPUT_ALG_DUMMY_NAME, salmap_rv::defaultUpdate );

      
      std::string inputnick = add_filter( input_dummy_name("") );
      DEBUGPRINTF(stdout, "Adding dummy input name...[%s] : inputnick is [%s]\n", input_dummy_name("").c_str(), inputnick.c_str());
	
      
      filtset.check_filters( consolidate_filters ); 
      
    
      
      
      
      
      
      for( auto iter = filtset.filters.begin(); iter != filtset.filters.end(); ++iter )
	{
	  auto impliter = filtimpls.addFilter( iter->second, algimpls );
	  iter->second.setImplIter( impliter );
	}
    
      
      deps_built = true;
    
      Timer ft;
      filtergraph.init( filtset.filters );

      int64_t max_tdelta = filtergraph.get_max_tdelta();
      int64_t max_stepdelta = (-1 * max_tdelta) / get_dt_nsec() + 1;
      trial_timesteps = max_stepdelta;
      HELPPRINTF(stdout, "Setting # trial timesteps to [%ld]\n", max_stepdelta );

#if HELPLEVEL > 10
      std::string graphfile = "salgraph.gv";
      HELPPRINTF(stdout, "Printing graph description to file [%s]\n", graphfile.c_str() );
      filtergraph.deps.print_deps();
      auto g = taggraph_from_salmap();
      std::string graphstr = g.graph_to_graphviz();
      std::ofstream of(graphfile);
      of << graphstr;
#endif
      
      DEBUGPRINTF(stdout, "Filter Execute Graph initialization [%lf] msec\n", ft.elapsed()*1e3);
    }
}


void salmap_rv::SalMap::set_direct_realtime(const std::string& localname, const cv::Mat& data, const float64_t realtime_sec)
{
  std::shared_ptr<FeatMapImplInst> retptr;

  auto it = ptroriginputs.find(localname);
  if( ptroriginputs.end() == it )
    {
      fprintf(stderr, "set_copy_realtime, err\n"); exit(1);
    }
  
  
  it->second.setTo( retptr, get_time_nsec() ); 
  
  
  ptroriginputinsts[ localname ].set_time( realtime_sec * SEC_TO_NSEC );
  
  
  ptroriginputinsts[ localname ].copyMetadataTo( retptr );

  DEBUGPRINTF(stdout, "Pointer I will copy to (add_input_direct()): [%p]\n", retptr.get() );
  
#ifdef USE_OPENCL
  fprintf(stderr, "SET_DIRECT -- USE_OPENCL defined, so copying Mat\n");
  retptr->copyFromCpu(data); 
#else
  retptr->cpu_w() = data;
#endif
}


void salmap_rv::SalMap::set_direct(const std::string& localname, const cv::Mat& data)
{
  std::shared_ptr<FeatMapImplInst> retptr;

  auto it = ptroriginputs.find(localname);
  if( ptroriginputs.end() == it )
    {
      fprintf(stderr, "set_copy_realtime, err\n"); exit(1);
    }
  
  
  it->second.setTo( retptr, get_time_nsec() ); 
  
  
  ptroriginputinsts[ localname ].set_time( get_time_nsec() );
  
  
  ptroriginputinsts[ localname ].copyMetadataTo( retptr );

  DEBUGPRINTF(stdout, "Pointer I will copy to (add_input_direct()): [%p]\n", retptr.get() );
  
#ifdef USE_OPENCL
  fprintf(stderr, "SET_DIRECT -- USE_OPENCL defined, so copying Mat\n");
  retptr->copyFromCpu(data); 
#else
  retptr->cpu_w() = data;
#endif
}


void salmap_rv::SalMap::set_copy(const std::string& localname, const cv::Mat& data)
{
  std::shared_ptr<FeatMapImplInst> retptr;

  auto it = ptroriginputs.find(localname);
  if( ptroriginputs.end() == it )
    {
      fprintf(stderr, "set_copy_realtime, err\n"); exit(1);
    }

  
  it->second.setTo( retptr, get_time_nsec() );
  
  ptroriginputinsts[ localname ].set_time( get_time_nsec() );
  
  
  ptroriginputinsts[ localname ].copyMetadataTo( retptr );

  DEBUGPRINTF(stdout, "Pointer I will copy to (add_input_copy()): [%p]\n", retptr.get() );
  retptr->copyFromCpu(data); 
}

void salmap_rv::SalMap::set_copy_realtime(const std::string& localname, const cv::Mat& data, const float64_t realtime_sec )
{
  std::shared_ptr<FeatMapImplInst> retptr;

  auto it = ptroriginputs.find(localname);
  if( ptroriginputs.end() == it )
    {
      fprintf(stderr, "set_copy_realtime, err\n"); exit(1);
    }
  
  
  it->second.setTo( retptr, get_time_nsec() ); 
  
  
  ptroriginputinsts[ localname ].set_time( realtime_sec * SEC_TO_NSEC );
  
  
  ptroriginputinsts[ localname ].copyMetadataTo( retptr );

  DEBUGPRINTF(stdout, "Pointer I will copy to (add_input_copy()): [%p]\n", retptr.get() );
  retptr->copyFromCpu(data); 
}





void salmap_rv::SalMap::setup_input( const std::string& localname, const cv::Mat& data )
{
  
  auto it = ptroriginputs.find( localname );
  DEBUGPRINTF(stdout, "Will try to add input...[%s]?\n", localname.c_str());
    
  
  if( it == ptroriginputs.end() )
    {
      
      
      compile_orig_input(localname);
	
      ptroriginputs[ localname ] = FeatMapImplPtr( localname, compiled_input.m.toString(), filtimpls, filtset, mapimpls ); 
      
      float64_t origaspect_wh = (float64_t)data.size().width/data.size().height;
      if( input_dva_wid <= 0 && input_dva_hei <= 0 )
	{
	  fprintf(stderr, "REV: attempting to add_input (setup_input), but dva of wid %d / hei %d is 0? (note these are salmap class members)\n", input_dva_wid, input_dva_hei );
	}
      else if( input_dva_hei <= 0 )
	{
	  
	  float64_t ratio_hw = (float64_t)data.size().height / data.size().width;
	  input_dva_hei = input_dva_wid * ratio_hw;
	  fprintf(stdout, "First input of [%s]: modifying input_dva_hei to [%lf] based on h/w pixel ratio of [%lf]\n", localname.c_str(), input_dva_hei, ratio_hw );
	}
      else if( input_dva_wid <= 0 )
	{
	  float64_t ratio_wh = (float64_t)data.size().width / data.size().height;
	  input_dva_wid = input_dva_hei * ratio_wh;
	  fprintf(stdout, "First input of [%s]: modifying input_dva_wid to [%lf] based on w/ pixel ratio of [%lf]\n", localname.c_str(), input_dva_wid, ratio_wh );
	}
      else 
	{
	  float64_t ratio_wh = (float64_t)data.size().width / data.size().height;
	  float64_t dva_wh = input_dva_wid / input_dva_hei;
	  float64_t errorthresh = 0.025;
	  fprintf( stdout, "First input of [%s]: dva W/H ratio is [%lf], pixel ratio is [%lf] (REV: will exit if absolute ratio error is > thresh [%lf])\n", ratio_wh, dva_wh, errorthresh );
	  float64_t absdiff = abs( ratio_wh - dva_wh );
	  if( absdiff > errorthresh )
	    {
	      fprintf(stderr, "Absolute error difference is [%lf] (> thresh [%lf])  exiting...\n", absdiff, errorthresh );
	      exit(1);
	    }

	}

      
      
      ptroriginputinsts[ localname ] = FeatMapImplInst( get_time_nsec(), input_dva_wid, input_dva_hei, origaspect_wh );

      
      data.copyTo(ptroriginputinsts[ localname ].cpu_w());
    } 
  else
    {
      cv::Size origsize=ptroriginputinsts[ localname ].size(); 
      if( origsize != data.size() || ptroriginputinsts[ localname ].cpu().type() != data.type() )
	{
	  fprintf(stderr, "You are trying to add new input for [%s] but previous (first) input was different size (or type)!!! New size [%d %d]    Original size [%d %d]\n", data.size().width, data.size().height, origsize.width, origsize.height );
	  exit(1);
	}
    }
  
}




void salmap_rv::SalMap::add_input_copy( const std::string& localname, const cv::Mat& data )
{
  DEBUGPRINTF(stdout, "Adding input copy [%s] (size %d %d)\n", localname.c_str(), data.size().width, data.size().height );
  setup_input( localname, data);
  set_copy( localname, data );
}


void salmap_rv::SalMap::add_input_copy_realtime( const std::string& localname, const cv::Mat& data, const float64_t realtime_sec )
{
  DEBUGPRINTF(stdout, "Adding input copy [%s] (size %d %d)\n", localname.c_str(), data.size().width, data.size().height );
  setup_input( localname, data );
  set_copy_realtime( localname, data, realtime_sec );
}






void salmap_rv::SalMap::add_input_direct( const std::string& localname, const cv::Mat& data )
{
  DEBUGPRINTF(stdout, "Adding input direct [%s] (size %d %d)\n", localname.c_str(), data.size().width, data.size().height );
  setup_input( localname, data);

#ifdef USE_OPENCL
  fprintf(stderr, "REV: add_input_direct --> correcting to add_input_copy because USE_OPENCL is defined\n");
  set_copy( localname, data );
#else
  set_direct( localname, data );
#endif
}

void salmap_rv::SalMap::add_input_direct_realtime( const std::string& localname, const cv::Mat& data, const float64_t realtime_sec )
{
  DEBUGPRINTF(stdout, "Adding input direct [%s] (size %d %d)\n", localname.c_str(), data.size().width, data.size().height );
  setup_input( localname, data);

#ifdef USE_OPENCL
  fprintf(stderr, "REV: add_input_direct --> correcting to add_input_copy because USE_OPENCL is defined\n");
  set_copy_realtime( localname, data, time_sec );
#else
  set_direct_realtime( localname, data, realtime_sec );
#endif
}

bool salmap_rv::SalMap::insert_raw( const std::string& filtname, const std::string& outputname, const std::string& targdir, const double dva_w, const double dva_h, const double aspect )
  {
    std::string ext=".exr";
    int64_t targtime_nsec = get_time_nsec();
    std::string timestr = stringify_num(targtime_nsec); 
    std::string targfname = targdir + "/" + filtname + "_" + timestr + ext;
    cv::Mat m = cv::imread( targfname, cv::IMREAD_ANYCOLOR | cv::IMREAD_ANYDEPTH );
    
    DEBUGPRINTF(stdout, "Getting filter [%s] for time [%ld] from file [%s] (size of resulting mat is %d %d)\n", filtname.c_str(), targtime_nsec, targfname.c_str(), m.size().width, m.size().height);
    if( 0 == m.size().width )
      {
	return false;
      }
    else
      {
	
	insert_raw_direct( filtname, outputname, m, dva_w, dva_h, aspect );
	return true;
      }
  }


void salmap_rv::SalMap::insert_raw_direct( const std::string& filtname, const std::string& outputname, cv::InputArray m, const double dva_w, const double dva_h, const double aspect )
  {
    int64_t targtime_nsec = get_time_nsec(); 
    auto fiter = filtset.filters.find(filtname);
    if( fiter == filtset.filters.end() )
      {
	fprintf(stderr, "REV: ERROR insert_raw: no such filter for output [%s]\n", filtname.c_str());
	return;
      }
    else
      {
	FeatMapImplPtr finfp = getMapAccessor( outputname, filtname );
	
	FeatMapImplInst fin( targtime_nsec, dva_w, dva_h, aspect );
	if( false == deps_built )
	  {
	    fprintf(stderr, "(insert_raw, whoa!) Dependencies not yet built, building!\n");
	    build_dependencies();
	    filtergraph.reset_empty();
	    
	  }
	
	finfp.set_abs_curr_time( targtime_nsec );
	
	std::shared_ptr<FeatMapImplInst> retptr;
	finfp.setFrom( retptr, targtime_nsec ); 

	
	fin.copyMetadataTo( retptr );
	
	
	
	
	
	
	retptr->copyFromCpu(m);
	
	FeatFilterImpl* ffi;
	filtimpls.getFilter( fiter->second, ffi );
	
	filtergraph.curr_processing.push_back( ffi );
	filtergraph.mark_finished( ffi );
	
      }
  }


void salmap_rv::SalMap::save_to_file( const std::string& filtname, const std::string& outputname, const std::string& targdir )
{
  int64_t targtime_nsec = get_most_recent_update_time_nsec();
  std::string ext=".exr";
  std::string timestr = stringify_num(targtime_nsec); 
  std::string targfname = targdir + "/" + filtname + "_" + timestr + ext;
    
  auto fiter = filtset.filters.find(filtname);
  if( fiter == filtset.filters.end() )
    {
      fprintf(stderr, "REV: ERROR save_to_file: no such filter for output [%s]\n", filtname.c_str());
      return;
    }
  else
    {
      FeatMapImplPtr finfp = getMapAccessor( outputname, filtname );
	
      std::shared_ptr<FeatMapImplInst> fin;
      bool gotfin = mapFromAccessor( finfp, fin, targtime_nsec );
      if( gotfin )
	{
	  DEBUGPRINTF(stdout, "REV: Outputting save_to_file for filter [%s]\n", filtname.c_str() );
	  
	  
	  cv::imwrite( targfname, fin->cpu() );
	  return;
	}
      else
	{
	  fprintf(stderr, "Couldn't get filter [%s]'s time (nsec) [%ld] matrix\n", filtname.c_str(), targtime_nsec );
	  return;
	}

	
    }
}
  
bool salmap_rv::SalMap::update()
{
  return update( get_dt_nsec() );
}


bool salmap_rv::SalMap::update_single_thread( const int64_t& dt_in_nsec )
{
  std::mutex mux2;

  
  
    
  bool nextexists=false;
  std::condition_variable cv;
  std::list<size_t> finished_idxs;
  std::vector<std::future<bool>> myfutures; 
  std::vector<FeatFilterImpl*> next_to_exec;
  std::vector<bool> tmpsuc;
  bool success=true;
  size_t execed=0;
  size_t currproc = 0;
  do
    {
      FeatFilterImpl* nexttoexec=NULL;
      nextexists = filtergraph.nextfilter( nexttoexec ); 
	
      currproc = filtergraph.currproc();
	
      
      if( nextexists )
	{
	  next_to_exec.push_back( nexttoexec );
	  size_t myidx = next_to_exec.size()-1;
	    
	  
	   
	  
	  
	  tmpsuc.push_back( packaged_update( 0, next_to_exec.back(), std::ref(mapimpls), get_time_nsec(), std::ref(mux2), std::ref(cv), myidx, std::ref(finished_idxs), std::ref(filtimpls), std::ref(filtset)
#ifdef GPU
					     , std::ref(streams)
#endif
					     )  );
	}
      else
	{
	  if(finished_idxs.size() > 0 )
	    {
		
	      size_t finidx = finished_idxs.front();
	      finished_idxs.pop_front();
	      bool suc = tmpsuc[finidx];
	      filtergraph.mark_finished(next_to_exec[finidx]);
	      if( false == suc )
		{
		  success = false;
		}
	      ++execed;
	    }
	}
    }
  while ( true == nextexists || currproc > 0 );

  
  
  
  
  successfully_updated = success;

  
  filtergraph.reset();
    
  return success;
  
} 



bool salmap_rv::SalMap::update_multi_thread( const int64_t& dt_in_nsec )
{
  std::mutex mux2;

  
  
    
  bool nextexists=false;
  std::condition_variable cv;
  std::list<size_t> finished_idxs;
  std::vector<std::future<float>> myfutures; 
  std::vector<FeatFilterImpl*> next_to_exec;
#if TIMINGLEVEL > 0
  std::vector<Timer> timers;
#endif
  
  bool success=true;
  size_t execed=0;
  size_t currproc = 0;

  Timer bureautimer;
  double totalfilttime=0;
  double waitedsec=0;
  do
    {
      FeatFilterImpl* nexttoexec=NULL;
      nextexists = filtergraph.nextfilter( nexttoexec ); 
	
      
      currproc = filtergraph.currproc();
	
      
      if( nextexists )
	{
	  DEBUGPRINTF(stdout, "Next filter existed... (currproc: %ld)\n", currproc );
	  next_to_exec.push_back( nexttoexec );
#if TIMINGLEVEL > 0
	  timers.emplace_back(); 
#endif
	  
	  size_t myidx = next_to_exec.size()-1;
	  
	  
	  myfutures.push_back(
			      tp->enqueue(
					  packaged_update, next_to_exec.back(), std::ref(mapimpls), get_time_nsec(), std::ref(mux2), std::ref(cv), myidx, std::ref(finished_idxs), std::ref(filtimpls), std::ref(filtset)
#ifdef GPU
					  , std::ref(streams)
#endif
					  )
			      );
	  
	  
	}
      else
	{
	  DEBUGPRINTF(stdout, "Next filter didn't exist... (currproc: %ld)\n", currproc );
	  {
	    
	    
	    std::unique_lock<std::mutex> lk( mux2 );
	    
	    
	    while( filtergraph.currproc() > 0 && finished_idxs.size() == 0 ) 
	      {
		
		  
		
		 
		

		cv.wait_for( lk, std::chrono::microseconds(200) ); 
		
	      }
	      
	    
	    if(finished_idxs.size() > 0 )
	      {

#if TIMINGLEVEL > 0
		fprintf(stdout, "Bureau Timer took [%lf] msec\n", bureautimer.elapsed()*1e3 );
		bureautimer.reset();
#endif
		
		size_t finidx = finished_idxs.front();
		finished_idxs.pop_front();
		float filttime = myfutures[finidx].get(); 
		filtergraph.mark_finished(next_to_exec[finidx]); 
		totalfilttime += filttime;
		
		if( filttime < 0 )
		  {
		    fprintf(stdout, "Filter [%s] failed to successfully execute\n", next_to_exec[finidx]->name.c_str());
		    success = false;
		  }
		else
		  {
#if TIMINGLEVEL > 0
		    fprintf(stdout, "Elapsed: [%lf] msec  ( filter [%s] ) (2nd bureau [%lf] msec)\n",  timers[finidx].elapsed()*1000, next_to_exec[finidx]->name.c_str(), bureautimer.elapsed()*1e3 );
		    bureautimer.reset();
#endif
		  }
		++execed;
	      }
	  }
	}
	
	
    }
  while ( true == nextexists || currproc > 0 );
  
  
  
  

  
  
  
    
  successfully_updated = success;
    
  
  filtergraph.reset();
    
  return success;
}

  

bool salmap_rv::SalMap::update( const int64_t& dt_in_nsec )
{

  Timer t;
  build_dependencies();

  
  if( get_trial_timesteps() > 0 &&
      get_time_idx() == get_trial_timesteps() )
    {
      HELPPRINTF(stdout, "Switching off TRIAL PERIOD\n");
      mapimpls->setdoneinit();
    }
  
  bool success = false;
  if( 0 == tp->get_size() )
    {
      success = update_single_thread( dt_in_nsec );
    }
  else
    {
      success = update_multi_thread( dt_in_nsec );
    }
  
  advance_time();

  
  return success;
}





void salmap_rv::SalMap::set_filt_input_param( FeatFilter& f, const std::string& argstr ) 
{
  
  auto tokenized = tokenize_string( argstr, ":", true );
  if( tokenized.size() > 0 )
    {
      
      std::string type = tokenized[0];
      std::string second = CONCATENATE_STR_ARRAY( tokenized, ":", 1 );
      tokenized = tokenize_string( second, "=", true );
      if( tokenized.size() > 0 )
	{
	  std::string localname = tokenized[0];
	  std::string value = CONCATENATE_STR_ARRAY( tokenized, "=", 1 );
	  if( same_string( type, "param" ) )
	    {
	      auto iter = f.params.find( localname );
	      if( iter == f.params.end() )
		{
		  f.params[localname] = value;
		}
	      else
		{
		  fprintf(stderr, "doubling param localnames... [%s]\n", localname.c_str());
		  exit(1);
		}
	    }
	  else if( same_string( type, "input" ) )
	    {
	      auto iter = f.inputs.find( localname );
	      if( iter == f.inputs.end() )
		{
		  DEBUGPRINTF(stdout, "Setting filter input to : [%s]\n", value.c_str());
		  f.inputs[localname] = value; 
		}
	      else
		{
		  fprintf(stderr, "doubling input localnames... [%s]\n", localname.c_str());
		  exit(1);
		}
	    }
	  else
	    {
	      fprintf(stderr, "unrecognized type of input [%s]\n", type.c_str() );
	      exit(1);
	    }
	}
      else
	{
	  fprintf(stderr, "Should be input|param:localname=value (this is part after :) [%s]\n", second.c_str());
	  exit(1);
	}
    }
  else
    {
      fprintf(stderr, "Should be input|param:localname=value [%s]\n", argstr.c_str());
      exit(1);
    }
      
} 

void salmap_rv::SalMap::set_filt_input_param( FeatFilter& f,  const std::vector<std::string>& argvec )
{
  
  for( auto& argstr : argvec )
    {
      set_filt_input_param(f, argstr);
    }
} 

  

  
  
  
  
  
  
  
  
  
taggraph salmap_rv::SalMap::taggraph_from_salmap()
{
  
  
  

  

  taggraph graph;
    
  std::map<FeatFilterImpl*, std::shared_ptr<tagnode>> algnodes;
    
  for( auto& f : filtergraph.deps.deps )
    {
      DEBUGPRINTF(stdout, "Adding [%s]\n", f.first->algorithm.name.c_str() );
      auto p = algnodes.find(f.first);
      if( p != algnodes.end() )
	{
	  exit(1);
	}
	
      algnodes[f.first] = graph.add_node( tagnode( algonode( f.first->algorithm.name, f.first->algorithm.params, f.first->name ) ) );
    }

  
  
  

  
  for( auto& f : filtergraph.deps.deps )
    {
      std::shared_ptr<tagnode> prealgo = algnodes[ f.first ];
      DEBUGPRINTF(stdout, "Pre algo [%s] required by [%ld] other algos\n", algnodes[ f.first ]->get<algonode>().nickname.c_str(), f.second.required_by.size() );
	
	
      for( size_t rb=0; rb < f.second.required_by.size(); ++rb )
	{
	    
	  
	  auto it = algnodes.find(  f.second.required_by[rb] );
	  if( it == algnodes.end() )
	    {
	      fprintf(stderr, "REV: uh, couldn't find post node...?\n");
	      exit(1);
	    }
	  std::shared_ptr<tagnode> postalgo = it->second;
	  int64_t tdelta = f.second.rtdelta[rb];
	  std::string inputname = f.second.rinput_mapping[rb];
	  std::string outputname = f.second.routput_mapping[rb];
	    
	    
	  DEBUGPRINTF(stdout, "Adding [%ld] [%s] [%s] (and post algo [%s])\n", tdelta, inputname.c_str(), outputname.c_str(), postalgo->get<algonode>().nickname.c_str() );

	  auto nextout = prealgo->get_next<mapnode>( mapnode( outputname ) );
	  if( !nextout )
	    {
	      nextout = graph.add_post_to_node( prealgo, tagnode( mapnode( outputname ) ) );
	    }
	    
	  auto nexttime = nextout->get_next<timenode>( timenode( tdelta ) );
	  if( !nexttime )
	    {
	      nexttime = graph.add_post_to_node( nextout, tagnode( timenode( tdelta ) ) );
	    }
	    
	  
	  
	  
	  auto nextinput = graph.add_post_to_node( nexttime, tagnode( inputnode( inputname ) ) );
	  

	  
	  auto nextalgo = nextinput->add_post_ptr( postalgo ); 
	    	    	    
	} 
    } 

  
   

  
 
        
  return graph;
} 
  

const int64_t salmap_rv::SalMap::get_dt_nsec() const
{
  return dt_nsec;
}

const float64_t salmap_rv::SalMap::get_realtime_sec() const
{
  return realtime.elapsed();
}

const int64_t salmap_rv::SalMap::get_time_nsec() const
{
  return time_nsec;
}

const int64_t salmap_rv::SalMap::get_time_idx() const
{
  return time_idx;
}

const int64_t salmap_rv::SalMap::get_trial_timesteps() const
{
  return trial_timesteps;
}

const int64_t salmap_rv::SalMap::get_most_recent_update_time_nsec() const
{
  return get_time_nsec() - get_dt_nsec();
}

const bool salmap_rv::SalMap::was_updated() const
{
  return successfully_updated;
}
  
