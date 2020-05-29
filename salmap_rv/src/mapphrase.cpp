
#include <salmap_rv/include/mapphrase.hpp>

#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/featfilter.hpp>
#include <salmap_rv/include/featmap.hpp>

#include <salmap_rv/include/phraseparser.hpp>

using namespace salmap_rv;

bool salmap_rv::mapphrase::is_filter() {
    std::string filtmarker=""; 
    bool localnameempty = same_string( filtmarker, localname );
    if( timevaltype.length() > 0 || timeval.length() > 0 )
      {
	if( true == localnameempty )
	  {
	    fprintf(stderr, "REV: error, local name is empty i.e. it's a filter, but it has a time\n");
	    exit(1);
	  }
      }
    
    return localnameempty;
  }
 salmap_rv::mapphrase::mapphrase() {}
std::map<std::string,std::string> salmap_rv::mapphrase::inputs_as_str_str() {
    std::map<std::string,std::string> m;
    for( auto it=inputs.begin(); it!=inputs.end(); ++it )
      {
	m[it->first] = it->second.print();
      }
    return m;
  }
std::string salmap_rv::mapphrase::enum_inputs( std::map< std::string, mapphrase >& m ) {
    std::string SEP=",";
    std::string result="";
    
    size_t size=0;
    for (auto it=m.begin(); it!=m.end(); ++it)
      {
	if(size > 0)
	  {
	    result += SEP;
	  }
	result += it->first + "=" + it->second.print();
	++size;
      }
    return(result);
  }
std::string salmap_rv::mapphrase::enum_params( std::map< std::string, std::string >& m ) {
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
std::string salmap_rv::mapphrase::print() {
    std::string s="";
    s += localname + "@";
    s += timevaltype + "@";
    s += timeval + "@";
    s += nickname;

    if( false==algorithm.empty() )
      {
	s += "@";
	s += desc + "@";
	s += algorithm + "@INPUTS[";
	s += enum_inputs( inputs );
	s += "]PARAMS[";
	s += enum_params( params );
	s += "]";
      }
    
    return(s);
  }
std::string salmap_rv::mapphrase::print_pretty( const size_t& tablevel ) {
    std::string mytab=maketab( tablevel );
    std::string s="";
    s += mytab + "LOCAL NAME: " + localname + "\n";
    s += mytab + "TIME VAL TYPE: " + timevaltype + "\n";
    s += mytab + "TIME VAL:      " + timeval + "\n";
    s += mytab + "NICKNAME:      " + nickname + "\n";
    s += mytab + "PRETTY DESC:   " + desc + "\n";
    s += mytab + "ALGORITHM:  " + algorithm + "\n";
    s += mytab + "INPUTS:\n";
    s += pretty_inputs( inputs, tablevel + 3);
    s += mytab + "PARAMS:\n";
    s += pretty_params( params, tablevel + 3 );
    return(s);
  }


salmap_rv::mapphrase::mapphrase( const std::string& s )
{
  phraseparser salmaprvparser;
  mapphrase p = salmaprvparser.parse( s );
  localname = p.localname;
  algorithm = p.algorithm;
  inputs = p.inputs;
  params = p.params;
  timevaltype = p.timevaltype;
  timeval = p.timeval;
  nickname = p.nickname; 
  desc = p.desc;
}
  
  

salmap_rv::mapphrase::mapphrase( FeatMap& map )
{
  phraseparser salmaprvparser;
  mapphrase p = salmaprvparser.parse(map.toString());
  localname = p.localname;
  algorithm = p.algorithm;
  inputs = p.inputs;
  params = p.params;
  timevaltype = p.timevaltype;
  timeval = p.timeval;
  desc = p.desc;
  nickname = p.nickname;
}


salmap_rv::mapphrase::mapphrase( FeatFilter& filt )
{
  phraseparser salmaprvparser;
  mapphrase p = salmaprvparser.parse(filt.toString());
  localname = "";
  algorithm = p.algorithm;
  inputs = p.inputs;
  params = p.params;
  timevaltype = "";
  timeval = "";
  nickname = p.nickname;
  desc = p.desc; 
}

FeatMap salmap_rv::mapphrase::toMap()
  {
    FeatMap m;
    m.localname = localname;
    m.filter.algorithm = algorithm;
    m.filter.inputs = inputs_as_str_str();
    m.filter.params = params;
    m.filter.desc = desc;
    m.filter.nickname = nickname;
    m.timevaltype = timevaltype;
    m.timeval = timeval;
    return m;
  }

FeatFilter salmap_rv::mapphrase::toFilter()
{
  FeatFilter f;
  f.algorithm = algorithm;
  f.inputs = inputs_as_str_str();
  f.params = params;
  f.desc = desc;
  f.nickname = nickname;
  return f;
}
