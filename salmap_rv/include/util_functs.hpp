#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>







namespace salmap_rv
{


  std::string CONCATENATE_STR_ARRAY(const std::vector<std::string>& arr, const std::string& sep, const size_t& start);

  std::vector<std::string> tokenize_string(const std::string& src, const std::string& delim, bool include_empty_repeats);


  cv::Mat zeroonenorm( const cv::Mat& m );

  std::string maketab ( const size_t& tablevel);

  template <typename T>
  std::vector<std::pair< std::string,T>> map_as_vect( const std::map<std::string, T>& m );
  
  
  template <typename T>
  std::string stringify_num( const T& arg );


  bool same_string( const std::string& a, const std::string& b );


  std::string type2str(int type);

  cv::Mat non_maxima_suppression_cpu( cv::InputArray image, const float64_t& thresh );

  size_t size_in_mem_strstrmap( const std::map<std::string,std::string>& input );
  size_t size_in_mem_strvect( const std::vector<std::string>& input );

  
  
  
  void attenuation_filter_cpu_wid( const float64_t& wid, cv::InputArray in, salmap_rv::FiltMat& out );
  void attenuation_filter_cpu_prop( const float64_t& prop, cv::InputArray in, salmap_rv::FiltMat& out );
  void attenuation_filter_cpu( const float64_t& prop,  cv::InputArray  in, salmap_rv::FiltMat& out );
  
  
  double avg_nonzero_cpu( cv::InputArray image, int& nummaxes );


  void zero_rectify_cpu( cv::InputArray m, cv::OutputArray output );

  template <typename T>
  std::string tostring( const T& i, const uint32_t precision=10  ); 

  double deg_to_rad( const double& deg );

  bool FAIL_EXIT_NANINF( const cv::Mat& m, const std::string tag="NONE", const bool doexit=true );
  
  std::string pretty_inputs( std::map<std::string, mapphrase>& m, const size_t& tablevel=0 );
  
  std::string pretty_params( std::map<std::string, std::string>& m, const size_t& tablevel=0 );

  double pixval_from_slope( const int x, const double islope );
  
  float64_t nyqf_from_dvaperpix( const double dvaperpix );
  float64_t nyqf_from_pixperdva( const double pixperdva );
  
  float64_t roundf_sig( const float64_t arg, const int sigdec );

