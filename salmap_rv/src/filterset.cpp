 

#include <salmap_rv/include/filterset.hpp>

#include <salmap_rv/include/featfilter.hpp>
#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/Timer.hpp>
#include <salmap_rv/include/mapphrase.hpp>
#include <salmap_rv/include/phraseparser.hpp>
#include <salmap_rv/include/featmap.hpp>

using namespace salmap_rv;

std::string salmap_rv::FilterSet::newnick() {
    
    ++nickname_ctr;
    return nickname_string+stringify_num(nickname_ctr);  
  }
 salmap_rv::FilterSet::FilterSet() : nickname_ctr(0)
  {  }
void salmap_rv::FilterSet::clear() {
    filters.clear();
    nickname_ctr=0; 
    
  }
std::string salmap_rv::FilterSet::add_filter( FeatFilter& f ) {
    
    
    

    DEBUGPRINTF(stdout, "FilterSet: Attempting to add a filter...[%s]\n", f.nickname.c_str());
    if( f.nickname.empty() )
      {
	f.nickname = newnick();
	DEBUGPRINTF(stdout, "Adding no-nickname filter, new nickname is [%s]\n", f.nickname.c_str() );
      }
    
    auto it = filters.find( f.nickname );
    if( it != filters.end() )
      {
	fprintf(stderr, "REV: attempting to double-add a filter... [%s] (%s)\n", f.nickname.c_str(), f.toString().c_str() );
	exit(1);
      }
    else
      {
	filters[f.nickname] = f;
	return f.nickname;
      }
  }
void salmap_rv::FilterSet::check_filters( const bool consolidate ) {
    size_t produced=0;
    
    Timer ft;

    
    
    std::list<std::map<std::string,FeatFilter>::iterator> doneset;

    DEBUGPRINTF(stdout, "Will check filters...\n");
    do
      {
	produced = 0;
	for( auto it=filters.begin(); it!=filters.end(); ++it )
	  {
	    
	    if( std::find( doneset.begin(), doneset.end(), it ) == doneset.end() )
	      {
		FeatFilter f = it->second;
		for( auto it2=f.inputs.begin(); it2!=f.inputs.end(); ++it2 )
		  {
		    
		    std::string retfiltname;
		    
		    bool success = add_filter_by_string( it2->second, retfiltname ); 
		    if( true == success ) 
		      {
			++produced;
		      }
		  }
		
		doneset.push_back( it );
		if( produced > 0 )
		  {
		    
		  }
	      }
	  }
      } while( produced > 0 );

    if(consolidate)
      {
	fprintf(stderr, "CONSOLIDATING FILTERS!\n");
	consolidate_filters();
      }
        
    DEBUGPRINTF(stdout, "Checking filters took %lf msec\n", ft.elapsed()*1e3);
}


std::string salmap_rv::FilterSet::enum_filters()
  {
    std::string output="";
    for( auto it = filters.begin(); it != filters.end(); ++it )
      {
	output += "-----------------------------\n";
	output += "FILTER: " + it->first + "\n";
	mapphrase mp( it->second );
	output += mp.print_pretty();
	output += "-----------------------------\n";
      }
    return output;
  }







bool salmap_rv::FilterSet::add_filter_by_string( const std::string& s, std::string& filtername )
{
  phraseparser salmaprvparser;

  
  mapphrase p = salmaprvparser.parse( s );
  FeatFilter f = p.toFilter();
  if( true == f.algorithm.empty() ) 
    {
      return false;
    }
  else
    {
      filtername = f.nickname; 
      if( filtername.empty() )
	{
	  filtername = newnick();
	}
      FeatFilter* ret;
      bool exists=element_in_map<FeatFilter>( filters, filtername, ret ); 
      if( false == exists )
	{
	  filters[filtername] = f;
	  return true;
	}
      else
	{
	  fprintf(stderr, "REV: attempting to double-add filter to filterset! nickname (8 mar 2020 this should never happen since there are never nested descriptions of filters): [%s]\n", filtername.c_str() );
	  exit(1);
	  return false;
	}
    }
}



