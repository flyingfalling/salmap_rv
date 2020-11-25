#include <salmap_rv/include/itti_salmap.hpp>

#include <salmap_rv/include/param_set.hpp>
#include <salmap_rv/include/salmap.hpp>

#include <salmap_rv/include/util_functs.hpp>

using namespace salmap_rv;


#define GETINT(n) p.get<int64_t>(std::string(#n))
#define GETFLT(n) p.get<float64_t>(std::string(#n))
#define GETSTR(n) p.get<std::string>(std::string(#n))
#define GETBOOL(n) p.get<bool>(std::string(#n))

#define SETINT(n,v) p.set<int64_t>(std::string(#n), v)
#define SETFLT(n,v) p.set<float64_t>(std::string(#n),v)
#define SETSTR(n,v) p.set<std::string>(std::string(#n),v)
#define SETBOOL(n,v) p.set<bool>(std::string(#n),v)



param_set salmap_rv::itti_formal_default_params() 
{
  double lum = 1.0;
  double mot = 1.0;
  double ori = 4.0;
  double col = 1.0;
  double SIGMA_MULT=3;
  
  param_set p;
  
  
  
  
  
  p.set<float64_t>( "lum_f0_cyc_dva", lum ); 
  p.set<float64_t>( "lum_f0_mod", 0.9 );
  p.set<float64_t>( "lum_f0_nyq_mult", SIGMA_MULT ); 
  
  
  
  
  
  
  
  
  
  

  
  
  

  
  
  
  
  
  p.set<float64_t>( "ori_gabor_f0_cyc_dva", ori );
  p.set<float64_t>( "ori_gabor_f0_bandwidth_octaves", 1.0 ); 
  p.set<float64_t>( "ori_gabor_prop_edge_atten", 1 ); 
  

  
  p.set<float64_t>( "ori_hpf_f0_cyc_dva", ori ); 
  p.set<float64_t>( "ori_hpf_f0_mod", 0.9 );
  p.set<float64_t>( "ori_hpf_f0_nyq_mult", SIGMA_MULT ); 
  
  p.set<int64_t>("ori_n_dir", 4);
  
    
  p.set<float64_t>( "mot_f0_cyc_dva", mot );
  p.set<float64_t>( "mot_f0_mod", 0.9 );
  p.set<float64_t>( "mot_f0_nyq_mult", SIGMA_MULT ); 
  p.set<float64_t>( "mot_vel_dva_sec", 2.0 );
  p.set<float64_t>( "mot_atten_size_prop", 0.05 );
  p.set<float64_t>( "mot_thresh", 0.010 );
  
  p.set<int64_t>("mot_n_dir", 8);


  
  
  
  
  p.set<float64_t>("lpf_tau_nsec", 50000000);

  
  
  p.set<float64_t>("w_ori", 0.6);
  p.set<float64_t>("w_mot", 1);
  p.set<float64_t>("w_lum", 0.5);
  
  
  p.set<float64_t>("w_col", 0.7); 
  
  p.set<float64_t>("w_col_rg", 1);
  p.set<float64_t>("w_col_by", 1);
  
  
  
  
  p.set<int64_t>( "ori_ncenters", 3 );
  p.set<int64_t>( "ori_cstart", 2 );
  p.set<int64_t>( "ori_nsurr", 2 );
  p.set<int64_t>( "ori_soffset", 3 ); 

  
  p.set<int64_t>( "mot_ncenters", 3 );
  p.set<int64_t>( "mot_cstart", 2 );
  p.set<int64_t>( "mot_nsurr", 2 );
  p.set<int64_t>( "mot_soffset", 3 );

  p.set<int64_t>( "lum_ncenters", 3 );
  p.set<int64_t>( "lum_cstart", 2 );
  p.set<int64_t>( "lum_nsurr", 2 );
  p.set<int64_t>( "lum_soffset", 3 );

  
  
  
  p.set<bool>( "col_sep_rgby", false );
  p.set<float64_t>( "col_lum_thresh", 0.10 );
  
  p.set<int64_t>( "col_rg_ncenters", 3 );
  p.set<int64_t>( "col_rg_cstart", 2 );
  p.set<int64_t>( "col_rg_nsurr", 2 );
  p.set<int64_t>( "col_rg_soffset", 3 );

  p.set<float64_t>( "col_rg_f0_cyc_dva", col );
  p.set<float64_t>( "col_rg_f0_mod", 0.9 );
  p.set<float64_t>( "col_rg_f0_nyq_mult", SIGMA_MULT ); 

  p.set<int64_t>( "col_by_ncenters", 3 );
  p.set<int64_t>( "col_by_cstart", 2 );
  p.set<int64_t>( "col_by_nsurr", 2 );
  p.set<int64_t>( "col_by_soffset", 3 );
  
  p.set<float64_t>( "col_by_f0_cyc_dva", col );
  p.set<float64_t>( "col_by_f0_mod", 0.9 );
  p.set<float64_t>( "col_by_f0_nyq_mult", SIGMA_MULT ); 

  if( false == p.get<bool>( "col_sep_rgby" ) )
    {
      p.set<int64_t>( "col_by_ncenters", GETINT(col_rg_ncenters) );
      p.set<int64_t>( "col_by_cstart", GETINT(col_rg_cstart) );
      p.set<int64_t>( "col_by_nsurr", GETINT(col_rg_nsurr) );
      p.set<int64_t>( "col_by_soffset", GETINT(col_rg_soffset) );
      
      p.set<float64_t>( "col_by_f0_cyc_dva", GETFLT(col_rg_f0_cyc_dva) );
      p.set<float64_t>( "col_by_f0_mod", GETFLT(col_rg_f0_mod) );
      p.set<float64_t>( "col_by_f0_nyq_mult", GETFLT(col_rg_f0_nyq_mult) );
    }
  
  
  
  
  
  
  
  
  


  
  
  p.set<float64_t>( "output_dva_per_pix", 0.75 ); 
  
  
  
  p.set<bool>( "norm_final_output", true );
  
  
  p.set<int64_t>( "DOG_niter", 4 );
  
  
  p.set<float64_t>( "DOG_cdva", 1.0 ); 
  p.set<float64_t>( "DOG_sdva", 8.0 ); 
  p.set<float64_t>( "DOG_constinh", 0.02 );
  p.set<float64_t>( "DOG_cex", 0.5 );
  p.set<float64_t>( "DOG_cinh", 1.5 );
  
  
  p.set<std::string>( "normtype", "DOG" );


  p.set<std::string>( "color_oppon_method", "walther2006");
  p.set<std::string>( "lum_extract_algo", "extract_luminance_equal" );
  p.set<std::string>( "orient_extract_algo", "orient_gabor" );

  
  p.set<float64_t>( "resample_grace", 0.001 );

  p.set<bool>("do_orihpf", true );
  
  return p;
} 