  cv::Mat apply_color( const cv::Mat& input );
  cv::Mat overlay_salmap( const cv::Mat& orig, const cv::Mat& salmap, const double& salmapalpha, const int interp=cv::INTER_NEAREST );
  cv::Mat make_pretty( const cv::Mat& input, const std::string& name=std::string(), const int64_t time=-1, const cv::Mat& raw=cv::Mat(), const float alpha=0.7, const int interp =cv::INTER_NEAREST);
  void draw_eye_pos(cv::Mat& mymat, const float& eyex, const float& eyey);
  cv::Mat make_bg( const cv::Size& size, const cv::Scalar& col );
  cv::Mat embed_bg( const cv::Size& size, const cv::Scalar& col, const cv::Mat& input, const int offsetx=0, const int offsety=0 );
  void add_name( const std::string& name, cv::Mat& m);
  void add_timing( const int64_t& msec, cv::Mat& m);
  
  
  

  
  float64_t downsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				    int& retwid, int& rethei );

  float64_t _upsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				   int& retwid, int& rethei );
  float64_t _downsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				     int& retwid, int& rethei );
  float64_t upsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				  int& retwid, int& rethei );

  float64_t resize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				  int& retwid, int& rethei );

  float64_t dva_to_pix( const float64_t dva, const float64_t dva_per_pix);
  float64_t pix_to_dva( const float64_t pix, const float64_t dva_per_pix );

  
  float64_t sigma_from_sigmaf( const float64_t sigmaf );

  float64_t sigmaf_from_sigma( const float64_t sigma );

  void sigma_from_freq_modul( const float64_t targf, const float64_t targmodul,
			      float64_t& retsig, float64_t& retsigf );

  void freq_modul_from_sigma( const float64_t targsig, const float64_t targmodul,
			      float64_t& rettargf );

  float64_t bandwidth_comp( const float64_t sigma, const float64_t f0 );

  float64_t sigma_from_bandwidth_lambda( const float64_t bandwidth_octaves, const float64_t lambda );

  void compute_octave( const float64_t fc,
		       float64_t& fmin, float64_t& fmax);

  float64_t f_mod_by_gabor( const float64_t f, const float64_t f0, const float64_t sigma);


  
  std::string print_str_str_map( const std::map<std::string, std::string>& m );


  template <typename T>
  bool element_in_map( std::map<std::string,T>& m, const std::string& name, T*& ret );

  std::string map_from_filter_abstime( FeatFilter& f, const std::string& localname, const int64_t& offset );
  std::string map_from_filter_reltime( FeatFilter& f, const std::string& localname, const int64_t& offset );













 



 

  float64_t param_as_float64( const parammap& params, const std::string& paramname );
  float64_t param_as_int64( const parammap& params, const std::string& paramname );

  std::string enum_params( std::map< std::string, std::string >& m );
  std::string pretty_params( std::map<std::string, std::string>& m, const size_t& tablevel );

  cv::Mat get_cpu_size( const cv::Mat& m, const size_t& width, const size_t& height );

  void work_thread( exec_graph& filtergraph, std::shared_ptr<FeatMapImplSet>& mapimpls, const int64_t time_nsec, std::mutex& mux, bool& doloop, bool& success, size_t& execed
#ifdef GPU
		    , cv::cuda::Stream& stream
#endif
		    );

  float packaged_update( size_t threadidx, FeatFilterImpl* next_to_exec, std::shared_ptr<FeatMapImplSet>& mapimpls, const int64_t time_nsec, std::mutex& mux, std::condition_variable& cv, const size_t myidx, std::list<size_t>& finished_idxs, const FeatFilterImplSet& filtimpls, const FilterSet& filts
			
#ifdef GPU
			, std::vector<cv::cuda::Stream>& streams
#endif
			);
  
  
  template <typename T>
  T& get_from_dict_or_fail( std::map<std::string,T>& dict, const std::string& targ );

  std::shared_ptr<FeatMapImplInst> get_from_collection_or_fail( std::shared_ptr<FeatMapImplCollection>& col, const std::string& name, const int64_t& attime_nsec );
  
  bool issamestring( const std::string& a, const std::string& b );
  
  std::string print_time( const int64_t time, const size_t nchars );
  
  cv::Mat resize_wid( cv::InputArray frame, const size_t targwid, const int interp );  
  
  cv::Mat resize_hei( cv::InputArray frame, const size_t targwid, const int interp );
  
  cv::Mat resize(  cv::InputArray frame, const cv::Size& newsize, const int interp  );

  int ksize_compute( const float64_t sigma );

  int ksize_Nsigma( const float64_t sigma, const float64_t mult );

  int ksize_shiftlinear( const float64_t sigma );

  std::vector<float64_t> recursive_gauss_lpf_sigmas( const std::vector<cv::Size>& recur_sizes, const double origpixperdva_w, const double nyq_sigmaf_mult, std::vector<float64_t>& pixperdvas );

  std::vector<cv::Size>  recursive_downsample( const cv::Size& currsize, const cv::Size& targsize, const double min_perstep_ratio, const double max_perstep_ratio,  const double orig_aspect_wh, const double allowederr );

  void recursive_downsize_2mat( const salmap_rv::FiltMat& i, salmap_rv::FiltMat& m1, salmap_rv::FiltMat& m2,
				const cv::Size& targsize, const double min_perstep_ratio, const double max_perstep_ratio,  const double orig_aspect_wh, const double allowederr,
				const double pix_per_dva_w, const double nyq_sigmaf_mult,
				const int interp );

  void recursive_downsize_2mat_optim( const salmap_rv::FiltMat& i, salmap_rv::FiltMat& m1, salmap_rv::FiltMat& m2,
				      const std::vector<cv::Size>& sizes, const std::vector<float64_t>& pixperdvas, const std::vector<float64_t>& blurwidths_dva,
				      const int interp );




  
  
  

  
  
  


  
  const std::string PARAM_GRP_PREFIX_CONNECTOR = "-->";
  const std::string PARAM_GRP_CONJUNCTION_SEP = "+++";

  struct param_group
  {
    std::vector<std::string> data;

    
    
    
    
    
    
    
    
    template <class ...Ts> 
    param_group( Ts... rest ) : data{ rest... }
    {
      
      bool include_empty_repeats = true;
      std::vector<std::string>  newdata;
      
      for( auto& s1 : data )
	{
	  std::vector<std::string> separated = tokenize_string( s1, PARAM_GRP_PREFIX_CONNECTOR, include_empty_repeats );
	  for( auto& s2 : separated )
	    {
	      newdata.push_back( s2 );
	      if( s2.empty() )
		{
		  fprintf(stderr, "Empty grp name detected (double repeat?)\n");
		  exit(1);
		}
	    }
	}
    }

    
    
    std::string tostr() const
    {
      std::string s = "";
      if( data.size() > 0 )
	{
	  s += data[0];
	  for( size_t x=1; x<data.size(); ++x )
	    {
	      s += PARAM_GRP_PREFIX_CONNECTOR + data[x];
	    }
	}
      return s;
    }

    bool is_same_or_more_specific_than( const param_group& pg2 ) const
    {
      if( data.size() >= pg2.data.size() )
	{
	  for( size_t a = 0; a<pg2.data.size(); ++a )
	    {
	      
	      if( 0 != pg2.data[a].compare( data[a] ) )
		{
		  return false;
		}
	    }
	  return true;
	}
      else
	{
	  return false;
	}
    }
  };
  
  
  


  
  
  
  
  
  
  
  
  
  

  
  
  struct grouped_param
  {
    std::vector<param_group> groups;
    std::string name;
    std::string value;

    
    bool in_group( const std::string& targgrp )
    {
      param_group pg( targgrp );
      return in_group( pg );
    }

    std::string getfullname() const
    {
      std::string s = "";
      for( auto& grp : groups )
	{
	  s += grp.tostr() + PARAM_GRP_CONJUNCTION_SEP;
	}
      s += name;
      return s;
    }

    template <typename T>
    T getvalue() const
    {
      T ret;
      std::stringstream ss( value );
      ss >> ret;
      return ret;
    }
    
    bool in_group( const param_group pg )
    {
      for( auto& g : groups )
	{
	  bool yes = g.is_same_or_more_specific_than( pg );
	  if( true == yes )
	    {
	      return true;
	    }
	}
      return false;
    }
      
    
    std::string tostr()
    {
      std::string s = "";
      for( auto& grp : groups )
	{
	  s += grp.tostr() + PARAM_GRP_CONJUNCTION_SEP;
	}
      s += name + "=" + value;
      return s;
    };

    grouped_param( const std::string& fullstr )
    {
      bool include_empty_repeats = false;
      std::vector<std::string> separated = tokenize_string( fullstr, PARAM_GRP_CONJUNCTION_SEP, include_empty_repeats );

      
      
      if( separated.size() < 1 )
	{
	  fprintf(stderr, "contained nothing... wtf?");
	  exit(1);
	}
      else
	{
	  for( size_t x=0; x<separated.size()-1; ++x )
	    {
	      std::string str = separated[x];
	      if( str.empty() )
		{
		  fprintf(stderr, "input grp separated contained empty repeat [%s]\n", fullstr.c_str() );
		  exit(1);
		}
	      else
		{
		  groups.push_back( param_group( separated[x] ) );
		}
	    }
	  
	  std::string nameval = separated.back();
	  const std::string sep="=";
	  std::vector<std::string> namevaltok = tokenize_string( nameval, sep, include_empty_repeats );
	  if( namevaltok.size() == 0 )
	    {
	      
	      fprintf(stderr, "(grp param) namevaltok < 1\n");
	      exit(1);
	    }
	  if( namevaltok.size() > 0 )
	    {
	      name = namevaltok[0];
	      
	      if( name.empty() ) { fprintf(stderr, "Name empty\n"); exit(1); }
	      
	    }
	  if( namevaltok.size() > 1 )
	    {
	      value = namevaltok[1];
	      if( value.empty() ) {  fprintf(stderr, "Value empty\n"); exit(1); }
	    }
	  if( namevaltok.size() > 2 )
	    {
	      fprintf(stderr, "Namevaltok > 2?\n");
	      exit(1);
	    }
	}
    } 
    
    
    
     

    
    
    
    
    template <class ...Ts>
    grouped_param( const std::string& _name, const std::string& _value, Ts... rest ) : groups{  rest... }
    {
      
      for( size_t x=0; x<groups.size(); ++x )
	{
	  
	}
      name = _name;
      value = _value;
    }
    
  }; 



  
  struct grouped_input
  {
    std::vector<param_group> groups;
    std::string name; 
    std::string value;


    std::string getname() const
    {
      return name;
    }

    std::string getfullname() const
    {
      std::string s = "";
      for( auto& grp : groups )
	{
	  s += grp.tostr() + PARAM_GRP_CONJUNCTION_SEP;
	}
      s += name;
      return s;
    }
    
    
    bool in_group( const std::string targgrp )
    {
      param_group pg( targgrp );
      return in_group( pg );
    }
    
    bool in_group( const param_group& pg )
    {
      for( auto& g : groups )
	{
	  bool yes = g.is_same_or_more_specific_than( pg );
	  if( true == yes )
	    {
	      return true;
	    }
	}
      return false;
    }
      
    
    std::string tostr()
    {
      std::string s = "";
      for( auto& grp : groups )
	{
	  s += grp.tostr() + PARAM_GRP_CONJUNCTION_SEP;
	}
      s += name + "=" + value;
      return s;
    };


     
    
    
    
    grouped_input( const std::string& _name )
    {
      
      bool include_empty_repeats = false;
      std::vector<std::string> separated = tokenize_string( _name, PARAM_GRP_CONJUNCTION_SEP, include_empty_repeats );
      
      if( separated.size() < 1 )
	{
	  fprintf(stderr, "contained nothing... wtf?");
	  exit(1);
	}
      else
	{
	  for( size_t x=0; x<separated.size()-1; ++x )
	    {
	      std::string str = separated[x];
	      if( str.empty() )
		{
		  fprintf(stderr, "input grp separated contained empty repeat [%s]\n", _name.c_str() );
		  exit(1);
		}
	      else
		{
		  groups.push_back( param_group( separated[x] ) );
		}
	    }
	  
	  std::string nameval = separated.back();
	  const std::string sep="=";
	  std::vector<std::string> namevaltok = tokenize_string( nameval, sep, include_empty_repeats );

	  if( namevaltok.size() == 0 )
	    {
	      fprintf(stderr, "(grp input) namevaltok < 1\n");
	      exit(1);
	    }
	  if( namevaltok.size() > 0 )
	    {
	      name = namevaltok[0];
	      
	      if( name.empty() ) { fprintf(stderr, "Name empty\n"); exit(1); }
	      
	    }
	  if( namevaltok.size() > 1 )
	    {
	      value = namevaltok[1];
	      if( value.empty() ) {  fprintf(stderr, "Value empty\n"); exit(1); }
	    }
	  if( namevaltok.size() > 2 )
	    {
	      fprintf(stderr, "Namevaltok > 2?\n");
	      exit(1);
	    }
	  
	}
    } 

    
    
    

     

    
    template <class ...Ts>
    grouped_input( const std::string& _name, const std::string& _value, Ts... rest ) : groups{ rest... }
    {
      name = _name;
      value = _value;
    }
    
  };  


  
  template <class ...Rest>
  void set_filt_input_params( FeatFilter& f, grouped_param gp, Rest... rest);

  template <class ...Rest>
  void set_filt_input_params( FeatFilter& f, grouped_input gi, Rest... rest);

  template <class ...Rest>
  void set_filt_input_params( FeatFilter& f, std::vector<grouped_param> gpv, Rest... rest);
  
  template <class ...Rest>
  void set_filt_input_params( FeatFilter& f, std::vector<grouped_input> giv, Rest... rest);
  
  
  
  void set_filt_input_params( FeatFilter& f );

  void set_filt_input( FeatFilter& f, grouped_input gi );

  void set_filt_param( FeatFilter& f, grouped_param gp );




  
  template <typename T>
  std::vector<std::string> raw_params_from_group( const std::map<std::string,T>& paramsmap, const param_group targpg );

  template <typename T>
  std::map<std::string,std::string> flat_raw_params_from_group( const std::map<std::string,T>& paramsmap, const param_group targpg );

  
  template <typename T>
  std::vector<std::string> raw_inputs_from_group( const std::map<std::string,T>& inputsmap, const param_group targpg );

  template <typename T>
  std::map<std::string,std::string> flat_raw_inputs_from_group( const std::map<std::string,T>& inputsmap, const param_group targpg );

    
} 



