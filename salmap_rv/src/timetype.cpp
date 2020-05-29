#include <salmap_rv/include/timetype.hpp>

#include <salmap_rv/include/util_functs.hpp>

using namespace salmap_rv;

 salmap_rv::TIMETYPE::TIMETYPE() {}
 salmap_rv::TIMETYPE::TIMETYPE( const int64_t& curr_time ) {
    t = ABSTIME;
    offset = curr_time;
  }
 salmap_rv::TIMETYPE::TIMETYPE( const std::string& type, const std::string& off ) {
    set_timetype( type );
    set_timeval( off );
    
    
  }
std::string salmap_rv::TIMETYPE::print() {
    return get_timetype() + ":" + get_timeval();
  }
std::string salmap_rv::TIMETYPE::get_timeval() {
    return stringify_num(offset); 
  }
void salmap_rv::TIMETYPE::set_timeval( const std::string& off ) {
    std::istringstream ss( off );
    ss >> offset;
  }
const bool salmap_rv::TIMETYPE::is_time() const {
    if( t == ABSTIME || t == RELTIME )
      {
	return true;
      }
    else
      {
	return false;
      }
  }
const bool salmap_rv::TIMETYPE::is_idx() const {
    return !is_time();
  }
int64_t salmap_rv::TIMETYPE::time_from_current( const int64_t& current ) const {
    if( is_time() )
      {
	if( t == ABSTIME )
	  {
	    return offset;
	  }
	else
	  {
	    return current + offset;
	  }
      }
    else
      {
	fprintf(stderr, "REV: attempting to access time when type is not time!\n");
	exit(1);
      }
  }
int64_t salmap_rv::TIMETYPE::idx_from_current( int64_t& current ) {
    if( !is_time() )
      {
	if( t == ABSIDX )
	  {
	    return offset;
	  }
	else
	  {
	    return current + offset;
	  }
      }
    else
      {
	fprintf(stderr, "REV: attempting to access idx when type is not idx!\n");
	exit(1);
      }
  }
void salmap_rv::TIMETYPE::set_timetype( const std::string& type ) {
    if( same_string( "ABSTIME", type ) )
      {
	t = ABSTIME;
      }
    else if( same_string( "RELTIME", type ) )
      {
	t = RELTIME;
      }
    else if( same_string( "ABSIDX", type ) )
      {
	t = ABSIDX;
      }
    else if( same_string( "RELIDX" , type ) )
      {
	t = RELIDX;
      }
    else
      {
	
	
      }
  }
std::string salmap_rv::TIMETYPE::get_timetype() {
    if( t == ABSTIME )
      {
	return "ABSTIME";
      }
    else if( t == RELTIME )
      {
	return "RELTIME";
      }
    else if( t == ABSIDX )
      {
	return "ABSIDX";
      }
    else if( t == RELIDX )
      {
	return "RELIDX";
      }
    else
      {
	fprintf(stderr, "REV: timetype not set\n");
	exit(1);
      }
  }
