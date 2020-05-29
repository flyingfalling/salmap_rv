
#include <salmap_rv/include/phraseparser.hpp>

#include <salmap_rv/include/mapphrase_grammar.hpp>

using namespace salmap_rv;
mapphrase salmap_rv::phraseparser::parse( const std::string& storage ) {
    mapphrase myphrase;
    
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
    bool r = phrase_parse(iter, end, grammar, boost::spirit::ascii::space, myphrase); 
    if( !r )
      {
	fprintf(stderr, "Parse error (parsing: [\n%s\n])\n", storage.c_str());
	exit(1);
      }
    else
      {
	return myphrase;
      }
  }
