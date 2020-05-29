#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/mapphrase.hpp>

BOOST_FUSION_ADAPT_STRUCT(
			  salmap_rv::mapphrase,
			  (std::string, localname)
			  (std::string, algorithm)
			  (salmap_rv::inputmap, inputs)
			  (salmap_rv::parammap, params)
			  (std::string, timevaltype)
			  (std::string, timeval)
			  (std::string, nickname)
			  (std::string, desc)
			  )
namespace salmap_rv
{
    
  template <typename Iterator>
  struct mapphrase_grammar : qi::grammar<Iterator, mapphrase(), ascii::space_type>
  {
    mapphrase_grammar();
  
    qi::rule<Iterator, mapphrase(), ascii::space_type> node;
    
    
    qi::rule<Iterator, std::string(), ascii::space_type> node0;
    qi::rule<Iterator, std::string(), ascii::space_type> algo;
    
    qi::rule<Iterator, std::string()> desc; 
    qi::rule<Iterator, std::string(), ascii::space_type> nickname; 
    qi::rule<Iterator, std::map<std::string,mapphrase>(), ascii::space_type> inputs;
    qi::rule<Iterator, std::string(), ascii::space_type> input1;
    qi::rule<Iterator, std::pair<std::string,mapphrase>(), ascii::space_type> inputpair;
    qi::rule<Iterator, std::map<std::string,std::string>(), ascii::space_type> params;
    qi::rule<Iterator, std::string(), ascii::space_type> param1;
    qi::rule<Iterator, std::string(), ascii::space_type> param2;
    qi::rule<Iterator, std::pair<std::string,std::string>(), ascii::space_type> parampair;
  };
}


#include <salmap_rv/templates/mapphrase_grammar.cpp>
