

#include <salmap_rv/include/itti_salmap.hpp>
#include <salmap_rv/include/load_salmap.hpp>
#include <salmap_rv/include/salmap.hpp>
#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/param_set.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>
#include <salmap_rv/include/featmapimplptr.hpp>
#include <salmap_rv/include/Timer.hpp>

#include <iomanip>
#include <sstream>




#include <opencv2/core.hpp>
#include <opencv2/hdf.hpp>

using namespace salmap_rv;

void testinterp(const int noww, const int nowh)
{
  double err2=0.002;
  
  std::vector<double> rats = {4, 2, 1, 1.0/2.0, 1.0/4.0, 1.0/8.0, 1.0/16, 1.0/32, 1/64.0, 1/128.0};
  
  
  double origrat = noww/(double)nowh;
  fprintf(stdout, "%d %d:\n", noww, nowh);
  for( auto rat : rats )
    {
      int retw, reth;
      float64_t err = 
	resize_image_w_ratio( noww, nowh, rat, err2, origrat, retw, reth );
      fprintf(stdout, "   %d %d\n", retw, reth);
    }
  
}


void showlums( SalMap& sm )
{
  std::vector<cv::Mat> lums;
  for( size_t x=0; x<9; ++x )
    {
      std::string name = "lum"+std::to_string(x);
      lums.push_back( sm.get_map_now_nickname( name, "output", cv::Size(640,0)));
      double mi, ma;
      cv::minMaxLoc( lums.back(), &mi, &ma );
      fprintf(stdout, "[%s] Min %lf   Max %lf\n", name.c_str(), mi, ma ); 
      cv::imshow( name, lums.back() );
    }
}

void showlumdiffs( SalMap& sm )
{
  std::vector<cv::Mat> lums;
  for( size_t x=2; x<=4; ++x )
    {
      for( size_t s=x+3; s<=x+4; ++s )
	{
	  std::string name = "lum"+std::to_string(x)+"-"+std::to_string(s);
	  lums.push_back( sm.get_map_now_nickname( name, "output", cv::Size(640,0) ) );
	  double mi, ma;
	  cv::minMaxLoc( lums.back(), &mi, &ma );
	  fprintf(stdout, "[%s] Min %lf   Max %lf\n", name.c_str(), mi, ma ); 
	  cv::imshow( name, lums.back() );
	}
    }
}

void showlumddiffs( SalMap& sm )
{
  std::vector<cv::Mat> lums;
  for( size_t x=2; x<=4; ++x )
    {
      for( size_t s=x+3; s<=x+4; ++s )
	{
	  std::string name = "dlum"+std::to_string(x)+"-"+std::to_string(s);
	  lums.push_back( sm.get_map_now_nickname( name, "output", cv::Size(640,0) ) );
	  double mi, ma;
	  cv::minMaxLoc( lums.back(), &mi, &ma );
	  fprintf(stdout, "[%s] Min %lf   Max %lf\n", name.c_str(), mi, ma ); 
	  cv::imshow( name, lums.back() );
	}
    }
}

void showlumnddiffs( SalMap& sm )
{
  std::vector<cv::Mat> lums;
  for( size_t x=2; x<=4; ++x )
    {
      for( size_t s=x+3; s<=x+4; ++s )
	{
	  std::string name = "ndlum"+std::to_string(x)+"-"+std::to_string(s);
	  lums.push_back( sm.get_map_now_nickname( name, "output", cv::Size(640,0) ) );
	  double mi, ma;
	  cv::minMaxLoc( lums.back(), &mi, &ma );
	  fprintf(stdout, "[%s] Min %lf   Max %lf\n", name.c_str(), mi, ma ); 
	  cv::imshow( name, lums.back() );
	}
    }
}


void embed_in_mat( cv::Mat& retm, const cv::Mat& argm, const size_t x_from_left, const size_t y_from_top )
{
  argm.copyTo( retm(cv::Rect(x_from_left, y_from_top, argm.size().width, argm.size().height ) ) );
}

void itr_to_xy( const size_t ncols, const size_t itr, size_t& x, size_t& y )
{
  x = itr % ncols; 
  y = itr / ncols; 
}


