#include <salmap_rv/include/util_functs.hpp>

#include <salmap_rv/include/featmap.hpp>
#include <salmap_rv/include/Timer.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featfilterimpl.hpp>
#include <salmap_rv/include/mapphrase.hpp>

#include <salmap_rv/include/featmapimplcollection.hpp>

using namespace salmap_rv;


bool salmap_rv::FAIL_EXIT_NANINF( const cv::Mat& m, const std::string tag, const bool doexit )
{
  cv::Mat test = cv::Mat( m != m );
  int nonzero = cv::countNonZero( test );
  if( nonzero > 0 )
    {
      std::cerr << m << std::endl;
      fprintf(stderr, "REV: Big whoops, Massive error HAVE A NAN/INF (tag [%s])\n", tag.c_str() );
      if( true == doexit )
	{
	  exit(1);
	}
      return true;
    }
  return false;
}

std::string salmap_rv::pretty_inputs( std::map<std::string, mapphrase>& m, const size_t& tablevel )
  {
    std::string mytab=maketab( tablevel );
    std::string s="";
    
    for (auto it=m.begin(); it!=m.end(); ++it)
      {
	s += mytab + "ARG: " + it->first + "\n";
	s += it->second.print_pretty( tablevel + 3 );
      }
    return s;
  }

  
std::string salmap_rv::pretty_params( std::map<std::string, std::string>& m, const size_t& tablevel )
  {
    std::string mytab=maketab( tablevel );
    std::string s="";
    
    for (auto it=m.begin(); it!=m.end(); ++it)
      {
	s += mytab + it->first + "=" + it->second + "\n";
      }
    return s;
  }



cv::Mat salmap_rv::zeroonenorm( const cv::Mat& m )
{
  cv::Mat n, o;
  
  if( m.size().width < 320 )
    {
      float scale=320/m.size().width;
      cv::resize( m, n, cv::Size(0, 0), scale, scale );
      cv::normalize( n, o, 0, 1, cv::NORM_MINMAX );
      return o;
    }
  else
    {
      cv::normalize( m, n, 0, 1, cv::NORM_MINMAX );
      return n;
    }
}




 

double salmap_rv::deg_to_rad( const double& deg )
{
  return ((deg * CV_PI) / 180.0); 
}






float salmap_rv::packaged_update( size_t threadidx, FeatFilterImpl* next_to_exec, std::shared_ptr<FeatMapImplSet>& mapimpls, const int64_t time_nsec, std::mutex& mux, std::condition_variable& cv, const size_t myidx, std::list<size_t>& finished_idxs, const FeatFilterImplSet& filtimpls, const FilterSet& filts
		      
#ifdef GPU
		      , std::vector<cv::cuda::Stream>& streams
#endif
		      )
{
  Timer t;

#ifdef GPU
  
  
  if( !streams[threadidx] )
    {
      DEBUGPRINTF(stdout, "It's default stream wtf!!!!\n");
      exit(1);
    }
#endif
  
  
  
  
  
  
  
  if(nullptr == next_to_exec)
    {
      fprintf(stderr, "Huh next to exec is null\n");
      exit(1);
    }
  else
    {
      DEBUGPRINTF(stdout, "Attempting to exec filter [%s]\n", next_to_exec->name.c_str() );
    }
#ifdef GPU
  bool suc = next_to_exec->update( mapimpls, time_nsec, filtimpls, filts, streams[threadidx] );
#else
  bool suc = next_to_exec->update( mapimpls, time_nsec, filtimpls, filts);
#endif
  
  
  
  Timer t2;
  {
    
    std::lock_guard<std::mutex> lk( mux );
    finished_idxs.push_back( myidx );
    
  }
  
  
  
  cv.notify_all(); 
  
  float ret = t.elapsed();
  if( false == suc )
    {
      ret = -ret;
    }
  return ret;
}

std::shared_ptr<FeatMapImplInst> salmap_rv::get_from_collection_or_fail( std::shared_ptr<FeatMapImplCollection>& col, const std::string& name, const int64_t& attime_nsec )
{
  if( !col )
    {
      fprintf(stderr, "REV: wtf ptr to collection is null...[%s]\n", name.c_str());
      exit(1);
    }
  
  DEBUGPRINTF(stdout, "Getting from collection! (name=[%s]   attime=[%ld])\n", name.c_str(), attime_nsec );
  std::shared_ptr<FeatMapImplInst> retptr;
  bool existed = col->getSetterFromMapImpl( name, attime_nsec, retptr );
  
  DEBUGPRINTF(stdout, "Got from collection! (name=[%s]   attime=[%ld])  (ptr=[%p])\n", name.c_str(), attime_nsec, retptr.get() );
  
  return retptr;
}























double salmap_rv::pixval_from_slope( const int x, const double islope )
{
  
  double ret = 0;
  if( islope < x )
    {
      ret = 1;
    }
  else
    {
      ret = (x+1)/(islope+1);
    }

  return ret;
  
  
  
   
}