using namespace salmap_rv;




template <typename T>
std::string salmap_rv::tostring( const T& i, const uint32_t precision ) 
{
  std::stringstream ss;
  
  ss << std::fixed << std::setprecision(precision) << std::fixed << i;
  return ss.str();
}







template <typename T>
std::vector<std::pair< std::string,T>> salmap_rv::map_as_vect( const std::map<std::string, T>& m )
{
  std::vector<std::pair< std::string,T>> v;
  for (auto it=m.begin(); it!=m.end(); ++it)
    {
      v.push_back( std::pair<std::string,T>( it->first, it->second ) );
    }
  return v;
}



template <typename T>
std::string salmap_rv::stringify_num( const T& arg )
{
  std::stringstream ss;
  ss << std::setprecision(STR_FLOAT_PRECISION) << std::setfill('0') << std::fixed << std::setw(3+STR_FLOAT_PRECISION) << arg;
  return ss.str();
}



template <typename T>
bool salmap_rv::element_in_map( std::map<std::string,T>& m, const std::string& name, T*& ret )
{
  auto it = m.find( name );
  if( it == m.end() )
    {
      return false;
    }
  else
    {
      ret = & (it->second); 
      return true;
    }
}



template <typename T>
T& salmap_rv::get_from_dict_or_fail( std::map<std::string,T>& dict, const std::string& targ )
{
  DEBUGPRINTF(stdout, "Attempting to get in dict or fail : [%s]\n", targ.c_str() );
  auto it = dict.find(targ);
  if( it == dict.end() )
    {
      fprintf(stderr, "FAIL! Failed to find [%s] in dict. Entries:\n");
      for( auto& entry : dict )
	{
	  fprintf(stderr, "[%s]\n", entry.first.c_str() );
	}
      exit(1);
    }
  else
    {
      DEBUGPRINTF(stdout, "Found it in dict...is null ptr?\n");
      if( nullptr == it->second )
	{
	  fprintf(stderr, "Yep, it's nullptr\n");
	  exit(1);
	}
    }
  
  DEBUGPRINTF(stdout, "[%d %d] dummy? [%s]\n", it->second->cpu().size().width, it->second->cpu().size().height, it->second->isDummy() ? "true" : "false" );

#ifndef ALLOW_NONEXISTENT_MAP_INPUTS
  if( it->second.cpu().empty() )
    {
      fprintf(stderr, "Input in dict [%s] is empty mat (define ALLOW_NONEXISTENT_MAP_INPUTS to allow this)!\n", targ.c_str() );
      exit(1);
    }
#endif
  
  
  return it->second;
}