param_set salmap_rv::itti_formal_1998_params( const int wid, const int fps )
{
  param_set p = itti_formal_default_params();
  
  
  float64_t arbitrary_dva_per_pix = 40.0 / 640; 
  SETFLT(input_dva_per_pix, arbitrary_dva_per_pix);
  
  
  
  
  
  

  
  
  
  
  
  float64_t lumsigmapix = 0.85; 
  float64_t orisigmapix = 1.4345; 

  

  
  
  float64_t lum_f0_dva, ori_f0_dva;
  
  freq_modul_from_sigma( lumsigmapix*arbitrary_dva_per_pix, 0.9, lum_f0_dva );
  freq_modul_from_sigma( orisigmapix*arbitrary_dva_per_pix, 0.9, ori_f0_dva );
  fprintf(stdout, "Setting lumf0dva to %lf\n", lum_f0_dva );
  SETFLT( lum_f0_cyc_dva, lum_f0_dva );
  
  
  
  
  
  
  
  
  float64_t targ_resample_ratio=0.5; 
  float64_t currnyqf = nyqf_from_dvaperpix( arbitrary_dva_per_pix );
  float64_t targnyqf = targ_resample_ratio * currnyqf;
  float64_t lumnyqfmult = targnyqf / lum_f0_dva;
  fprintf(stdout, "Lum nyqf mult is %lf\n", lumnyqfmult);
  
  float64_t lowpass3_nyqmult=3; 
  float64_t lowpass5_nyqmult=3; 
  float64_t lowpass79_nyqmult=3; 
  SETFLT( lum_f0_nyq_mult, lowpass3_nyqmult ); 


  SETFLT( ori_hpf_f0_cyc_dva, ori_f0_dva );
  SETFLT( ori_hpf_f0_nyq_mult, lowpass79_nyqmult ); 
  
  SETFLT( ori_gabor_f0_bandwidth_octaves, 0.94 ); 
  
  
  
  
  float64_t orispatfreqpix = 2.6 / (2*M_PI);

  
  SETFLT( ori_gabor_f0_cyc_dva, orispatfreqpix*(1/arbitrary_dva_per_pix) );
  
  
  SETFLT( col_rg_f0_cyc_dva, lum_f0_dva );
  SETFLT( col_rg_f0_nyq_mult, lowpass3_nyqmult );
  
  SETFLT( mot_f0_cyc_dva, lum_f0_dva );
  SETFLT( mot_f0_nyq_mult, lowpass3_nyqmult );
  SETFLT( "mot_vel_dva_sec", arbitrary_dva_per_pix * fps ); 
  SETINT( mot_n_dir, 4 );
  
  
  
  
  SETSTR(normtype,"DOG");
  
  
  
  SETFLT( output_dva_per_pix, 1.0 );
  

  
  if( false == p.get<bool>( "col_sep_rgby" ) )
    {
      p.set<int64_t>( "col_by_ncenters", GETINT(col_rg_ncenters) );
      p.set<int64_t>( "col_by_cstart", GETINT(col_rg_cstart) );
      p.set<int64_t>( "col_by_nsurr", GETINT(col_rg_nsurr) );
      p.set<int64_t>( "col_by_soffset", GETINT(col_rg_soffset) );
      
      p.set<float64_t>( "col_by_f0_cyc_dva", GETFLT(col_rg_f0_cyc_dva) );
      p.set<float64_t>( "col_by_f0_mod", GETFLT(col_rg_f0_mod) );
      p.set<float64_t>( "col_by_f0_nyq_mult", GETFLT(col_rg_f0_nyq_mult) );
    }

  p.set<std::string>( "color_oppon_method", "itti1998");
  p.set<std::string>( "lum_extract_algo", "extract_luminance_equal" );
  p.set<std::string>( "orient_extract_algo", "orient_steerable" );

  SETFLT( w_ori, 1 );
  SETFLT( w_col, 1 );
  SETFLT( w_lum, 1 );
  SETFLT( w_mot, 1 );

  
  return p;
} 
















 

grouped_input INPUT( const std::string& inputname, const std::string& mapname, const std::string& timetype, const int64_t timeval, const std::string& nickname )
{
  
  std::string val = mapname + "@" + timetype + "@" + std::to_string(timeval) + "@" + nickname;
  std::string name = inputname;
  grouped_input gi( name, val );
  return gi;
}

grouped_input INPUTNOW( const std::string& inputname, const std::string& mapname, const std::string& nickname )
{
  return INPUT( inputname, mapname, "RELTIME", 0, nickname );
}

grouped_input INPUTREL( const std::string& inputname, const std::string& mapname, const std::string& nickname, const int64_t& tdelta )
{
  return INPUT( inputname, mapname, "RELTIME", tdelta, nickname );
}




 


template <typename T>
grouped_param PARAM( const std::string& inputname, const T& val )
{
  std::stringstream ss;
  ss << val;
  grouped_param gp( inputname, ss.str() );
  return gp;
}


template <typename T>
grouped_param PARAM( const std::string& inputname, const T& val, const param_group pg1 )
{
  std::stringstream ss;
  ss << val;
  grouped_param gp( inputname, ss.str(), pg1 );

  fprintf(stdout, "Created PARAM [%s]\n", gp.tostr().c_str() );
  return gp;
}

























































































 




struct inputlist
{
  std::vector<grouped_input> vec;
  
  void add( const std::string& inputname, const std::string& mapname, const std::string& timetype, const int64_t& timeval, const std::string& filtnick )
  {
    
    
    vec.push_back( INPUT( inputname, mapname, timetype, timeval, filtnick ) );
  }
  
  void addnow( const std::string& inputname, const std::string& mapname, const std::string& filtnick )
  {
    
    
    
    vec.push_back( INPUTNOW( inputname, mapname, filtnick ) );
  }
};

struct paramlist
{
  std::vector<grouped_param> vec;

  template <typename T>
  void add( const std::string& paramname, const T& val )
  {
    
    vec.push_back( PARAM<T>( paramname, val ) );
  }

  template <typename T>
  void add( const std::string& paramname, const T& val, const std::string& groupname )
  {
    
    vec.push_back( PARAM<T>( paramname, val, param_group(groupname) ) );
  }
};