void salmap_rv::attenuation_filter_cpu_wid( const float64_t& wid, cv::InputArray in, salmap_rv::FiltMat& out )
{
  if(in.empty())
    {
      fprintf(stderr, "Atten filter wid input empty...\n");
      exit(1);
    }
  float64_t w = wid;
  salmap_rv::FiltMat ones = salmap_rv::FiltMat::ones( in.size(), in.type() );
  
  ones.copyTo(out);
  if( w * 2 > in.size().width ) { w = in.size().width / 2; }
  if( w * 2 > in.size().height ) { w = in.size().height / 2; }

#ifdef NAIVE_ATTEN

  for( size_t x=0; x<w; ++x )
    {
      size_t right=out.size().width-(x+1);
      size_t bot=out.size().height-(x+1);
      

      float64_t val = pixval_from_slope( x, w );
      
      out.col(x) = out.col(x) * val;
      
      
      out.col(right) = out.col(right) * val;

      
      out.row(x) = out.row(x) * val;
      
      
      out.row(bot) = out.row(bot) * val;
    }

#else
  
  
  for( size_t x=0; x<w+1; ++x )
    {
      
      
      float64_t val = pixval_from_slope( x, w );
      DEBUGPRINTF(stdout, "X=%d   R=%3.2lf (wid=%lf)\n", x, val, w);
      out.col(x).setTo( cv::Scalar(val ) );
      ones.row(x).setTo( cv::Scalar( val ));
    }
  cv::multiply( out, ones, out );
  cv::flip( out, ones, -1 ); 
  cv::multiply( out, ones, out );
#endif
}

void salmap_rv::attenuation_filter_cpu_prop( const float64_t& prop, cv::InputArray in, salmap_rv::FiltMat& out )
{
  if(in.empty())
    {
      fprintf(stderr, "Atten filter prop input empty...\n");
      exit(1);
    }
  size_t s = in.size().width > in.size().height ? in.size().width : in.size().height;
  float64_t w = (s * prop); 
  
  attenuation_filter_cpu_wid( w, in, out );
}


void salmap_rv::attenuation_filter_cpu( const float64_t& prop, cv::InputArray in, salmap_rv::FiltMat&  out )
{
  return salmap_rv::attenuation_filter_cpu_prop( prop, in, out );
}





std::string salmap_rv::enum_params( std::map< std::string, std::string >& m )
{
  std::string SEP=",";
  std::string result="";
    
  size_t size=0;
  for (auto it=m.begin(); it!=m.end(); ++it)
    {
      if(size > 0)
	{
	  result += SEP;
	}
      result += it->first + "=" + it->second;
      ++size;
    }
  return(result);
}

 


std::string salmap_rv::map_from_filter_reltime( FeatFilter& f, const std::string& localname, const int64_t& offset )
{
  FeatMap m;
  m.filter = f;
  m.localname = localname;
  m.set_relative_time( offset );
  return m.toString();
}

std::string salmap_rv::map_from_filter_abstime( FeatFilter& f, const std::string& localname, const int64_t& offset )
{
  FeatMap m;
  m.filter = f;
  m.localname = localname;
  m.set_absolute_time( offset );
  return m.toString();
}


std::string salmap_rv::maketab ( const size_t& tablevel = 0 )
{
  std::string s="";
  for( size_t x=0; x<tablevel; ++x)
    {
      s+=" ";
    }
  return s;
}

std::string salmap_rv::print_str_str_map( const std::map<std::string, std::string>& m )
{
  std::string SEP=",";
  std::string result="";
  
  size_t size=0;
  for(  std::map<std::string, std::string>::const_iterator it=m.begin(); it!=m.end(); ++it )
    {
      if(size > 0)
	{
	  result += SEP;
	}
      result += it->first + "=" + it->second;
      ++size;
    }
  return(result);
}



bool salmap_rv::same_string( const std::string& a, const std::string& b )
{
  Timer t;
  bool res = ( 0 == a.compare(b) );
  return res;
}



std::string salmap_rv::type2str(int type)
{
  std::string r;
  
  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);
  
  switch ( depth )
    {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

  r += "C";
  r += (chans+'0');
  
  return r;
}

cv::Mat salmap_rv::non_maxima_suppression_cpu( cv::InputArray image, const float64_t& thresh )
{
  cv::Mat mask;
  int s=5;
  cv::Mat structelem = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(s,s) ); 
  float32_t irrelevant=-1;
  cv::Mat image2;
  image.copyTo(image2);
  cv::threshold( image2, image2, thresh, irrelevant, cv::THRESH_TOZERO );
  cv::dilate(image2, mask, structelem);
  cv::compare(image2, mask, mask, cv::CMP_GE);
  float32_t scale = 1.f/255;
  int dtype = CV_32F;
  cv::multiply( image2, mask, image2, scale, dtype );
  return image2; 
}