cv::Mat embed_frames_equal_sameaspect( const std::vector<cv::Mat>& mymats2, const size_t ncols, const size_t targwid, const size_t borderpx=7 )
{
  

  
  cv::Size msiz;
  int mtype;

  std::vector<cv::Mat> mymats;
  for( auto& m : mymats2 )
    {
      mymats.push_back( resize_wid( m, targwid, cv::INTER_CUBIC ) );
    }
  
  size_t itr = 0;
  
  
  const size_t nrows = (mymats.size()+1) / ncols; 

  const size_t xborders = ncols-1; 
  const size_t yborders = nrows-1;
    
  cv::Mat retm;
  if( false == mymats.empty() )
    {
      msiz = mymats.front().size();
      mtype = mymats.front().type();
      size_t bigwidpix = ncols * msiz.width + xborders*borderpx;
      size_t bigheipix = nrows * msiz.height + yborders*borderpx;
      
      retm = cv::Mat( cv::Size( bigwidpix, bigheipix ), mtype, cv::Scalar(0) ); 
      
      auto m = mymats.front();
      
      size_t x,y;
      itr_to_xy( ncols, itr, x, y );
      ++itr;
      size_t borderextrax=0;
      size_t borderextray=0;
      if( x>0 )
	{
	  borderextrax = x*borderpx;
	}
      if( y>0 )
	{
	  borderextray = y*borderpx;
	}
      
      embed_in_mat( retm, m, x*msiz.width+borderextrax, y*msiz.height+borderextray );
    }
  for( size_t a=1; a<mymats.size(); ++a )
    {
      cv::Mat m = mymats[a];
      if( m.type() != mtype || m.size() != msiz )
	{
	  fprintf(stderr, "One of these is of different type...(%s) vs (%s) (%d %d) vs (%d %d)?\n", cv::typeToString(m.type()).c_str(), cv::typeToString(mymats.front().type()).c_str(), m.size().width, m.size().height, msiz.width, msiz.height );
	  exit(1);
	}
      
      size_t x,y;
      itr_to_xy( ncols, itr, x, y );
      ++itr;
      size_t borderextrax=0;
      size_t borderextray=0;
      if( x>0 )
	{
	  borderextrax = x*borderpx;
	}
      if( y>0 )
	{
	  borderextray = y*borderpx;
	}
      
      embed_in_mat( retm, m, x*msiz.width+borderextrax, y*msiz.height+borderextray );
      
    }
  return retm;
}

