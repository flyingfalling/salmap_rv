#include <salmap_rv/include/param_set.hpp>

#include <salmap_rv/include/util_functs.hpp>

using namespace salmap_rv;

 salmap_rv::param_set::param_set() {  }
bool salmap_rv::param_set::exists( const std::string n ) const {
    auto iter = dict.find(n);
    if( iter == dict.end() )
      {
	return false;
      }
    else
      {
	return true;
      }
  }
std::string salmap_rv::param_set::tostring( const std::variant<std::string, int64_t, float64_t, bool> v ) const {
    if( std::holds_alternative<float64_t>(v) ) { return std::to_string( std::get<float64_t>(v) ); }
    else if( std::holds_alternative<int64_t>(v) ) { return std::to_string( std::get<int64_t>(v) ); }
    else if( std::holds_alternative<std::string>(v) ) { return std::get<std::string>(v); }
    else if( std::holds_alternative<bool>(v) ) { return true==std::get<bool>(v) ? "true" : "false"; }
    else { fprintf(stderr, "tostring failed, unrecognized alternative\n"); exit(1); }
  }
void salmap_rv::param_set::enumerate() const {
    for( auto i=dict.begin(); i != dict.end(); ++i )
      {
	std::cout << i->first << " : " << tostring(i->second) << std::endl;
      }
  }
void salmap_rv::param_set::tofile( const std::string& fname ) {
    std::ofstream ostrm;
    ostrm.open( fname );
    if( !ostrm.is_open() )
      {
	fprintf(stderr, "What no ostrm open? [%s]\n", fname.c_str() );
	exit(1);
      }
    ostrm << header << std::endl;
    
    std::string mymark;

    for( auto v : dict )
      {
	if( std::holds_alternative<float64_t>(v.second) ) { mymark=Fmark; }
	else if( std::holds_alternative<int64_t>(v.second) ) { mymark=Imark; }
	else if( std::holds_alternative<std::string>(v.second) ) { mymark=Smark; }
	else if( std::holds_alternative<bool>(v.second) ) { mymark=Bmark; }
	else { fprintf(stderr, "Tofile failed, unrecognized alternative\n"); exit(1); }
	  
	
	ostrm << v.first << " " << mymark << " " << tostring( v.second ) << std::endl; 
      }
  }
void salmap_rv::param_set::fromfile( const std::string& fname ) {
    std::ifstream istrm;
    istrm.open( fname ); 
    if( !istrm.is_open() )
      {
	fprintf(stderr, "What no istrm open? [%s]\n", fname.c_str() );
	exit(1);
      }
    std::string line;
    while( std::getline(istrm, line) )
      {
	std::stringstream iss( line );
	std::string tmp;
	if( iss >> tmp )
	  {
	    if( tmp[0] == '#' )
	      {
		
	      }
	    else
	      {
		
		std::string name = tmp;
		std::string type;
		if( iss >> type )
		  {
		    if( issamestring( type, Fmark ) )
		      {
			float64_t f;
			iss >> f;
			set<float64_t>( name, f );
		      }
		    else if( issamestring( type, Imark ) )
		      {
			int64_t i;
			iss >> i;
			set<int64_t>( name, i );
		      }
		    else if( issamestring( type, Bmark ) )
		      {
			bool b;
			iss >> std::boolalpha >> b;
			set<bool>( name, b );
		      }
		    else if( issamestring( type, Smark ) )
		      {
			std::string s;
			iss >> s;
			set<std::string>( name, s );
		      }
		    else
		      {
			fprintf(stdout, "[%s] not recognized type (FIBS)\n", type.c_str());
			exit(1);
		      }
		  }
		else
		  {
		    fprintf(stdout, "Incorrect format of param_set...\n");
		    exit(1);
		  }
	      }
	  } 
      } 
    
    
    
  }