double salmap_rv::avg_nonzero_cpu( cv::InputArray image, int& nonzero )
{
  nonzero = cv::countNonZero( image );
  double mysum = cv::sum( image )[0];
  
  
  
  if( nonzero > 0 )
    {
      double avg = mysum/nonzero;
      
      return avg;
    }
  else
    {
      return 0;
    }
}


void salmap_rv::zero_rectify_cpu( cv::InputArray m, cv::OutputArray output )
{
  
  
  
  cv::max( m, 0, output );
}

float64_t salmap_rv::param_as_float64( const parammap& params, const std::string& paramname )
{
  auto iter = params.find( paramname );
  if( iter == params.end() )
    {
      fprintf(stderr, "Params as float64 %s not found\n", paramname.c_str() );
      exit(1);
    }
  std::stringstream ss( iter->second );
  float64_t f;
  ss >> f;
  return f;
}

float64_t salmap_rv::param_as_int64( const parammap& params, const std::string& paramname )
{
  auto iter = params.find( paramname );
  if( iter == params.end() )
    {
      fprintf(stderr, "Params as int64 %s not found\n", paramname.c_str() );
      exit(1);
    }
  std::stringstream ss( iter->second );
  int64_t f;
  ss >> f;
  return f;
}


size_t salmap_rv::size_in_mem_strstrmap( const std::map<std::string,std::string>& input )
{
  size_t s = 0;
  for( auto iter=input.begin(); iter!=input.end(); ++iter )
    {
      s += iter->first.size() + iter->second.size();
    }
  return s;
}

size_t salmap_rv::size_in_mem_strvect( const std::vector<std::string>& input )
{
  size_t s = 0;
  for(size_t x=0; x<input.size(); ++x )
    {
      s += input[x].size();
    }
  return s;
}



float64_t salmap_rv::nyqf_from_pixperdva( const double pixperdva )
{
  
  float64_t nyqf = pixperdva/2;
  return nyqf;
}

float64_t salmap_rv::nyqf_from_dvaperpix( const double dvaperpix )
{
  float64_t samplerate_pixperdva = 1.0/dvaperpix;
  float64_t nyqf = samplerate_pixperdva/2;
  return nyqf;
}

float64_t salmap_rv::roundf_sig( const float64_t arg, const int sigdec )
{
  int mult=1;
  for(size_t x=0; x<sigdec; ++x)
    {
      mult*=10;
    }

  float64_t partmult = mult*arg; 
  float64_t rounded = round(partmult); 

  return (rounded / mult); 
}



float64_t salmap_rv::_upsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				 int& retwid, int& rethei )
{
  if( inratio < 1 )
    {
      fprintf(stderr, "Attempting to upsize with ratio < 1...%lf\n", inratio );
      exit(1);
    }
  else if( inratio == 1 )
    {
      retwid = inw;
      rethei = inh;
    }
  else
    {
      float64_t targw = inw * inratio;
      float64_t targh = inh * inratio;
      int maxw = floor(targw);
      int maxh = floor(targh);
      float64_t minr = origwhrat - origwhrat*allowerr;
      float64_t maxr = origwhrat + origwhrat*allowerr;
      
      retwid = inw;
      rethei = inh;
  
      for( int twid=maxw; twid>inw; --twid )
	{
	  int mintest = ceil( twid/maxr );
	  int maxtest = floor( twid/minr );
	  
	  if( maxtest >= mintest )
	    {
	      bool gotone=false;
	      float64_t besterr = 10000; 
	      for( int t=mintest; t<maxtest+1; ++t )
		{
		  float64_t tmpr = (float64_t)twid / t;
		  if( tmpr > minr && tmpr < maxr && abs(tmpr - origwhrat) < besterr )
		    {
		      besterr = abs(tmpr - origwhrat);
		      retwid = twid;
		      rethei = t;
		      gotone = true;
		    }
		}
	      if( true == gotone )
		{
		  break;
		}
	    }
	}
    }
  float64_t raterr = abs((((float64_t)retwid/rethei) - origwhrat)/origwhrat); 
  return raterr;
}


float64_t salmap_rv::_downsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				   int& retwid, int& rethei )
{
  if( inratio > 1 )
    {
      fprintf(stderr, "Attempting to downsize with ratio > 1...%lf\n", inratio );
      exit(1);
    }
  else if( inratio == 1 )
    {
      retwid = inw;
      rethei = inh;
    }
  else
    {
      float64_t targw = inw * inratio;
      float64_t targh = inh * inratio;
      int minw = ceil(targw);
      int minh = ceil(targh);
      float64_t minr = origwhrat - origwhrat*allowerr;
      float64_t maxr = origwhrat + origwhrat*allowerr;
      
      retwid = inw;
      rethei = inh;
      
      
      for( int twid=minw; twid<inw; ++twid )
	{
	  int mintest = ceil( twid/maxr );
	  int maxtest = floor( twid/minr );
	  
	  if( maxtest >= mintest )
	    {
	      bool gotone=false;
	      float64_t besterr = 100000; 
	      for( int t=mintest; t<maxtest+1; ++t )
		{
		  
		  float64_t tmpr = (float64_t)twid / t;
		  
		  if( tmpr > minr && tmpr < maxr && abs(tmpr - origwhrat) < besterr )
		    {
		      besterr = abs(tmpr - origwhrat);
		      retwid = twid;
		      rethei = t;
		      gotone = true;
		    }
		}
	      if( true == gotone )
		{
		  break;
		}
	    }
	}
    }
  float64_t raterr = abs((((float64_t)retwid/rethei) - origwhrat)/origwhrat); 
  return raterr;
}