template <typename ...Rest>
void salmap_rv::set_filt_input_params( FeatFilter& f, grouped_param gp, Rest... rest)
{
  
  set_filt_param( f, gp );
  set_filt_input_params( f, rest... );
}

template <typename ...Rest>
void salmap_rv::set_filt_input_params( FeatFilter& f, std::vector<grouped_param> gpv, Rest... rest)
{
  
  for( auto& gp : gpv )
    {
      set_filt_param( f, gp );
    }
  set_filt_input_params( f, rest... );
}

template <typename ...Rest>
void salmap_rv::set_filt_input_params( FeatFilter& f, std::vector<grouped_input> giv, Rest... rest)
{
  

  fprintf(stdout, "Calling with INPUT vector with size [%ld]\n", giv.size() );
  for( auto& gi : giv )
    {
      set_filt_input( f, gi );
    }
  set_filt_input_params( f, rest... );
}

template <typename ...Rest>
void salmap_rv::set_filt_input_params( FeatFilter& f, grouped_input gi, Rest... rest)
{
  
  set_filt_input( f, gi );
  set_filt_input_params( f, rest... );
}






template <typename T>
std::vector<std::string> salmap_rv::raw_params_from_group( const std::map<std::string,T>& paramsmap, const param_group targpg )
{
  std::vector<std::string> res;
  for( auto it=paramsmap.begin(); it != paramsmap.end(); ++it )
    {
      grouped_param gp( it->first );
      if( gp.in_group( targpg ) )
	{
	  res.push_back( it->first );
	}
    }
  return res;
}


