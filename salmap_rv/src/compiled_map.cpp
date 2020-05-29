#include <salmap_rv/include/compiled_map.hpp>

#include <salmap_rv/include/phraseparser.hpp>

#include <salmap_rv/include/mapphrase.hpp>

#include <salmap_rv/include/featfilterimplset.hpp>
#include <salmap_rv/include/featfilterimpl.hpp>

using namespace salmap_rv;
void salmap_rv::compiled_map::set_localname( const std::string& ln )
  {
    m.localname = ln;
  }
  
  std::string salmap_rv::compiled_map::name_no_time_no_localname() const
  {
    return m2.toString();
  }

  std::string salmap_rv::compiled_map::get_localname() const
  {
    return m.localname;
  }
  
  salmap_rv::compiled_map::compiled_map( )
  {
  }

  
  
  salmap_rv::compiled_map::compiled_map( const std::string& name )
  {
    phraseparser salmaprvparser;
    mapphrase p = salmaprvparser.parse( name );
    m = p.toMap();
  }


salmap_rv::compiled_map::compiled_map( const std::string& name, const FeatFilterImplSet& filtimpls, const FilterSet& fs  )
  {
    phraseparser salmaprvparser;
    
    mapphrase p = salmaprvparser.parse( name );
    std::string timevaltype = p.timevaltype;
    std::string timeval = p.timeval;
    std::string localname = p.localname;
    
    std::string s = p.print_pretty();
    
    if( p.algorithm.empty() )
      {
	DEBUGPRINTF(stdout, "Attempting to compile map with no algo name...finding impl [%s]\n", name.c_str() );
	auto iter = filtimpls.filters.find( p.nickname ); 
	if( iter == filtimpls.filters.end() )
	  {
	    fprintf(stderr, "REV: didn't exist in filtimpls: (yet another error wtf) [%s]\n", name.c_str() );
	    exit(1);
	  }
	
	std::string newname = iter->second.toString( fs ); 
	p = salmaprvparser.parse( newname );

	
	p.timevaltype = timevaltype;
	p.timeval = timeval;
	p.localname = localname;
	
	
      }

    
    m = p.toMap();
    m2 = m;
    m2.timevaltype="";
    m2.timeval="";
    m2.localname="";
  }