float64_t salmap_rv::downsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				  int& retwid, int& rethei )
{
  if( inw >= inh )
    {
      return _downsize_image_w_ratio( inw, inh, inratio, allowerr, origwhrat, retwid, rethei );
    }
  else
    {
      return _downsize_image_w_ratio( inh, inw, inratio, allowerr, 1/origwhrat, rethei, retwid );
    }
}

float64_t salmap_rv::upsize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				  int& retwid, int& rethei )
{
  if( inw >= inh )
    {
      return _upsize_image_w_ratio( inw, inh, inratio, allowerr, origwhrat, retwid, rethei );
    }
  else
    {
      return _upsize_image_w_ratio( inh, inw, inratio, allowerr, 1/origwhrat, rethei, retwid );
    }
}

float64_t salmap_rv::resize_image_w_ratio( const int inw, const int inh, const float64_t inratio, const float64_t allowerr, const float64_t origwhrat,
				int& retwid, int& rethei )
{
  if( inratio > 1 )
    {
      return upsize_image_w_ratio( inw, inh, inratio, allowerr, origwhrat, retwid, rethei );
    }
  else 
    {
      return downsize_image_w_ratio( inw, inh, inratio, allowerr, origwhrat, retwid, rethei );
    }
}




 

float64_t salmap_rv::dva_to_pix( const float64_t dva, const float64_t dva_per_pix )
{
  return dva * (1/dva_per_pix); 
}

float64_t salmap_rv::pix_to_dva( const float64_t pix, const float64_t dva_per_pix )
{
  return pix * dva_per_pix; 
}


float64_t salmap_rv::sigma_from_sigmaf( const float64_t sigmaf )
{
  return 1/(2*M_PI*sigmaf);
}

float64_t salmap_rv::sigmaf_from_sigma( const float64_t sigma )
{
  return 1/(2*M_PI*sigma);
}




void salmap_rv::sigma_from_freq_modul( const float64_t targf, const float64_t targmodul,
			    float64_t& retsig, float64_t& retsigf )
{
  if( targmodul >= 1 )
    {
      fprintf(stderr, "TARG MODUL can't be >= 1...will cause eror\n");
      exit(1);
    }
  retsigf = sqrt( -(targf*targf) / (2*log(targmodul) ) ); 
  retsig = sigma_from_sigmaf( retsigf ); 

#if DEBUGLEVEL > 5
  DEBUGPRINTF(stdout, "To attenuate frequency %lf (wavelength %lf) to %lf of maximum, sigma of gauss filter is %lf (sigmaf is %lf)\n", targf, 1/targf, targmodul, retsig, retsigf );
  float64_t sanityval = exp( -(targf*targf) / (2*retsig*retsigf) );
  DEBUGPRINTF(stdout, "Sanity check: value of gaussian of sigmaf=%lf at targfreq x=%lf is [%lf]\n", retsigf, targf, sanityval );
#endif
  
  return;
}



void salmap_rv::freq_modul_from_sigma( const float64_t targsig, const float64_t targmodul,
			    float64_t& rettargf )
{
  float64_t sigmaf = sigmaf_from_sigma( targsig );
  
  
  
  
  
  
  
  float64_t inside = -1 * (sigmaf*sigmaf) * 2*log(targmodul);
  rettargf = sqrt(inside);

  
  fprintf(stdout, "Target blur of %lf and modul %lf implies inside of %lf\n", targsig, targmodul, inside);
  if( inside <= 0 )
    {
      fprintf(stderr, "REV: in freq_modul_from_sigma, we have negative value we will take sqrt of...\n");
      exit(1);
    }
  float64_t tmpretsig, tmpretsigf;
  sigma_from_freq_modul( rettargf, targmodul, tmpretsig, tmpretsigf );
  
  fprintf(stdout, "Sanity check freq_modul_from_sigma: computed freq [%lf] if modulated [%lf], implies a sigmaf of [%lf] and thus sigma [%lf] (expect [%lf])\n", rettargf, targmodul, tmpretsigf, tmpretsig, targsig );
  
  return;
}

float64_t salmap_rv::bandwidth_comp( const float64_t sigma, const float64_t f0 )
{
  float64_t lambda = 1/f0;
  float64_t ln2 = log(2);
  float64_t top = (M_PI*sigma)/lambda + sqrt(ln2/2);
  float64_t bot = (M_PI*sigma)/lambda - sqrt(ln2/2);
  float64_t b = log2(top/bot);
  return b;
}