template <typename T>
std::vector<std::string> salmap_rv::raw_inputs_from_group( const std::map<std::string,T>& inputsmap, const param_group targpg )
{
  std::vector<std::string> res;
  for( auto it=inputsmap.begin(); it != inputsmap.end(); ++it )
    {
      grouped_input gp( it->first );
      if( gp.in_group( targpg ) )
	{
	  res.push_back( it->first );
	}
    }
  return res;
}




template <typename T>
std::map<std::string,std::string> salmap_rv::flat_raw_params_from_group( const std::map<std::string,T>& paramsmap, const param_group targpg )
{
  std::map<std::string,std::string> res;
  for( auto it=paramsmap.begin(); it != paramsmap.end(); ++it )
    {
      grouped_param gp( it->first );
      if( gp.in_group( targpg ) )
	{
	  auto found = res.find( gp.name );
	  if( found != res.end() )
	    {
	      fprintf(stderr, "Flat raw params from grp: more than one flat param name found\n");
	      exit(1);
	    }
	  res[gp.name] = it->first;
	}
    }
  return res;
}


template <typename T>
std::map<std::string,std::string> salmap_rv::flat_raw_inputs_from_group( const std::map<std::string,T>& inputsmap, const param_group targpg )
{
  std::map<std::string,std::string> res;
  for( auto it=inputsmap.begin(); it != inputsmap.end(); ++it )
    {
      grouped_input gi( it->first );
      if( gi.in_group( targpg ) )
	{
	  auto found = res.find( gi.name );
	  if( found != res.end() )
	    {
	      fprintf(stderr, "Flat raw inputs from grp: more than one flat param name found\n");
	      exit(1);
	    }
	  res[gi.name] = it->first;
	}
    }
  return res;
}
