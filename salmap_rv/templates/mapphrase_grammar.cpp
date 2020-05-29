
using namespace salmap_rv;

template <typename Iterator>
salmap_rv::mapphrase_grammar<Iterator>::mapphrase_grammar()
  : salmap_rv::mapphrase_grammar<Iterator>::base_type(node)
{
  using qi::eps;
  using qi::lit;
  using qi::lexeme;
    
  using ascii::char_;
  using ascii::string;
  using namespace qi::labels;
  using phoenix::at_c;
  using phoenix::push_back;
    
  using qi::on_error;
  using qi::fail;
    
  using phoenix::construct;
  using phoenix::val;

    
  node0 =
    lexeme[*(char_ - '@')]
    ;
    
  algo =
    lexeme[+(char_ - '@')]
    ;

  nickname =
    lexeme[*(char_ - '@' - ',' - ']')] 
    ;

  desc =
    lexeme[*(char_ - '@')]
    ;

  
  
  


  
  node =
    node0 [at_c<0>(_val) = _1] 
    > lit('@')
    > node0 [at_c<4>(_val) = _1] 
    > lit('@')
    > node0 [at_c<5>(_val) = _1] 
    > lit('@')
    > nickname [at_c<6>(_val) = _1] 
    > -(   
	lit('@')
	> desc [at_c<7>(_val) = _1] 
	> lit('@')
	> algo [at_c<1>(_val) = _1] 
	> lit('@')
	> lit("PARAMS[") 
	> params [at_c<3>(_val) = _1]
	> lit("]INPUTS[")
	> inputs [at_c<2>(_val) = _1]
	> lit(']')
	)
    ;

   
  input1 =
    lexeme[+(char_ - '=' - ']')]
    ;
    
  inputpair =
    
    input1 > lit('=') > node 
    ;
    
  
  inputs =
    -( inputpair  % ',' )
    ;
    
    
  param1 =
    lexeme[+(char_ - '=' - ']')]
    ;
    
  param2 =
    lexeme[+(char_ - ']' - ',') ]
    ;

  parampair =
    param1 > lit('=') > param2
    ;

  
  params =
    -( parampair % ',' )
    ;
    
  node.name("node");
  
  
  node0.name("node0");
  algo.name("algo");
  nickname.name("nickname");
  desc.name("desc");
  inputs.name("inputs");
  inputpair.name("inputpair");
  input1.name("input1");
  params.name("params");
  parampair.name("parampair");
  param1.name("param1");
  param2.name("param2");
    
  on_error<fail>
    (
     node
     , std::cout
     << val("Error! Expecting ")
     << _4                               
     << val(" here: \"")
     << construct<std::string>(_3, _2)   
     << val("\"")
     << std::endl
     );
  
}