float64_t salmap_rv::sigma_from_bandwidth_lambda( const float64_t bandwidth_octaves, const float64_t lambda )
{
  float64_t c = pow(2, bandwidth_octaves);
  if( c <= 1.0 )
    {
      fprintf(stderr, "ERROR -- REV: sigma_from_bandwidth_lambda, c <= 1.0 [%lf] (probably because bandwidth is 0?)\n", c);
      exit(1);
    }
  float64_t sigma = lambda * 1/M_PI * sqrt( log(2)/2 ) * (c+1)/(c-1); 
  return sigma;
}

void salmap_rv::compute_octave( const float64_t fc,
		     float64_t& fmin, float64_t& fmax)
{
  fmin = fc / sqrt(2);
  fmax = fc * sqrt(2);
}

float64_t salmap_rv::f_mod_by_gabor( const float64_t f, const float64_t f0, const float64_t sigma)
{
  float64_t alpha = sqrt( 1/(2*sigma*sigma));
  float64_t f_f0 = f-f0;
  float64_t resp = sqrt( M_PI / (alpha*alpha) ) * exp( - (f_f0*f_f0) * ((M_PI*M_PI) / (alpha*alpha)) );
  return resp;
}





std::string salmap_rv::CONCATENATE_STR_ARRAY(const std::vector<std::string>& arr, const std::string& sep, const size_t& start )
{
  if(arr.size() > start )
    {
      std::string ret=arr[start];
      for(size_t a=start+1; a<arr.size(); ++a)
	{
	  ret += sep + arr[a];
	}
      return ret;
    }
  else
    {
      return "";
    }
}

std::vector<std::string> salmap_rv::tokenize_string(const std::string& src, const std::string& delim, bool include_empty_repeats)
{
  std::vector<std::string> retval;
  boost::char_separator<char> sep( delim.c_str() );
  boost::tokenizer<boost::char_separator<char>> tokens(src, sep);
  for(const auto& t : tokens)
    {
      retval.push_back( t );
    }
  return retval;
}






bool salmap_rv::issamestring( const std::string& a, const std::string& b )
{
  return (0 == a.compare(b));
}


std::string salmap_rv::print_time( const int64_t time, const size_t BUFSIZE )
{
  char buf[BUFSIZE]; 
  int printed = snprintf( buf, BUFSIZE, "%015ld", time );
  if( printed >= 0 && printed <= BUFSIZE ) 
    {
      std::string bufstr = std::string(buf);
      return bufstr;
    }
  else
    {
      fprintf(stderr, "Error in print_time (attempting to print [%ld] into [%ld] characters) (retvalue [%d])\n", time, BUFSIZE, printed );
      exit(1);
    }
}



cv::Mat salmap_rv::resize_wid( cv::InputArray frame, const size_t targwid, const int interp )
{
  double ratio = (double)frame.size().height / frame.size().width;
  size_t targhei = ratio * targwid; 
  cv::Size targsize(targwid, targhei);
  cv::Mat frame2;
  cv::resize( frame, frame2, targsize, 0, 0, interp );
  return frame2;
}

cv::Mat salmap_rv::resize_hei( cv::InputArray frame, const size_t targhei, const int interp )
{
  double ratio = (double)frame.size().height / frame.size().width;
  size_t targwid = targhei / ratio; 
  cv::Size targsize(targwid, targhei);
  cv::Mat frame2;
  cv::resize( frame, frame2, targsize, 0, 0, interp );
  return frame2;
}


cv::Mat salmap_rv::resize(  cv::InputArray frame, const cv::Size& newsize, const int interp )
{
  
  if( 0 == newsize.width && 0 == newsize.height )
    {
      cv::Mat frame2;
      frame.copyTo(frame2);
      return frame2;
    }
  
  if( 0 == newsize.width )
    {
      return resize_hei( frame, newsize.height, interp );
    }
  else if( 0 == newsize.height )
    {
      return resize_wid( frame, newsize.width, interp );
    }
  else
    {
      cv::Mat frame2;
      cv::resize( frame, frame2, newsize, 0, 0, interp );
      return frame2;
    }
}



void salmap_rv::add_name( const std::string& name, cv::Mat& m)
{
  const size_t minsize=300;
  if( m.size().width < minsize )
    {
      m = resize_wid( m, minsize, cv::INTER_AREA );
    }
  
  int width = m.size().width;
  int height= m.size().height;
  
  float scale = width/640.0;
  
  size_t yoff=40;
  cv::putText( m, name, cv::Point(10*scale, 2*yoff*scale), cv::FONT_HERSHEY_PLAIN, 4*scale, cv::Scalar(150,255,255) );
}

