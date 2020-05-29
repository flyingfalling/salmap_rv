
auto cui = GETINPUT("currUnshift");
auto pui = GETINPUT("prevUnshift"); 

auto output = GETOUTPUT("output");

#define WARPTYPE float

if( true == pui->isDummy() )
  {
    output->cpu_w() = salmap_rv::FiltMat::zeros( cui->size(), cui->cpu().type() );
    output->setMetadata( cui->get_time(), cui->get_dva_wid(), cui->get_dva_hei(), cui->get_origaspectratio_wh() );
    
    
    return;
  }


if( false == scratch.initialized() )
  {
    int64_t timedelta_nsec = cui->get_time() - pui->get_time();
    float64_t angle_deg = param_as_float64( params, "motion_angle_deg" );
    float64_t speed_dva_sec = param_as_float64( params, "motion_speed_dva_per_sec" );

    DEBUGPRINTF(stdout, "Reichardt motion: dva wid: [%lf], pix wid [%d], dva/pix wid [%lf] (timedelta = %ld)\n", cui->get_dva_wid(), cui->size().width, cui->get_dva_per_pix_wid(), timedelta_nsec );
    double pix_per_sec = speed_dva_sec / cui->get_dva_per_pix_wid() ; 

    
    
    
    float64_t timedelta_sec = timedelta_nsec / SEC_TO_NSEC; 
    double pix_per_timedelta = pix_per_sec * timedelta_sec; 
    double angle_rad = CV_PI * angle_deg / (180.0); 
    double yshift_pixpertime = pix_per_timedelta * sin(angle_rad); 
    double xshift_pixpertime = pix_per_timedelta * cos(angle_rad); 

    
    scratch.float64("xpix_per_nsec") = pix_per_sec/SEC_TO_NSEC * sin(angle_rad);
    scratch.float64("ypix_per_nsec") = pix_per_sec/SEC_TO_NSEC * cos(angle_rad);
    
    HELPPRINTF(stdout, "REICHARDT MOTION (%s): will do a per dt %lf pixel shift in direction [%lf] radians (%lf deg), i.e. xshift [%lf], yshfit [%lf] (imgsize %d %d)\n", nickname.c_str(), pix_per_timedelta, angle_rad, angle_deg, yshift_pixpertime, xshift_pixpertime, cui->size().width, cui->size().height );
      
    if( pix_per_timedelta < 0.0001 || pix_per_timedelta > 200 )
      {
	fprintf(stdout, "!!!!!!!!!!!!!!!WARNING! REICHARDT MOTION (%s) will do a %lf pixel shift in direction [%lf] radians (%lf deg), i.e. xshift [%lf], yshfit [%lf] (printing because questionable timedelta < 0.0001 | > 200.0)\n", nickname.c_str(), pix_per_timedelta, angle_rad, angle_deg, yshift_pixpertime, xshift_pixpertime );
      }
      
    if( cui->size() != pui->size() )
      {
	fprintf(stderr, "REV: pu and cu size not eq reichardt\n");
	exit(1);
      }

    
    cv::Mat tmp = (cv::Mat_<WARPTYPE>(2,3) << 1, 0, xshift_pixpertime, 0, 1, yshift_pixpertime);
    tmp.copyTo(scratch.cpu_w("transMat"));
    
    
    attenuation_filter_cpu( param_as_float64(params, "atten_size_prop"), cui->cpu() , scratch.cpu_w("atten") );
    scratch.float64("thresh") = param_as_float64(params, "mot_thresh");
  }



  
#ifdef USE_REAL_TIMEDELTA
const int64_t timedelta_nsec = cui->get_time() - pui->get_time();
const double xshift_pixpertime = timedelta_nsec * scratch.float64("xpix_per_nsec");
const double yshift_pixpertime = timedelta_nsec * scratch.float64("ypix_per_nsec");
DEBUGPRINTF(stdout, "REICHARDT: (tdelta: [%lf] msec) -- executing pixel shift x: [%lf]   y: [%lf]\n", timedelta_nsec/1e6, xshift_pixpertime, yshift_pixpertime );
const cv::Mat tmp = (cv::Mat_<WARPTYPE>(2,3) << 1, 0, xshift_pixpertime, 0, 1, yshift_pixpertime);
tmp.copyTo(scratch.cpu_w("transMat"));
#endif

cv::warpAffine( cui->cpu(), scratch.cpu_w("cs"), scratch.cpu("transMat"), cui->size() );
cv::warpAffine( pui->cpu(), output->cpu_w(), scratch.cpu("transMat"), cui->size() );

cv::multiply( cui->cpu(), output->cpu(), output->cpu_w() );
cv::multiply( pui->cpu(), scratch.cpu("cs"), scratch.cpu_w("cs") );

cv::subtract( output->cpu(), scratch.cpu("cs"), output->cpu_w() );


float64_t thresh=param_as_float64(params, "mot_thresh"); 
float64_t irrelevant=-1;

cv::threshold( output->cpu(), output->cpu_w(), scratch.float64("thresh"), irrelevant, cv::THRESH_TOZERO );

cv::multiply( output->cpu(), scratch.cpu("atten"), output->cpu_w() );


output->setMetadata( cui->get_time(), cui->get_dva_wid(), cui->get_dva_hei(), cui->get_origaspectratio_wh() );


 
