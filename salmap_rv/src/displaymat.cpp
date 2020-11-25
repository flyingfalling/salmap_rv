#include <salmap_rv/include/displaymat.hpp>

#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/load_salmap.hpp>
using namespace salmap_rv;

void displaymat::reset()
{

  outputs.clear();
  hasmat = false;
  hasprettymat=false;
  isselected = false;
  gotoutputs=false;
  selected_model = "";
  desc="";
}

displaymat::displaymat()
{
  outputs.clear();
  hasmat = false;
  hasprettymat=false;
  isselected = false;
  gotoutputs=false;
  selected_model = "";
  desc="";
}

void displaymat::set_filter( SalMap& salmap, const std::string& name )
{
  std::unique_lock<std::mutex> lk(mux);
  reset();
  auto tokenized = tokenize_string( name, "@", true );
  if( tokenized.size() != 2 )
    {
      fprintf(stderr, "set_filt expects outputname@filtername, got [%s]\n", name.c_str());
      exit(1);
    }
  outputname=tokenized[0];
  
  selected_model=tokenized[1];
    
  auto iter = salmap.filtset.filters.find( selected_model );
  if( iter != salmap.filtset.filters.end() )
    {
      desc = outputname + " @ " + iter->second.getdesc( );
      isselected = true;
      f = iter->second;
      fp = salmap.getMapAccessor( outputname, selected_model );
    }
  else
    {
      fprintf(stdout, "Could not find filter [%s]\n", name.c_str() );
      isselected = false;
    }
}

void displaymat::update( SalMap& salmap )
{
  std::unique_lock<std::mutex> lk(mux);
  if( isselected && salmap.was_updated() )
    {
      if( !gotoutputs )
	{
	  outputs = salmap.get_outputs_of_filter( f );
	  gotoutputs = true;
	  if( outputs.size() < 1 )
	    {
	      fprintf(stderr, "REV: error, filter [%s] has no outputs?\n", f.nickname.c_str());
	      exit(1);
	    }
	  outputname = outputs.front();
	}
	
      
      
      
      bool got = salmap.mapFromAccessor( fp, fm, salmap.get_time_nsec() - salmap.get_dt_nsec() );
      if( got )
	{
	  {
	    fm->copyCpuTo( mymatraw );
	    hasmat=true;
	  }
	    
	}
      else
	{
	  fprintf(stdout, "Failed to get!\n");
	  hasmat=false;
	}
    }
  else
    {
      hasmat=false;
    }
}

void displaymat::make_pretty( const cv::Mat& raw, const double alpha, const bool drawEye, const int eyeres, const bool drawTime, const bool drawName, const float eyex, const float eyey, const int64_t time, const std::string name )
{
  std::unique_lock<std::mutex> lk(mux);
  if( hasmat )
    {
      mymatraw.convertTo(mymat, CV_8UC1, 255.0);

      if( pretty )
	{
	  mymat = apply_color( mymat );
	    
	}
      else
	{
	  cv::cvtColor(mymat, mymat, cv::COLOR_GRAY2BGR);
	}

      
      cv::resize( mymat, mymat, raw.size() );
	
      if( pretty )
	{
	  mymat = overlay_salmap( raw, mymat, alpha );
	}
	
      if( drawEye && eyeres > 0 ) { draw_eye_pos( mymat, eyex, eyey ); }

      if( drawTime ) { add_timing(time, mymat ); }

      if( drawName ) { add_name(desc, mymat); }
	
      hasprettymat=true;
    }
  else
    {
      hasprettymat=false;
    }
}

void displaymat::getMat( cv::Mat& ret )
{
  std::unique_lock<std::mutex> lk(mux);
  
  mymat.copyTo( ret );
}