void salmap_rv::add_timing( const int64_t& msec, cv::Mat& m)
{
  const size_t minsize=300;
  if( m.size().width < minsize )
    {
      m = resize_wid( m, minsize, cv::INTER_AREA );
    }
  
  int width = m.size().width;
  int height= m.size().height;
  std::string toput = std::to_string( msec ) + " ms";
  float scale = width/640.0;
  size_t yoff=40;
  
  cv::putText( m, toput, cv::Point(10*scale, 1*yoff*scale), cv::FONT_HERSHEY_PLAIN, 3.5*scale, cv::Scalar(150,255,255) );
}


cv::Mat salmap_rv::apply_color( const cv::Mat& input )
{
  cv::Mat tmp;
  if( input.depth() != CV_8U )
    {
      input.convertTo( tmp, CV_8U, 255.f );
    }
  else
    {
      tmp = input;
    }
  cv::applyColorMap(tmp, tmp, cv::COLORMAP_JET);
  return tmp;
}


cv::Mat salmap_rv::overlay_salmap( const cv::Mat& orig, const cv::Mat& salmap, const double& salmapalpha, const int interp )
{
  if( orig.empty() )
    {
      return salmap;
    }
  cv::Mat resized;
  
  
  cv::resize( salmap, resized, orig.size(), 0, 0, interp );
  
  double beta = ( 1.0 - salmapalpha );
  cv::Mat dst;
  cv::addWeighted( orig, salmapalpha, resized, beta, 0.0, dst);
    
  return dst;
}



cv::Mat salmap_rv::make_pretty( const cv::Mat& input, const std::string& name, const int64_t time, const cv::Mat& raw, const float alpha, const int interp)
{
  cv::Mat ret;
  if( input.type() != CV_8UC1 )
    {
      input.convertTo(ret, CV_8UC1, 255.0);
    }
  else
    {
      input.copyTo(ret); 
    }
  
  
  ret = apply_color( ret );

  if( false == raw.empty() )
    {
      ret = overlay_salmap( raw, ret, alpha, interp );
    }

  if( time >= 0 )
    {
      add_timing(time, ret );
    }
  
  if( false == name.empty() )
    {
      add_name(name, ret);
    }
  
  return ret;
}


cv::Mat salmap_rv::make_bg( const cv::Size& size, const cv::Scalar& col )
{
  cv::Mat background( size, CV_8UC3, col );
  return background;
}


cv::Mat salmap_rv::embed_bg( const cv::Size& size, const cv::Scalar& col, const cv::Mat& input, const int offsetx, const int offsety )
{
  
  cv::Mat bg = make_bg(size, col);
  if( input.size().width > bg.size().width || input.size().height > bg.size().height )
    {
      fprintf(stderr, "REV: input, bg size is smaller than input\n");
      exit(1);
    }
    
  size_t bg_midh = size.width / 2;
  size_t bg_midv = size.height / 2;
  size_t left = bg_midh - (input.size().width/2) + offsetx;
  size_t top = bg_midv - (input.size().height/2) + offsety;
  
  input.copyTo( bg( cv::Rect( left, top, input.size().width, input.size().height ) ) );
  return bg;
}


void salmap_rv::draw_eye_pos(cv::Mat& mymat, const float& eyex, const float& eyey)
{
  
  cv::Scalar color(0,255,255);
  
  
  
  
  
  size_t wid = mymat.size().width;
  size_t hei = mymat.size().height;

  int radius = wid * 0.033333;
  int thickness= wid * 0.0033333 ; 
    
  int eyexpix = wid * eyex; 
  int eyeypix = hei * eyey;

  
  cv::circle( mymat, cv::Point( eyexpix, eyeypix ), radius, color, thickness );
}

int salmap_rv::ksize_Nsigma( const float64_t sigma, const float64_t nyqmult )
{
  return sigma*nyqmult*2 + 1; 
}

int salmap_rv::ksize_shiftlinear( const float64_t sigma )
{
  
  
  
  
  
  
  
  
  return (20.0/3)*sigma - (7/3.0);
}


int salmap_rv::ksize_compute( const float64_t sigma )
{
  int ksize=0;
#ifdef KSIZE_SHIFTLINEAR
  ksize = ksize_shiftlinear( sigma );
#else
  const float64_t nyqmult = 3; 
  ksize = ksize_Nsigma( sigma, nyqmult );
#endif

  
  if( 0 == ksize % 2 )
    {
      ksize = ksize + 1;
    }

  return ksize;
}






