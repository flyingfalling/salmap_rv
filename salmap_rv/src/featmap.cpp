#include <salmap_rv/include/featmap.hpp>
#include <salmap_rv/include/util_functs.hpp>

using namespace salmap_rv;
void salmap_rv::FeatMap::set_relative_time( const int64_t& offset )
  {
    timevaltype = "RELTIME";
    timeval = tostring<int64_t>(offset);
  }

  void salmap_rv::FeatMap::set_absolute_time( const int64_t& offset )
  {
    timevaltype = "ABSTIME";
    timeval = tostring( offset );
  }

  void salmap_rv::FeatMap::set_absolute_index( const int64_t& offset )
  {
    timevaltype = "ABSIDX";
    timeval = tostring(offset); 
  }
  
  void salmap_rv::FeatMap::set_relative_index( const int64_t& offset )
  {
    timevaltype = "RELIDX";
    timeval = tostring(offset);
  }

  bool salmap_rv::FeatMap::is_relative_index()
  {
    if( same_string( "RELIDX", timevaltype ) )
      {
	return true;
      }
    else
      {
	return false;
      }
  }

  bool salmap_rv::FeatMap::is_absolute_index()
  {
    if( same_string( "ABSIDX", timevaltype ) )
      {
	return true;
      }
    else
      {
	return false;
      }
  }
  
  bool salmap_rv::FeatMap::is_relative_time()
  {
    if( same_string( "RELTIME", timevaltype ) )
      {
	return true;
      }
    else
      {
	return false;
      }
  }

  bool salmap_rv::FeatMap::is_absolute_time()
  {
    if( same_string( "ABSTIME", timevaltype ) )
      {
	return true;
      }
    else
      {
	return false;
      }
  }

  std::string salmap_rv::FeatMap::toString() const
  {
    
    std::string tmp = ( localname + "@" + timevaltype + "@" + timeval + filter.toStringStub() );
    return tmp;
  }

  std::string salmap_rv::FeatMap::toStringNoNick() const
  {
    
    fprintf(stdout, "Calling in map!\n");
    std::string tmp = ( localname + "@" + timevaltype + "@" + timeval + filter.toStringNoNickStub() );
    return tmp;
  }