int main(int argc, char** argv)
{
  
  double dva_image=90; 
  size_t nworkers=1;
  int64_t input_wid_pix=0;
  
  
  
  
  
  

  
  

  bool isvid=false;
  bool iscam=false;
  std::string fname;
  std::string type;
  std::string outvidfname;
  std::string vidext = ".mp4";
  int64_t bgw=0; int64_t bgh=0;
  std::string paramsfile;

  
  if( argc > 2 )
    {
      type=std::string(argv[1]);
      fname=std::string(argv[2]);
      fprintf(stdout, "Type [%s] : Reading in image (vid?) file from [%s]\n", type.c_str(), fname.c_str());

      if( same_string(type, "vid") )
	{
	  fprintf(stdout, "Is vid!\n");
	  isvid=true;
	  iscam=false;
	}
      else if( same_string(type, "img") )
	{
	  fprintf(stdout, "Is img!\n");
	  isvid=false;
	  iscam=false;
	}
      else if( same_string(type, "cam" ) )
	{
	  fprintf(stdout, "Is cam!\n");
	  iscam=true;
	  isvid=false;
	}
      else
	{
	  fprintf(stderr, "Unrecognized type [%s]\n", type.c_str() );
	  exit(1);
	}
    }

  if( argc > 3 )
    {
      std::string asdf = std::string(argv[3]);
      std::stringstream ss( asdf );
      ss >> dva_image;
      fprintf(stdout, "Set DVA of whole image to [%lf]\n", dva_image);
    }

  if( argc > 4 )
    {
      std::string asdf = std::string(argv[4]);
      std::stringstream ss( asdf );
      ss >> input_wid_pix;
      fprintf(stdout, "Set input wid pix of whole image to [%ld]\n", input_wid_pix );
    }

  

  if( argc > 5 )
    {
      if( argc < 7 )
	{
	  fprintf(stderr, "REV: expect 2 bg numbers...\n");
	  exit(1);
	}
      
      std::string asdf = std::string(argv[5]);
      std::stringstream ss( asdf );
      ss >> bgw;
      asdf = std::string(argv[6]);
      ss = std::stringstream( asdf );
      ss >> bgh;
      fprintf(stdout, "Set bg [%ld %ld]\n", bgw, bgh );
    }

  
  if( argc > 7 )
    {
      paramsfile = std::string(argv[7]);
      fprintf(stdout, "Will load params from file [%s]\n", paramsfile.c_str() );
    }
  
  bool showme = true;
  if( argc > 8 )
    {
      outvidfname = std::string( argv[8] );

      
      outvidfname += "_dva-" + std::to_string( dva_image );
      outvidfname += "_wid-" + std::to_string( input_wid_pix );
      outvidfname += vidext;
      fprintf(stdout, "Outputting to video [%s] (added [%s]) (note setting showme to false)\n", outvidfname.c_str(), vidext.c_str() );
      showme = false;
    }

  cv::VideoCapture video;
  cv::Mat frame;
  size_t frame_wid;
  int64_t vidfps = 25;
  int64_t dt_nsec = 1e9 / vidfps;
  

  
  
  
  
  if( isvid )
    {
      fprintf(stdout, "Will video cap from [%s]\n", fname.c_str() );
      video = cv::VideoCapture(fname);
      
      if(!video.isOpened())
	{
	  fprintf(stderr, "REV: error, couldn't open video (Fname=[%s])\n", fname.c_str());
	  exit(1);
	}
      frame_wid = video.get(cv::CAP_PROP_FRAME_WIDTH);
      if( frame_wid <= 0 )
	{
	  fprintf(stderr, "REV: wtf itti formal frame wid 0?\n");
	  exit(1);
	}
      
      if(input_wid_pix <= 0)
	{
	  input_wid_pix = frame_wid;
	}
      
      vidfps = video.get(cv::CAP_PROP_FPS);
      if( vidfps <= 0 )
	{
	  fprintf(stderr, "FPS is fucked?\n");
	  exit(1);
	}
      DEBUGPRINTF(stdout, "GOT FPS of [%d]\n", vidfps);
      dt_nsec = 1e9 / vidfps; 
    }
  else if( iscam )
    {
      fprintf(stdout, "Will video cap from WEBCAM\n", fname.c_str() );
      video = cv::VideoCapture(0);
      
      if(!video.isOpened())
	{
	  fprintf(stderr, "REV: error, couldn't open video (Fname=[%s])\n", fname.c_str());
	  exit(1);
	}
      frame_wid = video.get(cv::CAP_PROP_FRAME_WIDTH);
      if(input_wid_pix <= 0)
	{
	  input_wid_pix = frame_wid;
	}
      
      vidfps = video.get(cv::CAP_PROP_FPS);
      if( vidfps <= 0 )
	{
	  fprintf(stderr, "FPS is fucked?\n");
	  exit(1);
	}
      DEBUGPRINTF(stdout, "GOT FPS of [%d]\n", vidfps);
      dt_nsec = 1e9 / vidfps; 
    }
  else
    {
      fprintf(stdout, "Reading single video frame from [%s]\n", fname.c_str() );
      
      frame = cv::imread( fname );
      frame_wid = frame.size().width;
      exit(1);
    }
  
  
#ifdef USE1998
  param_set p = itti_formal_1998_params( frame_wid, vidfps ); 
#else
  param_set p = itti_formal_default_params(); 
  p.set<float64_t>("input_dva_wid", dva_image);
#endif

  fprintf(stdout, "Full image (including BG) dva: [%lf]\n", dva_image/input_wid_pix * bgw );
  
  p.set<int64_t>("dt_nsec", dt_nsec );
  p.set<int64_t>("nsalthreads", 3);

  
  if( false == paramsfile.empty() )
    {
      p.fromfile( paramsfile );
    }
  
  p.enumerate();
  
  SalMap cpusalmap;
  make_itti_dyadic_cpu_weighted_formal( p, cpusalmap, nullptr );
  fprintf(stdout, "Made it, now run\n");
  
  
  std::shared_ptr<FeatMapImplInst> finori, finlum, finrg, finby, finmot, finfin, fincol;
  
  FeatMapImplPtr colfp = cpusalmap.getMapAccessor( "output", "navg_col" );
  FeatMapImplPtr orifp = cpusalmap.getMapAccessor( "output", "navg_orient_all" );
  FeatMapImplPtr lumfp = cpusalmap.getMapAccessor( "output", "avg_nlumlpf" );

  FeatMapImplPtr motfp = cpusalmap.getMapAccessor( "output", "navg_mot_all");
  
  FeatMapImplPtr finfp = cpusalmap.getMapAccessor( "output", "finavg" );

  cv::VideoWriter vw;
  
  double avg=0;
  
  
  
  
  size_t maxsec=300;
  size_t maxframe = vidfps * maxsec;
  
  Timer timer_sec;
  cv::Mat frame1;


  
  fprintf(stdout, "Will open file...(new?)\n");
  std::string h5filename = "./testfile.h5";
  cv::Ptr<cv::hdf::HDF5> h5io = cv::hdf::open(h5filename);

  
  
  std::string parent_name = "/data";
  std::string dataset_name = parent_name + "/video1";

  if (!h5io->hlexists(parent_name)) h5io->grcreate(parent_name);

  
  
  
  

  
  
  

  
  
  
  
  for(size_t framen=0; framen < maxframe; ++framen )
    {
      
      if(isvid || iscam)
	{
	  if( video.read( frame ) )
	    {
	      
	    }
	  else
	    {
	      fprintf(stdout, "Video file finished\n");
	      return 0;
	    }
	}
      
      
	  frame1 = frame;
	  

      cv::Mat frame2;
      size_t targwid=640;
      double ratio = (double)frame.size().height / frame.size().width;
      size_t targhei = ratio * targwid; 
      cv::Size targsize(targwid, targhei);
      
      cv::resize( frame1, frame2, targsize);
      
      
      cv::Mat inframe2 = frame;
      
      if( input_wid_pix > 0 )
	{
	  inframe2 = resize_wid( frame, input_wid_pix, cv::INTER_AREA );
	}
      
      cv::Mat inframe = inframe2;
      
      if( bgw > 0 )
	{
	  
	  uchar bgblu=162;
	  uchar bggre=162;
	  uchar bgred=162;
	  inframe = embed_bg(  cv::Size( bgw, bgh ), cv::Scalar( bgblu, bggre, bgred ),  inframe2 );
	}
      
      
      Timer t3;
      double currtime_sec = timer_sec.elapsed();
      
      cpusalmap.add_input_copy_realtime( "bgr", inframe, currtime_sec ); 
      bool cpuupdated = cpusalmap.update();

      avg+=t3.elapsed();
      fprintf(stdout, "Updating [%ld]th frame of w/h %d %d took [%lf] msec (avg %lf)\n", framen, inframe.size().width, inframe.size().height, t3.elapsed()*1e3, avg*1e3/(framen+1));

      
      if( true == cpuupdated )
	{
	  fprintf(stdout, "CPU UPDATED\n");
	  
	  bool gotlum = cpusalmap.mapFromAccessor( lumfp, finlum, cpusalmap.get_time_nsec() - cpusalmap.get_dt_nsec() );
	  bool gotcol = cpusalmap.mapFromAccessor( colfp, fincol, cpusalmap.get_time_nsec() - cpusalmap.get_dt_nsec() );
	  bool gotori = cpusalmap.mapFromAccessor( orifp, finori, cpusalmap.get_time_nsec() - cpusalmap.get_dt_nsec() );
      
	  
	  

	  bool gotmot = cpusalmap.mapFromAccessor( motfp, finmot, cpusalmap.get_time_nsec() - cpusalmap.get_dt_nsec() );
	  bool gotfin = cpusalmap.mapFromAccessor( finfp, finfin, cpusalmap.get_time_nsec() - cpusalmap.get_dt_nsec() );

	  if( false && showme )
	    {
	      if( gotori )
		{
		  cv::Mat ori;
		  cv::resize( finori->cpu(), ori, targsize );
		  fprintf(stdout, "Ori size %d %d\n", ori.size().width, ori.size().height);
		  cv::imshow( "Ori", ori );
		}
	      if( gotlum )
		{
		  cv::Mat lum;
		  cv::resize( finlum->cpu(), lum, targsize );
		  cv::imshow( "Lum", lum );
		}
	      if( gotcol )
		{
		  cv::Mat col;
		  cv::resize( fincol->cpu(), col, targsize );
		  cv::imshow( "Col", col );
		}
	      if( gotmot )
		{
		  cv::Mat mot;
		  cv::resize( finmot->cpu(), mot, targsize );
		  cv::imshow( "Mot", mot );
		}
	      if( gotfin )
		{
		  cv::Mat fin;
		  cv::resize( finfin->cpu(), fin, targsize );
		  cv::imshow( "Fin", fin );
		}
	    
	      
	      
	      cv::imshow( "Raw", inframe );
	  
	    }
	      
	  
	  
	  if( gotlum && gotmot && gotcol && gotori && gotfin )
	    {
	      double alpha=0.0;
	      std::vector<cv::Mat> mats;
	      int interp = cv::INTER_CUBIC;
	      cv::Mat raw = resize_wid( inframe, 320, cv::INTER_AREA ); 
	      auto plum = make_pretty( finlum->copyAsRealCVMat(), "Lum", -1, raw, alpha, interp );
	      auto pmot = make_pretty( finmot->copyAsRealCVMat(), "Mot", -1, raw, alpha, interp );
	      auto pori = make_pretty( finori->copyAsRealCVMat(), "Ori", -1, raw, alpha, interp );
	      auto pcol = make_pretty( fincol->copyAsRealCVMat(), "Col", -1, raw, alpha, interp );
	      auto pfin = make_pretty( finfin->copyAsRealCVMat(), "Sal", finlum->get_time() / 1e6, raw, alpha, interp );
	      
	      

	      
	      
	      mats.push_back(raw);
	      mats.push_back(pfin);
	      mats.push_back(plum);
	      mats.push_back(pmot);
	      mats.push_back(pori);
	      mats.push_back(pcol);

	      size_t ncols=3;
	      size_t eachwidpx=320;
	      size_t bpix=5;
	      auto combined = embed_frames_equal_sameaspect( mats, ncols, eachwidpx, bpix );

	      if(false == outvidfname.empty() )
		{
		  if( false == vw.isOpened() )
		    {
		      std::string s="h264"; 
		      if(s.size() < 4 ) { fprintf(stderr, "fourcc needs 4 chars\n"); exit(1); }
		      auto my4cc = cv::VideoWriter::fourcc(s[0], s[1], s[2], s[3]);
		      
		      vw.open( outvidfname, my4cc, vidfps, combined.size(), true );
		    }
		  vw << combined;
		}
	      
	      if(showme)
		{
#if DEBUGLEVEL > 100
		  showlums(cpusalmap);
		  showlumdiffs(cpusalmap);
		  showlumddiffs(cpusalmap);
		  showlumnddiffs(cpusalmap);
		  
		  auto strmatmap = cpusalmap.return_all_maps_resized( 320, 0 );
		  for( auto& entry : strmatmap )
		    {
		      cv::imshow( entry.first, entry.second );
		      int key = cv::waitKey(0);
		      if( key == 'q' )
			{
			  fprintf(stdout, "User pressed [q], exiting\n");
			  return 0;
			}
		    }
#endif

		  
		  
		  cv::Mat finmat = finfin->copyAsRealCVMat();
		  
		  int n_dims = 3; 
		  int chunks[n_dims] = { 1, 1, 1 }; 
		  fprintf(stdout, "Got finmat of size [%d %d]\n", finmat.size().width,  finmat.size().height );
		  
		  int dsdims[n_dims] = { cv::hdf::HDF5::H5_UNLIMITED,
					 finmat.size().height, 
					 finmat.size().width 
		  };
		  
		  if(!h5io->hlexists(dataset_name))
		    {
		      h5io->dscreate( n_dims,
				      dsdims,
				      finmat.type(),
				      dataset_name,
				      cv::hdf::HDF5::H5_NONE, 
				      chunks );
		      fprintf(stdout, "Created dataset [%s]\n", dataset_name.c_str() );
		    }

		  {
		    fprintf(stdout, "Insert. Framen= [%d] (dims input = [%d]\n", framen, finmat.dims);
		    for( int a=0; a<finmat.dims; ++a)
		      {
			fprintf(stdout, "Dim [%d]: [%d]\n", a, finmat.size[a] );
		      }
		    std::vector<int> newshape = { 1, finmat.size().height, finmat.size().width };
		    cv::Mat refin = finmat.reshape( 0, newshape );
		    int offset[n_dims] = { framen, 0, 0 };
		    
		    h5io->dsinsert( refin, dataset_name, offset ); 
		  }
		  
		  cv::imshow("Combined", combined);
		  char key = cv::waitKey(1);
		  if( key == 'q' )
		    {
		      fprintf(stdout, "User pressed [q], exiting\n");

		      fprintf(stdout, "First replaying HDF5!\n");

		      std::vector<int> sizes = h5io->dsgetsize( dataset_name );
		      
		      for( auto& a : sizes )
			{
			  fprintf(stdout, "H5 Dim: [%d]\n", a );
			}

		      size_t nsavedframes = sizes[0];
		      for( size_t x=0; x<nsavedframes; ++x )
			{
			  int offset[n_dims] = { x, 0, 0 };
			  int counts[n_dims] = { 1, sizes[1], sizes[2] };
			  cv::Mat retmat;
			  h5io->dsread( retmat, dataset_name, offset, counts );
			  retmat = retmat.reshape( 0, std::vector<int>( { sizes[1], sizes[2]} ) );
			  cv::imshow( "H5 Img", retmat );
			  int key2 = cv::waitKey(1);
			  if( key == 'd' )
			    {
			      fprintf(stdout, "again lol\n");
			      return 0;
			    }
			}
		      
		      return 0;
		    }
		}
	    }
	  
	  
	}
      else
	{
	  fprintf(stdout, "CPU UPDATE FAILED!!\n");
	}
    }
  fprintf(stdout, "Stopping due to frame > maxframe [%ld]\n", maxframe);
  
}