FeatFilter& salmap_rv::FilterSet::getFullFilter( const std::string arg )
  {
    phraseparser salmaprvparser;
    mapphrase p = salmaprvparser.parse( arg );
    FeatFilter f = p.toFilter();
    if( f.nickname.empty() )
      {
	fprintf(stderr, "Nicknameless filter? [%s]\n", arg.c_str());
	exit(1);
      }
    else
      {
	auto it = filters.find(f.nickname);
	if( it == filters.end() )
	  {
	    fprintf(stderr, "REV: filter [%s] does not exist in filterset... (getFullFilter())\n", arg.c_str() );
	    exit(1);
	  }
	else
	  {
	    return it->second;
	  }
      }
  }




void salmap_rv::FilterSet::consolidate_filters()
  {
    phraseparser pp;
    bool doit = true;
    while( doit )
      {
	
	
	std::map<std::string,std::set<std::string>> samenicks;
	std::map<std::string,std::string> cleannames;
	
	for( auto it = filters.begin(); it != filters.end(); ++it )
	  {
	    
	    std::string tofind = it->second.toStringNoNick();
	    
	    auto it2 = cleannames.find( tofind );
	    if( it2 == cleannames.end() )
	      {
		cleannames[tofind] = it->first;
	      }
	    else 
	      {
		
		
		samenicks[tofind].emplace( it->first );
		samenicks[tofind].emplace( it2->second );
	      }
	  }


	if( samenicks.size() > 0 )
	  {
	    

	    
	    auto myculprit = samenicks.begin();
	    DEBUGPRINTF(stdout, "Detected filter to merge! [%s]\n", myculprit->first.c_str() );
	    std::string newnick="merged";
	    std::string newdesc="Merged:";
	    for( auto& nick : myculprit->second )
	      {
		newnick += "_" + nick;
		DEBUGPRINTF(stdout, "\tShould merge [%s] (erasing...)\n", nick.c_str() );
		auto toerase = filters.find(nick);
		
		if( toerase == filters.end() )
		  {
		    fprintf(stderr, "REV: lol wtf\n");
		    exit(1);
		  }
		std::string desc = toerase->second.desc;
		newdesc += " (" + desc + ")";
		filters.erase( toerase );
	      }
	    
	    

	    
	    
	    for( auto& nick : myculprit->second )
	      {
		substitute_instances( nick, newnick, pp );
	      }

	    auto p = pp.parse( myculprit->first );
	    filters[newnick] = p.toFilter();
	    filters[newnick].nickname = newnick;
	    filters[newnick].desc = newdesc;
	    
	     
	  }
	else
	  {
	    doit=false;
	  }
	
	
      }
  }


std::string salmap_rv::FilterSet::recursive_replace_nick( const std::string& input, const std::string& oldnick, const std::string& newnick, phraseparser& pp )
  {
    mapphrase p = pp.parse( input );
    FeatMap fm = p.toMap();
    
    for( auto& i : fm.filter.inputs )
      {
	std::string newmap = recursive_replace_nick( i.second, oldnick, newnick, pp );
	mapphrase p2( newmap );
	if( issamestring( p2.nickname, oldnick ) )
	  {
	    p2.nickname = newnick;
	  }
	i.second = p2.toMap().toString();
      }
    return fm.toString();
  }

void salmap_rv::FilterSet::substitute_instances( const std::string oldnick, const std::string newnick, phraseparser& pp )
  {
    
    for( auto it = filters.begin(); it != filters.end(); ++it )
      {
	std::string filtstr = recursive_replace_nick( it->second.toString(), oldnick, newnick, pp );
	auto p = pp.parse( filtstr );
	it->second = p.toFilter(); 
      }
  }