std::vector<cv::Size>  salmap_rv::recursive_downsample( const cv::Size& currsize, const cv::Size& targsize, const double min_perstep_ratio, const double max_perstep_ratio,  const double orig_aspect_wh, const double allowederr )
{
  std::vector<cv::Size> recur_sizes;
  recur_sizes.push_back( currsize );

  
  if( currsize == targsize )
    {
      return recur_sizes;
    }
  
  cv::Size intermediate_sz = currsize;
    
  do
    {
      
      double to_target_ratio = ((float64_t)targsize.width / intermediate_sz.width);
      double nextratio = to_target_ratio < min_perstep_ratio ? min_perstep_ratio :  to_target_ratio;
      
      
      int retwidpix, retheipix;
      float64_t reterr = resize_image_w_ratio( intermediate_sz.width, intermediate_sz.height, nextratio, allowederr, orig_aspect_wh,
					       retwidpix, retheipix);
      if( reterr > allowederr )
	{
	  fprintf(stderr, "REV: downsample_recursive -- reterr > allowed error\n");
	  exit(1);
	}

      double foundratio = (float64_t)retwidpix / intermediate_sz.width;
      if( foundratio < min_perstep_ratio )
	{
	  fprintf(stderr, "Wtf ? Recursive blur/downsample returned < 0.5 step [%lf]?\n", foundratio);
	  exit(1);
	}
      else if( foundratio > max_perstep_ratio )
	{
	  
	  
	  recur_sizes.push_back( targsize );

	  
	  break;
	}
      else 
	{
	  

	  
	  
	  intermediate_sz = cv::Size( retwidpix, retheipix );
	  recur_sizes.push_back( intermediate_sz );
	}
      
      
    }
  while( intermediate_sz.width > targsize.width ); 

  return recur_sizes;
}

















std::vector<float64_t> salmap_rv::recursive_gauss_lpf_sigmas( const std::vector<cv::Size>& recur_sizes, const double origpixperdva_w, const double nyq_sigmaf_mult, std::vector<float64_t>& pixperdvas )
{
  pixperdvas.clear();
  std::vector<float64_t> ret_blur_sigma_dvas;
  pixperdvas.push_back( origpixperdva_w );

  
  
  if( recur_sizes.empty() )
    {
      fprintf(stderr, "nyq_blur_ratios -- empty!\n");
      exit(1);
    }
  else if( 1 == recur_sizes.size() )
    {
      
      
      
    }
  else
    {
      
      double pixperdva_w = origpixperdva_w;
      double orig_dva_w = recur_sizes[0].width / pixperdva_w; 
      
      double finalnyqf = nyqf_from_pixperdva( recur_sizes.back().width / orig_dva_w ); 

      std::vector<double> finalmults;
	
      for( size_t x=1; x<recur_sizes.size(); ++x )
	{
	  cv::Size prevsize = recur_sizes[x-1];
	  cv::Size nextsize = recur_sizes[x];
	  double ratio = (double)nextsize.width / prevsize.width;  
	  double newpixperdva_w = ratio * pixperdva_w; 
	  
	  double newnyquist = nyqf_from_pixperdva( newpixperdva_w );

	  double multfinalnyq = newnyquist / finalnyqf;
	  finalmults.push_back(multfinalnyq);
	  
	  
	  double sigmaf_cyc_dva = newnyquist / nyq_sigmaf_mult; 
	  
	  
	  double sigma_dva = sigma_from_sigmaf( sigmaf_cyc_dva );

	  pixperdvas.push_back( newpixperdva_w );
	  ret_blur_sigma_dvas.push_back( sigma_dva );
	  
	  pixperdva_w = newpixperdva_w;
	}
      
      if( finalmults.back() != 1.0 )
	{
	  fprintf(stderr, "WARNING Final mults not 1 (maybe float err?), [%lf]\n", finalmults.back() );
	}
			   
      double C = 1/(2*M_PI);
      
      double T = ( C * nyq_sigmaf_mult / finalnyqf );
      double T2=T*T;
      double C2=C*C;

      double accum=0;
      for( size_t x=0; x<finalmults.size(); ++x )
	{
	  accum += C2/(finalmults[x]*finalmults[x]);
	}
      accum /= T2;
      accum = sqrt(accum);
      
      double A = accum;

      double accumsig=0;
      
      
      for( size_t x=1; x<recur_sizes.size(); ++x )
	{
	  double mysamplerate =  recur_sizes[x].width / orig_dva_w; 
	  double mynyq = nyqf_from_pixperdva( mysamplerate );
	  double myA = finalmults[x-1] * A;
	  double mysig = (1/(2*M_PI*myA));
	  
	  
	  accumsig += mysig*mysig;
	  
	}
      
    }

  return ret_blur_sigma_dvas;
}





