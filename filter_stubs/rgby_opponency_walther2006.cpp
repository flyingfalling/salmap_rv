

auto rin = GETINPUT("rin");
auto gin = GETINPUT("gin");
auto bin = GETINPUT("bin");

auto lumin = GETINPUT("lumin");

auto rgout = GETOUTPUT("rg");
auto byout = GETOUTPUT("by");

cv::subtract( rin->cpu(), gin->cpu(), scratch.cpu_w("r-g") );
cv::min( rin->cpu(), gin->cpu(), scratch.cpu_w("min(r,g)") );
cv::subtract( bin->cpu(), scratch.cpu("min(r,g)"), scratch.cpu_w("b-y") );




#define MAXRGB

#ifdef MAXRGB
cv::max( rin->cpu(), gin->cpu(), scratch.cpu_w("lum") ); 
cv::max( scratch.cpu("lum"), bin->cpu(), scratch.cpu_w("lum") );


#else

lumin->copyCpuTo( scratch.cpu_w("lum") ); 
#endif

float64_t LUM_SMALL_NUM=0.001; 

cv::add( scratch.cpu("lum"), LUM_SMALL_NUM, scratch.cpu_w("lum") );

double mi, ma;
cv::minMaxLoc( scratch.cpu("lum"), &mi, &ma );

if( false == scratch.initialized() )
  {
    float32_t lumthresh = param_as_float64( params, "lum_thresh" );
    scratch.float64("lumthresh") = lumthresh;
  }

float32_t lumthresh = scratch.float64("lumthresh");

if( ma >= lumthresh )
  {
    cv::threshold( scratch.cpu("lum"), scratch.cpu_w("mask"), lumthresh, 1.0, cv::THRESH_BINARY ); 
    
    cv::multiply( scratch.cpu("r-g"), scratch.cpu("mask"), rgout->cpu_w() );
    cv::multiply( scratch.cpu("b-y"), scratch.cpu("mask"), byout->cpu_w() );
    
    cv::divide( rgout->cpu(), scratch.cpu("lum"), rgout->cpu_w() );
    cv::divide( byout->cpu(), scratch.cpu("lum"), byout->cpu_w() );
  }
 else 
   {
     
     cv::multiply( rin->cpu(), 0, rgout->cpu_w() );
     cv::multiply( rin->cpu(), 0, byout->cpu_w() );
   }

rgout->setMetadata( rin->get_time(), rin->get_dva_wid(), rin->get_dva_hei(), rin->get_origaspectratio_wh() );
byout->setMetadata( rin->get_time(), rin->get_dva_wid(), rin->get_dva_hei(), rin->get_origaspectratio_wh() );