void salmap_rv::make_itti_dyadic_cpu_weighted_formal(  const param_set& p, SalMap& mysalmap, std::shared_ptr<ThreadPool> _tp=nullptr )
{
  
  mysalmap.init( p, _tp );
  
  
  mysalmap.add_sal_filter( "extract_rgb_channels", "bgr", "Raw BGR", INPUTNOW("bgrin", "bgr", "rawinput" ) );
  float64_t output_dva_per_pix = GETFLT(output_dva_per_pix);
  float64_t output_nyquist_dva = nyqf_from_dvaperpix( output_dva_per_pix );
  
  
  mysalmap.add_sal_filter( GETSTR(lum_extract_algo), "lum0", "Raw Luminance", INPUTNOW("rin", "r", "bgr"), INPUTNOW("gin", "g", "bgr"), INPUTNOW("bin", "b", "bgr"));
  
  
  int64_t niterations = GETINT(DOG_niter);
  double centerdva = GETFLT(DOG_cdva);
  double surrdva = GETFLT(DOG_sdva);
  double constinhib = GETFLT( DOG_constinh);
  double cex = GETFLT( DOG_cex );
  double cinh = GETFLT( DOG_cinh );
  
  std::string normtype = GETSTR(normtype);
  std::string outputmapname = "output";
  
  double nmin = 0;
  double nmax = 1;

  
  
  
  
  size_t lum_nlev = -1 +
    GETINT(lum_cstart) +
    GETINT(lum_ncenters) +
    GETINT(lum_soffset) +
    GETINT(lum_nsurr) ;
  
  size_t lum_maxcenter = GETINT(lum_cstart) + GETINT(lum_ncenters) - 1;
  
  std::vector<float64_t> lum_sigreps(lum_nlev);
  std::vector<float64_t> lum_sigdvas(lum_nlev);
  std::vector<float64_t> lum_sigfs(lum_nlev);

   

  
  
  
  sigma_from_freq_modul( GETFLT(lum_f0_cyc_dva), GETFLT(lum_f0_mod),
			 lum_sigdvas[0], lum_sigfs[0] );
  
  lum_sigreps[0] = lum_sigdvas[0]; 
  
  
  
  
  for( size_t l=1; l<lum_nlev; ++l )
    {
      lum_sigfs[l] = lum_sigfs[l-1]/2; 
      
      
      lum_sigreps[l] = sigma_from_sigmaf(lum_sigfs[l]); 
      lum_sigdvas[l] = lum_sigreps[l];
      
      
    }

  
  for( size_t l=0; l<lum_nlev; ++l )
    {
      std::string newname = "lum-lpf" + tostring(l);
      std::string newdesc = "Luminance Map LPF " + tostring(l);
      std::string inname = "lum" + tostring(l);
      
      mysalmap.add_sal_filter( "lpf_gauss_sigma", newname, newdesc,
			       INPUTNOW("input", "output", inname ),
			       PARAM<float64_t>("sigma_dva",lum_sigdvas[l]) );
      
    }

  for( size_t l=1; l<lum_nlev; ++l )
    {
      std::string newname1 = "lum" + tostring(l);
      std::string newdesc1 = "Luminance Map " + tostring(l);
      std::string inname1 = "lum-lpf" + tostring(l-1);
      
      
      float64_t targnyquist = lum_sigfs[l-1]*GETFLT(lum_f0_nyq_mult);
      mysalmap.add_sal_filter( "downsample_minnyquist", newname1, newdesc1, INPUTNOW("input","output",inname1), PARAM<float64_t>( "targ_nyquist_dva", targnyquist ),
			       PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );
      
    }
  
  inputlist lumlpfinputfilters;

  double lumattn = GETFLT( lum_diff_edge_attn );
    
  for( size_t c=GETINT(lum_cstart); c<=lum_maxcenter; ++c )
    {
      
      size_t mymaxsurr = c+GETINT(lum_soffset)+GETINT(lum_nsurr)-1;
      for(size_t s=c+GETINT(lum_soffset); s<=mymaxsurr; ++s)
	{
	  
	  
	  
	  
	  std::string diffname = tostring(c) + "-" + tostring(s);
	  std::string newname = "lum" + diffname;
	  std::string newdesc = "LUM " + diffname;
	  std::string inname1 = "lum" + tostring(c);
	  std::string inname2 = "lum" + tostring(s);
	  
	  mysalmap.add_sal_filter( "edge_attenuated_absolute_diff", newname, newdesc, INPUTNOW("input1", "output", inname1), INPUTNOW("input2", "output", inname2), PARAM<float64_t>("attenuation_width_proportion", lumattn) );

	  
	  std::string dnewname = "d" + newname;
	  std::string dnewdesc = "DEC " + newdesc;
	  std::string dinname = newname;
	  mysalmap.add_sal_filter( "resample_minnyquist", dnewname, dnewdesc, INPUTNOW("input","output",dinname), PARAM("targ_nyquist_dva", output_nyquist_dva), PARAM("downsamp_nyquist_mult", GETFLT(lum_f0_nyq_mult) ), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );

	  
	  std::string ndnewname = "n" + dnewname;
	  std::string ndnewdesc = "Normed " + dnewdesc;
	  std::string ndinname = dnewname;
	  if( same_string( normtype, "CONST" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_constant_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("min",nmin), PARAM("max",nmax) );
	    }
	  else if( same_string( normtype, "LMAX" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_localmax_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("min",nmin), PARAM("max",nmax) );
	    }
	  else if( same_string( normtype, "DOG" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_iterative_DOG", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	    }
	  else
	    {
	      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	      exit(1);
	    }

	  lumlpfinputfilters.addnow( "input", "output", ndnewname );
	  
	}
    }
    
  
  mysalmap.add_sal_filter("avg_n", "avg_nlumlpf", "Average Normed Luminances", lumlpfinputfilters.vec );
  
  
  if( same_string( normtype, "CONST" ) )
    {
      mysalmap.add_sal_filter( "normalize_constant_range", "navg_nlumlpf", "Normed Average Normed Luminances", INPUTNOW("input","output","avg_nlumlpf"),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "LMAX" ) )
    {
      mysalmap.add_sal_filter( "normalize_localmax_range", "navg_nlumlpf", "Normed Average Normed Luminances", INPUTNOW("input","output","avg_nlumlpf"),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "DOG" ) )
    {
      mysalmap.add_sal_filter( "normalize_iterative_DOG", "navg_nlumlpf", "Normed Average Normed Luminances", INPUTNOW("input","output","avg_nlumlpf"),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
    }
  else
    {
      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
      exit(1);
    }





  
  
  
  
  size_t ori_nlev = -1 +
    GETINT(ori_cstart) +
    GETINT(ori_ncenters) +
    GETINT(ori_soffset) +
    GETINT(ori_nsurr) ;
  
  size_t ori_maxcenter = GETINT(ori_cstart) + GETINT(ori_ncenters) - 1;

  std::vector<float64_t> ori_sigreps(ori_nlev);
  std::vector<float64_t> ori_sigdvas(ori_nlev);
  std::vector<float64_t> ori_sigfs(ori_nlev);

  

   

  
  sigma_from_freq_modul( GETFLT(ori_hpf_f0_cyc_dva), GETFLT(ori_hpf_f0_mod),
			 ori_sigdvas[0], ori_sigfs[0] );
  ori_sigreps[0] = ori_sigdvas[0]; 

  {
    
    
    
  }

  
  for( size_t l=1; l<ori_nlev; ++l )
    {
      ori_sigfs[l] = ori_sigfs[l-1]/2; 
      ori_sigreps[l] = sigma_from_sigmaf(ori_sigfs[l]);
      
      ori_sigdvas[l] = ori_sigreps[l]; 
      
      
    }
  
  
  
  for( size_t l=0; l<ori_nlev; ++l )
    {
      std::string mapname="output";
      std::string newname = "orilum-lpf" + tostring(l); 
      std::string newdesc = "Orientation Map LPF " + tostring(l);
      std::string inname = "orilum" + tostring(l); 
      if(l==0)
	{
	  inname="lum0";
	}
      
      mysalmap.add_sal_filter( "lpf_gauss_sigma", newname, newdesc, INPUTNOW("input", "output", inname ), PARAM<float64_t>("sigma_dva",ori_sigdvas[l]) );
    }

  for( size_t l=1; l<ori_nlev; ++l )
    {
      std::string mapname1="output";
      std::string newname1 = "orilum" + tostring(l); 
      std::string newdesc1 = "Orientation Map " + tostring(l);
      std::string inname1 = "orilum-lpf" + tostring(l-1);
      
      
      float64_t targnyquist = ori_sigfs[l-1]*GETFLT(ori_hpf_f0_nyq_mult); 
      
      mysalmap.add_sal_filter( "downsample_minnyquist", newname1, newdesc1, INPUTNOW("input","output",inname1), PARAM<float64_t>( "targ_nyquist_dva", targnyquist ), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );
    }

     

  
  
  if( true == GETBOOL(do_orihpf) )
    {
      for( size_t l=1; l<ori_nlev; ++l )
	{
	  
	  std::string mapnameh="output";
	  std::string newnameh = "orilum-hpf" + tostring(l);
	  std::string newdesch = "Orientation Map HPF " + tostring(l);
	  std::string innameh1 = "orilum" + tostring(l);
	  std::string innameh2 = "orilum-lpf" + tostring(l);
      
	  mysalmap.add_sal_filter( "diff", newnameh, newdesch, INPUTNOW("input1","output",innameh1), INPUTNOW("input2","output",innameh2) );
	} 
    }
     
  std::vector<float64_t> ori_f0s( ori_nlev );
  
  ori_f0s[0] = GETFLT(ori_gabor_f0_cyc_dva);
  
  


  double oriattn = GETFLT(ori_diff_edge_attn);

  
  
  for( size_t l=1; l<ori_nlev; ++l )
    {
      ori_f0s[l] = ori_f0s[l-1]/2;
      
    }

  int degperstep=180 / GETINT(ori_n_dir); 

  inputlist orient_all_filts;

  
  
  for(size_t diri=0; diri<GETINT(ori_n_dir); ++diri)
    {
      float64_t dir = diri * degperstep;
      std::string dirstr = tostring(dir, 0); 

      inputlist inputfilts;
      
      
      for( size_t l=GETINT(ori_cstart); l<ori_nlev; ++l )
	{
	  std::string mapname="output";
	  std::string newname="orient_ang" +  dirstr + "_" + tostring(l);
	  std::string newdesc="Orientation " +  dirstr + " deg " + tostring(l);

	  
	  std::string inname="orilum-lpf" + tostring(l);
	  if( true == GETBOOL(do_orihpf) ) 
	    {
	      inname="orilum-hpf" + tostring(l); 
	    }


	  
	  
	  
	  mysalmap.add_sal_filter( GETSTR(orient_extract_algo), newname, newdesc, INPUTNOW("input","output",inname),PARAM("f0_cyc_dva",ori_f0s[l]), PARAM("orient_theta_deg",dir), PARAM("bandwidth_octaves",GETFLT(ori_gabor_f0_bandwidth_octaves)), PARAM("prop_width_edge_atten",GETFLT(ori_gabor_prop_edge_atten)) );
	}
      
      
      
      for( size_t c=GETINT(ori_cstart); c<=ori_maxcenter; ++c )
	{
	  size_t mymaxsurr = c+GETINT(ori_soffset)+GETINT(ori_nsurr)-1;
	  for(size_t s=c+GETINT(ori_soffset); s<=mymaxsurr; ++s)
	    {
	      
	      std::string diffname = tostring(c) + "-" + tostring(s);
	      std::string mapname="output";
	      
	      std::string newname="orient_ang" + dirstr + "_" + diffname;
	      std::string newdesc="Orientation " + dirstr + " deg " + diffname;
	      std::string inname1="orient_ang" + dirstr + "_" + tostring(c);
	      std::string inname2="orient_ang" + dirstr + "_" + tostring(s);
	      
	      mysalmap.add_sal_filter( "edge_attenuated_rectified_diff",newname,newdesc,INPUTNOW("input1","output",inname1),INPUTNOW("input2","output",inname2),PARAM("attenuation_width_proportion",oriattn) );

	       
	      std::string dnewname="d" + newname;
	      std::string dnewdesc="Dec " + newdesc;
	      std::string dinname1=newname;

	      mysalmap.add_sal_filter( "resample_minnyquist",dnewname, dnewdesc,INPUTNOW("input","output",dinname1),PARAM("targ_nyquist_dva", output_nyquist_dva), PARAM("downsamp_nyquist_mult",GETFLT(ori_hpf_f0_nyq_mult) ), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );
	      
	      
	      
	      std::string ndnewname="n" + dnewname;
	      std::string ndnewdesc="Norm " + dnewdesc;
	      std::string ndinname1=dnewname;
	      if( same_string( normtype, "CONST" ) )
		{
		  mysalmap.add_sal_filter( "normalize_constant_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname1),PARAM("min",nmin), PARAM("max",nmax) );
		}
	      else if( same_string( normtype, "LMAX" ) )
		{
		  mysalmap.add_sal_filter( "normalize_localmax_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname1),PARAM("min",nmin), PARAM("max",nmax) );
		}
	      else if( same_string( normtype, "DOG" ) )
		{
		  mysalmap.add_sal_filter( "normalize_iterative_DOG", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname1),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
		}
	      else
		{
		  fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
		  exit(1);
		}
	      
	      inputfilts.addnow( "input","output",ndnewname );
	    } 
	} 
      
      std::string avgname = "avg_norient_" + dirstr;
      std::string avgdesc = "Average Normalized Orientation Deg: " + dirstr;
      
      mysalmap.add_sal_filter( "avg_n", avgname, avgdesc, inputfilts.vec );
      
      std::string navgname = "n" + avgname;
      std::string navgdesc = "Norm " + avgdesc;
      std::string ninname = avgname;
      std::string mapname="output";
      if( same_string( normtype, "CONST" ) )
	{
	  mysalmap.add_sal_filter( "normalize_constant_range", navgname, navgdesc, INPUTNOW("input","output",ninname),PARAM("min",nmin), PARAM("max",nmax) );
	}
      else if( same_string( normtype, "LMAX" ) )
	{
	  mysalmap.add_sal_filter( "normalize_localmax_range", navgname, navgdesc, INPUTNOW("input","output",ninname),PARAM("min",nmin), PARAM("max",nmax) );
	}
      else if( same_string( normtype, "DOG" ) )
	{
	  mysalmap.add_sal_filter( "normalize_iterative_DOG", navgname, navgdesc, INPUTNOW("input","output",ninname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	}
      else
	{
	  fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	  exit(1);
	}
      
      orient_all_filts.addnow( "input","output",navgname );
      
    } 

  std::string avg_oriallname="avg_orient_all";
  std::string avg_orialldesc="Average of Orientation Maps";
  
  mysalmap.add_sal_filter("avg_n", avg_oriallname, avg_orialldesc, orient_all_filts.vec );
  
  std::string navg_oriallname="n" + avg_oriallname;
  std::string navg_orialldesc="Norm " + avg_orialldesc;
  std::string navg_oriallinname = avg_oriallname;
  
  if( same_string( normtype, "CONST" ) )
    {
      mysalmap.add_sal_filter( "normalize_constant_range", navg_oriallname, navg_orialldesc, INPUTNOW("input","output",navg_oriallinname),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "LMAX" ) )
    {
      mysalmap.add_sal_filter( "normalize_localmax_range", navg_oriallname, navg_orialldesc, INPUTNOW("input","output",navg_oriallinname),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "DOG" ) )
    {
      mysalmap.add_sal_filter( "normalize_iterative_DOG", navg_oriallname, navg_orialldesc, INPUTNOW("input","output",navg_oriallinname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
    }
  else
    {
      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
      exit(1);
    }





  
  
  
  

  
  
  if( same_string( GETSTR(color_oppon_method), "walther2006" ) )
    {
      mysalmap.add_sal_filter( "rgby_opponency_walther2006", "rgbyopp", "RG-BY Opponency", INPUTNOW("rin","r","bgr"), INPUTNOW("gin","g","bgr"), INPUTNOW("bin","b","bgr"), INPUTNOW("lumin", "output", "lum0"), PARAM("lum_thresh", GETFLT(col_lum_thresh)) );
    }
  else if( same_string( GETSTR(color_oppon_method), "itti1998") )
    {
      mysalmap.add_sal_filter( "extract_rgby", "rgby", "Red-Green-Blue-Yellow", INPUTNOW("r","r","bgr"), INPUTNOW("g","g","bgr"), INPUTNOW("b","b","bgr") );

      mysalmap.add_sal_filter( "rgby_opponency", "rgbyopp", "RG-BY Opponency", INPUTNOW("rin","r","rgby"), INPUTNOW("gin","g","rgby"), INPUTNOW("bin","b","rgby"), INPUTNOW("yin","y","rgby"), INPUTNOW("lumin", "output", "lum0") , PARAM("lum_thresh", GETFLT(col_lum_thresh)));
    }
  else
    {
      fprintf(stderr, "Unrecognized color_oppon_method: [%s]\n", GETSTR(color_oppon_method).c_str() );
      exit(1);
    }
  
  
  
  
  
  
  
  
  
  
  

  
  size_t col_rg_nlev = -1 +
    GETINT(col_rg_cstart) +
    GETINT(col_rg_ncenters) +
    GETINT(col_rg_soffset) +
    GETINT(col_rg_nsurr);
  
  size_t col_rg_maxcenter = GETINT(col_rg_cstart) + GETINT(col_rg_ncenters) - 1;

  std::vector<float64_t> col_rg_sigreps(col_rg_nlev);
  std::vector<float64_t> col_rg_sigdvas(col_rg_nlev);
  std::vector<float64_t> col_rg_sigfs(col_rg_nlev);
  
  sigma_from_freq_modul( GETFLT(col_rg_f0_cyc_dva), GETFLT(col_rg_f0_mod),
			 col_rg_sigdvas[0], col_rg_sigfs[0] );

  col_rg_sigreps[0] = col_rg_sigdvas[0];
  
  
  for( size_t l=1; l<col_rg_nlev; ++l )
    {
      col_rg_sigfs[l] = col_rg_sigfs[l-1]/2;
      col_rg_sigreps[l] = sigma_from_sigmaf(col_rg_sigfs[l]);
      
      col_rg_sigdvas[l] = col_rg_sigreps[l];
      
    }
  
  
  
  std::string rgmapname="rg";
  mysalmap.add_sal_filter( "lpf_gauss_sigma", "col_rg-lpf0", "COL RG Map 0", INPUTNOW( "input", "rg", "rgbyopp"), PARAM("sigma_dva",col_rg_sigdvas[0]));
	  
  for( size_t l=1; l<col_rg_nlev; ++l )
    {
      std::string mapname="output";
      std::string newname = "col_rg-lpf" + tostring(l);
      std::string newdesc = "COL R-G Map LPF " + tostring(l);
      std::string inname = "col_rg" + tostring(l);
      
      mysalmap.add_sal_filter( "lpf_gauss_sigma", newname, newdesc, INPUTNOW( "input", "output", inname), PARAM("sigma_dva",col_rg_sigdvas[l]));
    }


  double colattn = GETFLT( col_diff_edge_attn );
  
  for( size_t l=1; l<col_rg_nlev; ++l )
    {
      std::string mapname1="output";
      std::string newname1 = "col_rg" + tostring(l);
      std::string newdesc1 = "COL R-G Map " + tostring(l);
      std::string inname1 = "col_rg-lpf" + tostring(l-1);
      
      float64_t targnyquist = col_rg_sigfs[l-1]*GETFLT(col_rg_f0_nyq_mult); 
      
      mysalmap.add_sal_filter( "downsample_minnyquist", newname1, newdesc1, INPUTNOW("input","output",inname1), PARAM("targ_nyquist_dva",targnyquist), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );
    }

  inputlist rglpfinputfilts;
  
  for( size_t c=GETINT(col_rg_cstart); c<=col_rg_maxcenter; ++c )
    {
      size_t mymaxsurr = c+GETINT(col_rg_soffset)+GETINT(col_rg_nsurr)-1;
      for(size_t s=c+GETINT(col_rg_soffset); s<=mymaxsurr; ++s)
	{
	  std::string mapname="output";
	  

	  
	  std::string diffname = tostring(c) + "-" + tostring(s);
	  std::string newname = "col_rg" + diffname;
	  std::string newdesc = "RG " + diffname;
	  std::string inname1 = "col_rg" + tostring(c);
	  std::string inname2 = "col_rg" + tostring(s);
	  
	  mysalmap.add_sal_filter( "edge_attenuated_absolute_diff", newname, newdesc, INPUTNOW("input1","output",inname1), INPUTNOW("input2","output",inname2), PARAM("attenuation_width_proportion", colattn ) );

	  
	  std::string dnewname = "d" + newname;
	  std::string dnewdesc = "DEC " + newdesc;
	  std::string dinname = newname;
	  
	  mysalmap.add_sal_filter( "resample_minnyquist", dnewname, dnewdesc, INPUTNOW("input","output",dinname), PARAM("targ_nyquist_dva",output_nyquist_dva),PARAM("downsamp_nyquist_mult",GETFLT(col_rg_f0_nyq_mult) ), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );

	  
	  std::string ndnewname = "n" + dnewname;
	  std::string ndnewdesc = "Normed " + dnewdesc;
	  std::string ndinname = dnewname;

	  if( same_string( normtype, "CONST" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_constant_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("min",nmin), PARAM("max",nmax) );
	    }
	  else if( same_string( normtype, "LMAX" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_localmax_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("min",nmin), PARAM("max",nmax) );
	    }
	  else if( same_string( normtype, "DOG" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_iterative_DOG", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	    }
	  else
	    {
	      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	      exit(1);
	    }
	  
	  
	  rglpfinputfilts.addnow( "input","output",ndnewname );
	} 
    } 

  
  mysalmap.add_sal_filter( "avg_n", "avg_nrglpf", "Average Normed R-Gs", rglpfinputfilts.vec  );

  
  if( same_string( normtype, "CONST" ) )
    {
      mysalmap.add_sal_filter( "normalize_constant_range", "navg_nrglpf", "Normed Average Normed R-Gs", INPUTNOW("input","output","avg_nrglpf"),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "LMAX" ) )
    {
      mysalmap.add_sal_filter( "normalize_localmax_range", "navg_nrglpf", "Normed Average Normed R-Gs", INPUTNOW("input","output","avg_nrglpf"),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "DOG" ) )
    {
      mysalmap.add_sal_filter( "normalize_iterative_DOG", "navg_nrglpf", "Normed Average Normed R-Gs", INPUTNOW("input","output","avg_nrglpf"),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
    }
  else
    {
      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
      exit(1);
    }


   
   
   

   size_t col_by_nlev = -1 +
    GETINT(col_by_cstart) +
    GETINT(col_by_ncenters) +
    GETINT(col_by_soffset) +
    GETINT(col_by_nsurr);
  
  size_t col_by_maxcenter = GETINT(col_by_cstart) + GETINT(col_by_ncenters) - 1;

  std::vector<float64_t> col_by_sigreps(col_by_nlev);
  std::vector<float64_t> col_by_sigdvas(col_by_nlev);
  std::vector<float64_t> col_by_sigfs(col_by_nlev);
  
  sigma_from_freq_modul( GETFLT(col_by_f0_cyc_dva), GETFLT(col_by_f0_mod),
			 col_by_sigdvas[0], col_by_sigfs[0] );
  col_by_sigreps[0] = col_by_sigdvas[0];
  
  
  for( size_t l=1; l<col_by_nlev; ++l )
    {
      col_by_sigfs[l] = col_by_sigfs[l-1]/2;
      col_by_sigreps[l] = sigma_from_sigmaf(col_by_sigfs[l]);
      
      col_by_sigdvas[l] = col_by_sigreps[l];
      
    }
  
  
  
  std::string bymapname="by";
  mysalmap.add_sal_filter( "lpf_gauss_sigma", "col_by-lpf0", "COL BY Map 0", INPUTNOW( "input", "by", "rgbyopp"), PARAM("sigma_dva",col_by_sigdvas[0]));
  
  for( size_t l=1; l<col_by_nlev; ++l )
    {
      
      std::string mapname="output";
      std::string newname = "col_by-lpf" + tostring(l);
      std::string newdesc = "COL B-Y Map LPF " + tostring(l);
      std::string inname = "col_by" + tostring(l);
      
      mysalmap.add_sal_filter( "lpf_gauss_sigma", newname, newdesc, INPUTNOW( "input", "output", inname), PARAM("sigma_dva",col_by_sigdvas[l]));
    }

  for( size_t l=1; l<col_by_nlev; ++l )
    {
      std::string mapname1="output";
      std::string newname1 = "col_by" + tostring(l);
      std::string newdesc1 = "COL B-Y Map " + tostring(l);
      std::string inname1 = "col_by-lpf" + tostring(l-1);
      
      float64_t targnyquist = col_by_sigfs[l-1]*GETFLT(col_by_f0_nyq_mult); 
      mysalmap.add_sal_filter( "downsample_minnyquist", newname1, newdesc1, INPUTNOW("input","output",inname1), PARAM("targ_nyquist_dva",targnyquist), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );
    }

  inputlist bylpfinputfilts;
  
  for( size_t c=GETINT(col_by_cstart); c<=col_by_maxcenter; ++c )
    {
      size_t mymaxsurr = c+GETINT(col_by_soffset)+GETINT(col_by_nsurr)-1;
      for(size_t s=c+GETINT(col_by_soffset); s<=mymaxsurr; ++s)
	{
	  std::string mapname="output";
	  

	  
	  std::string diffname = tostring(c) + "-" + tostring(s);
	  std::string newname = "col_by" + diffname;
	  std::string newdesc = "BY " + diffname;
	  std::string inname1 = "col_by" + tostring(c);
	  std::string inname2 = "col_by" + tostring(s);
	  
	  mysalmap.add_sal_filter( "edge_attenuated_absolute_diff", newname, newdesc, INPUTNOW("input1","output",inname1), INPUTNOW("input2","output",inname2), PARAM("attenuation_width_proportion",colattn) );

	  
	  std::string dnewname = "d" + newname;
	  std::string dnewdesc = "DEC " + newdesc;
	  std::string dinname = newname;
	  
	  mysalmap.add_sal_filter( "resample_minnyquist", dnewname, dnewdesc, INPUTNOW("input","output",dinname), PARAM("targ_nyquist_dva",output_nyquist_dva),PARAM("downsamp_nyquist_mult",GETFLT(col_by_f0_nyq_mult) ), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );

	  
	  std::string ndnewname = "n" + dnewname;
	  std::string ndnewdesc = "Normed " + dnewdesc;
	  std::string ndinname = dnewname;

	  if( same_string( normtype, "CONST" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_constant_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("min",nmin), PARAM("max",nmax) );
	    }
	  else if( same_string( normtype, "LMAX" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_localmax_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("min",nmin), PARAM("max",nmax) );
	    }
	  else if( same_string( normtype, "DOG" ) )
	    {
	      mysalmap.add_sal_filter( "normalize_iterative_DOG", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	    }
	  else
	    {
	      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	      exit(1);
	    }
	  
	  
	  bylpfinputfilts.addnow( "input","output",ndnewname );
	} 
    } 


  
  

  
  mysalmap.add_sal_filter( "avg_n", "avg_nbylpf", "Average Normed B-Ys", bylpfinputfilts.vec );

  
   if( same_string( normtype, "CONST" ) )
    {
      mysalmap.add_sal_filter( "normalize_constant_range", "navg_nbylpf", "Normed Average Normed B-Ys", INPUTNOW("input","output","avg_nbylpf"),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "LMAX" ) )
    {
      mysalmap.add_sal_filter( "normalize_localmax_range", "navg_nbylpf", "Normed Average Normed B-Ys", INPUTNOW("input","output","avg_nbylpf"),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "DOG" ) )
    {
      mysalmap.add_sal_filter( "normalize_iterative_DOG", "navg_nbylpf", "Normed Average Normed B-Ys", INPUTNOW("input","output","avg_nbylpf"),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
    }
  else
    {
      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
      exit(1);
    }


   
   
   
   
  
   
   if( false == GETBOOL(col_sep_rgby) )
     {
       inputlist cols;

       
       cols.addnow( "input","output", "navg_nbylpf" );
       cols.addnow( "input","output", "navg_nrglpf" );

       
       mysalmap.add_sal_filter( "avg_n", "avg_col", "Average Combined Color", cols.vec );
       
       if( same_string( normtype, "CONST" ) )
	 {
	   mysalmap.add_sal_filter( "normalize_constant_range", "navg_col", "Normed Average Combined Colors", INPUTNOW("input","output","avg_col"),PARAM("min",nmin), PARAM("max",nmax) );
	 }
       else if( same_string( normtype, "LMAX" ) )
	 {
	   mysalmap.add_sal_filter( "normalize_localmax_range", "navg_col", "Normed Average Combined Colors", INPUTNOW("input","output","avg_col"),PARAM("min",nmin), PARAM("max",nmax) );
	 }
       else if( same_string( normtype, "DOG" ) )
	 {
	   mysalmap.add_sal_filter( "normalize_iterative_DOG", "navg_col", "Normed Average Combined Colors", INPUTNOW("input","output","avg_col"),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	 }
       else
	 {
	   fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	   exit(1);
	 }

       
     } 





   
   
  
  
  


  
  size_t mot_nlev = -1 +
  GETINT(mot_cstart) +
    GETINT(mot_ncenters) +
    GETINT(mot_soffset) +
    GETINT(mot_nsurr) ;
  
  size_t mot_maxcenter = GETINT(mot_cstart) + GETINT(mot_ncenters) - 1;

  std::vector<float64_t> mot_sigreps(mot_nlev);
  std::vector<float64_t> mot_sigdvas(mot_nlev);
  std::vector<float64_t> mot_sigfs(mot_nlev);
  std::vector<float64_t> mot_veldvas(mot_nlev);
  
  sigma_from_freq_modul( GETFLT(mot_f0_cyc_dva), GETFLT(mot_f0_mod),
			 mot_sigdvas[0], mot_sigfs[0] );

  mot_sigreps[0] = mot_sigdvas[0]; 
  mot_veldvas[0] = GETFLT(mot_vel_dva_sec);
  fprintf(stdout, "Got vel...: %lf\n", mot_veldvas[0]);

  {
    size_t l=0;
    
  }
  
  
  
  
  
  
  

  
  for( size_t l=1; l<mot_nlev; ++l )
    {
      
      
      mot_sigfs[l] = mot_sigfs[l-1]/2; 
      mot_sigreps[l] = sigma_from_sigmaf(mot_sigfs[l]); 
      
      mot_sigdvas[l] = mot_sigreps[l];

      
#define DOUBLE_REICHARDT_VEL
#ifdef DOUBLE_REICHARDT_VEL
      mot_veldvas[l] = mot_veldvas[l-1]*2;
#else
      mot_veldvas[l] = mot_veldvas[l-1];
#endif
      
      
      
    }

  
  for( size_t l=0; l<mot_nlev; ++l )
    {
      std::string mapname="output";
      std::string newname = "motlum-lpf" + tostring(l); 
      std::string newdesc = "Motion Map LPF " + tostring(l);
      std::string inname = "motlum" + tostring(l); 
      if(l==0)
	{
	  inname="lum0";
	}
      
      mysalmap.add_sal_filter( "lpf_gauss_sigma", newname, newdesc, INPUTNOW( "input", "output", inname), PARAM("sigma_dva",mot_sigdvas[l]));
    }

  for( size_t l=1; l<mot_nlev; ++l )
    {
      std::string mapname1="output";
      std::string newname1 = "motlum" + tostring(l); 
      std::string newdesc1 = "Motion Map " + tostring(l);
      std::string inname1 = "motlum-lpf" + tostring(l-1);
      
      
      float64_t targnyquist = mot_sigfs[l-1]*GETFLT(mot_f0_nyq_mult); 
      
      
      
      mysalmap.add_sal_filter( "downsample_minnyquist", newname1, newdesc1, INPUTNOW("input","output",inname1), PARAM("targ_nyquist_dva",targnyquist) , PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ));
    }
  
  int motdegperstep=360 / GETINT(mot_n_dir); 


  inputlist mot_all_filts;
  int64_t tdelta = -1 * mysalmap.get_dt_nsec();


  double motattn = GETFLT( mot_diff_edge_attn );
  
  for(size_t diri=0; diri<GETINT(mot_n_dir); ++diri)
    {
      float64_t dir = diri * motdegperstep;
      std::string dirstr = tostring(dir, 0);
      
      inputlist inputfilts;
      
      for( size_t l=GETINT(mot_cstart); l<mot_nlev; ++l )
	{
	  std::string mapname="output";
	  std::string newname="mot_ang" + dirstr + "_" + tostring(l);
	  std::string newdesc="Motion " + dirstr + " deg " + tostring(l);
	  std::string inname="motlum" + tostring(l);
	  
	  
	  mysalmap.add_sal_filter( "reichardt_motion_edge_atten", newname, newdesc, INPUTREL("currUnshift","output",inname,0), INPUTREL("prevUnshift","output",inname,tdelta), PARAM("motion_angle_deg",dir), PARAM("motion_speed_dva_per_sec",mot_veldvas[l]), PARAM("atten_size_prop", GETFLT(mot_atten_size_prop)), PARAM("mot_thresh", GETFLT(mot_thresh)) );
	}

      
      for( size_t c=GETINT(mot_cstart); c<=mot_maxcenter; ++c )
	{
	  size_t mymaxsurr = c+GETINT(mot_soffset)+GETINT(mot_nsurr)-1;
	  for(size_t s=c+GETINT(mot_soffset); s<=mymaxsurr; ++s)
	    {
	      
	      std::string diffname = tostring(c) + "-" + tostring(s);
	      std::string mapname="output";

	      std::string newname="mot_ang" + dirstr + "_" + diffname;
	      std::string newdesc="Motion " + dirstr + " deg " + diffname;
	      std::string inname1="mot_ang" + dirstr + "_" + tostring(c);
	      std::string inname2="mot_ang" + dirstr + "_" + tostring(s);
	      
	      mysalmap.add_sal_filter( "edge_attenuated_absolute_diff", newname, newdesc, INPUTNOW("input1","output",inname1), INPUTNOW("input2","output",inname2), PARAM("attenuation_width_proportion", motattn) );

	      
	      std::string dnewname="d" + newname;
	      std::string dnewdesc="Dec " + newdesc;
	      std::string dinname1=newname;

	      mysalmap.add_sal_filter( "resample_minnyquist", dnewname, dnewdesc, INPUTNOW("input","output",dinname1), PARAM("targ_nyquist_dva",output_nyquist_dva),PARAM("downsamp_nyquist_mult",GETFLT(mot_f0_nyq_mult) ), PARAM<float64_t>("resample_grace", GETFLT(resample_grace) ) );
	      
	      std::string ndnewname="n" + dnewname;
	      std::string ndnewdesc="Norm " + dnewdesc;
	      std::string ndinname1=dnewname;

	      if( same_string( normtype, "CONST" ) )
		{
		  mysalmap.add_sal_filter( "normalize_constant_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname1),PARAM("min",nmin), PARAM("max",nmax) );
		}
	      else if( same_string( normtype, "LMAX" ) )
		{
		  mysalmap.add_sal_filter( "normalize_localmax_range", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname1),PARAM("min",nmin), PARAM("max",nmax) );
		}
	      else if( same_string( normtype, "DOG" ) )
		{
		  mysalmap.add_sal_filter( "normalize_iterative_DOG", ndnewname, ndnewdesc, INPUTNOW("input","output",ndinname1),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
		}
	      else
		{
		  fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
		  exit(1);
		}
	      
	      inputfilts.addnow("input","output",ndnewname);
	    }
	}
      
      std::string avgname = "avg_nmot_" + dirstr;
      std::string avgdesc = "Average Normalized Motion Deg: " + dirstr;
      
      mysalmap.add_sal_filter( "avg_n", avgname, avgdesc, inputfilts.vec );
      
      std::string navgname = "n" + avgname;
      std::string navgdesc = "Norm " + avgdesc;
      std::string ninname = avgname;
      
      if( same_string( normtype, "CONST" ) )
	{
	  mysalmap.add_sal_filter( "normalize_constant_range", navgname, navgdesc, INPUTNOW("input","output",ninname),PARAM("min",nmin), PARAM("max",nmax) );
	}
      else if( same_string( normtype, "LMAX" ) )
	{
	  mysalmap.add_sal_filter( "normalize_localmax_range", navgname, navgdesc, INPUTNOW("input","output",ninname),PARAM("min",nmin), PARAM("max",nmax) );
	}
      else if( same_string( normtype, "DOG" ) )
	{
	  mysalmap.add_sal_filter( "normalize_iterative_DOG", navgname, navgdesc, INPUTNOW("input","output",ninname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	}
      else
	{
	  fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	  exit(1);
	}

      mot_all_filts.addnow("input","output",navgname);

    } 

   
  std::string avg_motallname="avg_mot_all";
  std::string avg_motalldesc="Average of Motion Maps";
  mysalmap.add_sal_filter("avg_n", avg_motallname, avg_motalldesc, mot_all_filts.vec );
  
  std::string navg_motallname="n" + avg_motallname;
  std::string navg_motalldesc="Norm " + avg_motalldesc;
  std::string navg_motallinname = avg_motallname;
  
  if( same_string( normtype, "CONST" ) )
    {
      mysalmap.add_sal_filter( "normalize_constant_range", navg_motallname, navg_motalldesc, INPUTNOW("input","output",navg_motallinname),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "LMAX" ) )
    {
      mysalmap.add_sal_filter( "normalize_localmax_range", navg_motallname, navg_motalldesc, INPUTNOW("input","output",navg_motallinname),PARAM("min",nmin), PARAM("max",nmax) );
    }
  else if( same_string( normtype, "DOG" ) )
    {
      mysalmap.add_sal_filter( "normalize_iterative_DOG", navg_motallname, navg_motalldesc, INPUTNOW("input","output",navg_motallinname),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
    }
  else
    {
      fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
      exit(1);
    }


  
  
  
  
  

  
  

  
  
  

  
  
  
  
  
  
  
  
  paramlist weights;
  inputlist all_nfilts;
  
  all_nfilts.addnow( "navg_mot_all","output","navg_mot_all" );
  weights.add<float64_t>( "navg_mot_all", GETFLT(w_mot), "weight" );
  
  all_nfilts.addnow( "navg_orient_all", "output","navg_orient_all" );
  weights.add<float64_t>( "navg_orient_all", GETFLT(w_ori), "weight" );
  
  all_nfilts.addnow("navg_nlumlpf", "output", "navg_nlumlpf" ) ;
  weights.add<float64_t>( "navg_nlumlpf", GETFLT(w_lum), "weight" );

  if( true == GETBOOL(col_sep_rgby ) )
    {
      all_nfilts.addnow( "navg_nbylpf", "output","navg_nbylpf" );
      weights.add<float64_t>( "navg_nbylpf", GETFLT( w_col_rg ), "weight" );
      all_nfilts.addnow( "navg_nrglpf","output","navg_nrglpf" ) ;
      weights.add<float64_t>( "navg_nrglpf", GETFLT( w_col_by ), "weight" );
    }
  else
    {
      all_nfilts.addnow( "navg_col","output","navg_col" ) ;
      weights.add<float64_t>( "navg_col", GETFLT( w_col ), "weight" );
    }
  
  
  mysalmap.add_sal_filter( "weighted_avg_n", "finavg", "Instantaneous Total Saliency", all_nfilts.vec, weights.vec );

  if( GETBOOL(norm_final_output) )
    {
      std::string newname="rfinavg";
      std::string newdesc="Rescaled Instantaneous Total Saliency";
      if( same_string( normtype, "CONST" ) )
	{
	  mysalmap.add_sal_filter( "normalize_constant_range", newname, newdesc, INPUTNOW("input","output","finavg"),PARAM("min",nmin), PARAM("max",nmax) );
	}
      else if( same_string( normtype, "LMAX" ) )
	{
	  mysalmap.add_sal_filter( "normalize_localmax_range", newname, newdesc, INPUTNOW("input","output","finavg"),PARAM("min",nmin), PARAM("max",nmax) );
	}
      else if( same_string( normtype, "DOG" ) )
	{
	  mysalmap.add_sal_filter( "normalize_iterative_DOG", newname, newdesc, INPUTNOW("input","output","finavg"),PARAM("iterations",niterations), PARAM("center_dva",centerdva), PARAM("surround_dva",surrdva),PARAM("constinhib",constinhib),PARAM("cex",cex),PARAM("cinh",cinh) );
	}
      else
	{
	  fprintf(stderr, "REV: stderr in create itti dyadic: unknown NORMALIZATION type: [%s]\n", normtype.c_str() );
	  exit(1);
	}
    }

  
  
  

  

  float64_t fact = exp( tdelta / GETFLT(lpf_tau_nsec) );
  if( GETBOOL(norm_final_output) )
    {
      
      mysalmap.add_sal_filter("time_low_pass", "smooth_finavg", "Smoothed Total Saliency", INPUTREL("curr","output","rfinavg",0),
			      INPUTREL("prev","output","smooth_finavg",tdelta),
			      PARAM("time_const_nsec",GETFLT(lpf_tau_nsec)) );

      
      
    }
  else
    {
      mysalmap.add_sal_filter("time_low_pass", "smooth_finavg", "Smoothed Total Saliency", INPUTREL("curr","output","finavg",0),
			      INPUTREL("prev","output","smooth_finavg",tdelta),
			      PARAM("time_const_nsec",GETFLT(lpf_tau_nsec)) );
      
      
    }
  



  
  
  

  
  mysalmap.outputs.push_back("smooth_finavg");
  mysalmap.outputs.push_back("navg_nlumlpf");
  mysalmap.outputs.push_back("navg_orient_all");
  mysalmap.outputs.push_back("navg_mot_all");
  if( true == GETBOOL( col_sep_rgby) )
    {
      mysalmap.outputs.push_back("navg_nbylpf");
      mysalmap.outputs.push_back("navg_nrglpf");
    }
  else
    {
      mysalmap.outputs.push_back("navg_col");
    }

  
  mysalmap.build_dependencies(); 
  
} 