void salmap_rv::recursive_downsize_2mat_optim( const salmap_rv::FiltMat& i, salmap_rv::FiltMat& m1, salmap_rv::FiltMat& m2,
					       const std::vector<cv::Size>& sizes, const std::vector<float64_t>& pixperdvas, const std::vector<float64_t>& blurwidths_dva,
					       const int interp )
{
  if( sizes.size() != blurwidths_dva.size() + 1)
    {
      fprintf(stderr, "Uh, (recursive_downsize_2mat) blurwidths/sizes not correctly sized?\n");
      exit(1);
    }

  if( sizes.size() > 1 )
    {
      for( size_t x=0; x<blurwidths_dva.size(); ++x )
	{
	  double level_sigmapix = pixperdvas[x] * blurwidths_dva[x];
	  cv::Size nextlevel_size = sizes[x+1];
	  
	  if( 0 == x )
	    {
	      cv::GaussianBlur( i, m1, cv::Size(0,0), level_sigmapix );
	    }
	  else
	    {
	      cv::GaussianBlur( m2, m1, cv::Size(0,0), level_sigmapix );
	    }
	  
	  cv::resize( m1, m2, nextlevel_size, 0, 0, interp );
	  
	  
	  
	}
    } 
  else if( 1 == sizes.size() )
    {
      
      i.copyTo( m2 );
    }
  else
    {
      fprintf(stderr, "Sizes is 0 size?\n");
      exit(1);
    }
}
  


void salmap_rv::recursive_downsize_2mat( const salmap_rv::FiltMat& i, salmap_rv::FiltMat& m1, salmap_rv::FiltMat& m2,
			      const cv::Size& targsize, const double min_perstep_ratio, const double max_perstep_ratio,  const double orig_aspect_wh, const double allowederr,
			      const double pix_per_dva_w, const double nyq_sigmaf_mult,
			      const int interp )
{
  auto sizes = recursive_downsample( i.size(), targsize, min_perstep_ratio, max_perstep_ratio, orig_aspect_wh, allowederr );
  if( sizes.size() > 1 )
    {
      std::vector<float64_t> pixperdvas;
      auto blurwidths_dva = recursive_gauss_lpf_sigmas( sizes, pix_per_dva_w, nyq_sigmaf_mult, pixperdvas );

      if( sizes.size() != blurwidths_dva.size() + 1)
	{
	  fprintf(stderr, "Uh, (recursive_downsize_2mat) blurwidths/sizes not correctly sized?\n");
	  exit(1);
	}
      
      for( size_t x=0; x<blurwidths_dva.size(); ++x )
	{
	  double level_sigmapix = pixperdvas[x] * blurwidths_dva[x];
	  cv::Size nextlevel_size = sizes[x+1];

	  if( 0 == x )
	    {
	      cv::GaussianBlur( i, m1, cv::Size(0,0), level_sigmapix );
	    }
	  else
	    {
	      cv::GaussianBlur( m2, m1, cv::Size(0,0), level_sigmapix );
	    }

	  cv::resize( m1, m2, nextlevel_size, 0, 0, interp );
	  
	  
	  
	}
    } 
  else if( 1 == sizes.size() )
    {
      
      i.copyTo( m2 );
    }
  else
    {
      fprintf(stderr, "Sizes is 0 size?\n");
      exit(1);
    }
}







































































void subsample_blur_downsize( const salmap_rv::FiltMat& i, salmap_rv::FiltMat& retmat,
			      const cv::Size& outsize, const float64_t xsigma_pix )
{
}














void salmap_rv::set_filt_input( FeatFilter& f, grouped_input gi )
{
  
  auto itr = f.inputs.find( gi.getfullname() );
  if( itr != f.inputs.end() )
    {
      int64_t i=0;
      std::string mystr = gi.getfullname() + std::to_string(i);
      
      while ( f.inputs.find( mystr ) != f.inputs.end() )
	{
	  ++i;
	  mystr = gi.getfullname() + std::to_string(i);
	}
      
      
      fprintf(stdout, "Base case set_filt_input VALUE: [%s] (tag: [%s])\n", gi.value.c_str(), gi.getfullname().c_str() );
      fprintf(stderr, "ERROR (warning?): you are attempting to overwrite an existing input in filter [%s] -- modifying input name to [%s]!!!\n",  gi.getfullname().c_str(), mystr.c_str());
      f.inputs[ mystr ] = gi.value;
    }
  else
    {
      f.inputs[ gi.getfullname() ] = gi.value;
    }
}

void salmap_rv::set_filt_param( FeatFilter& f, grouped_param gp )
{
  auto itr = f.params.find( gp.getfullname() );
  if( itr != f.params.end() )
    {
      int64_t i=0;
      std::string mystr = gp.getfullname() + std::to_string(i);
      
      while ( f.params.find( mystr ) != f.params.end() )
	{
	  ++i;
	  mystr = gp.getfullname() + std::to_string(i);
	}
      
      
      fprintf(stdout, "Base case set_filt_param VALUE: [%s] (tag: [%s])\n", gp.value.c_str(), gp.getfullname().c_str() );
      fprintf(stderr, "ERROR (warning?): you are attempting to overwrite an existing param in filter [%s] -- modifying param name to [%s]!!!\n",  gp.getfullname().c_str(), mystr.c_str());
      f.params[ mystr ] = gp.value;
      fprintf(stderr, "Yea, that should not happen. Exiting\n");
      exit(1);
    }
  else
    {
      f.params[ gp.getfullname() ] = gp.value;
    }
  
  
  
}

void salmap_rv::set_filt_input_params( FeatFilter& f )
{
  
  
}

 

 



















