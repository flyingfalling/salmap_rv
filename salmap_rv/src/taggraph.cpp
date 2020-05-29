#include <salmap_rv/include/taggraph.hpp>

#include <salmap_rv/include/util_functs.hpp>

using namespace salmap_rv;

algonode::algonode( const std::string& a,
		    const std::map<std::string,std::string>& m,
		    const std::string& n ) : algo(a), params(m), nickname(n)
{}
bool algonode::sameas( const algonode& a1 ) const {
  
  if( false == issamestring(a1.algo,algo) ||
      a1.params.size() != params.size() )
    {
      return false;
    }
    
  for( auto i1=a1.params.begin(); i1 != a1.params.end(); ++i1 )
    {
      auto i2 = params.find(  i1->first );
	
      if( i2 == params.end() ||
	  false == issamestring( i2->second, i1->second ) )
	{
	  return false;
	}
    }
    
  return true;
}

bool salmap_rv::operator==( const algonode& a1, const algonode& a2 ) {
  
  if( false == issamestring(a1.algo,a2.algo) ||
      a1.params.size() != a2.params.size() )
    {
      return false;
    }
    
  for( auto i1=a1.params.begin(); i1 != a1.params.end(); ++i1 )
    {
      auto i2 = a2.params.find(  i1->first );
	
      if( i2 == a2.params.end() ||
	  false == issamestring( i2->second, i1->second ) )
	{
	  return false;
	}
    }
    
  return true;
}

bool timenode::sameas(const timenode& t1 ) const {
  
  return (t1.reltime_nsec == reltime_nsec);
}

timenode::timenode( const int64_t& r ) : reltime_nsec( r )
{}

bool salmap_rv::operator==( const timenode& a1, const timenode& a2 ) {
  
  return (a1.reltime_nsec == a2.reltime_nsec);
}

bool mapnode::sameas(const mapnode& t1 ) const {
  
  return issamestring( t1.localname, localname );
}

mapnode::mapnode( const std::string& i ) : localname(i)
{}

bool salmap_rv::operator==( const mapnode& a1, const mapnode& a2 ) {
  
  return issamestring( a1.localname, a2.localname );
}

bool inputnode::sameas(const inputnode& t1 ) const {
  
  return issamestring( t1.inputname, inputname );
}

inputnode::inputnode( const std::string& i ) : inputname(i)
{}

bool salmap_rv::operator==( const inputnode& a1, const inputnode& a2 ) {
  
  return issamestring( a1.inputname, a2.inputname );
}

bool tagnode::is_algo() const {
  if( std::holds_alternative<algonode>(details) )
    {
      return true;
    }
  return false;
}

bool tagnode::is_time() const {
  if( std::holds_alternative<timenode>(details) )
    {
      return true;
    }
  return false;
}

bool tagnode::is_map() const {
  if( std::holds_alternative<mapnode>(details) )
    {
      return true;
    }
  return false;
}

bool tagnode::is_input() const {
  if( std::holds_alternative<inputnode>(details) )
    {
      return true;
    }
  return false;
}

std::shared_ptr<tagnode> tagnode::add_post( tagnode arg ) {
  
  
  
  auto tmp = std::make_shared<tagnode>(arg);
  efferents.push_back(tmp);
  
  return tmp;
}

std::shared_ptr<tagnode> tagnode::add_post_ptr( std::shared_ptr<tagnode>& ptr ) {
  
  
  
  efferents.push_back( ptr );
  
  return ptr;
}

bool tagnode::sameas( const tagnode& t ) const {
  if( t.details == details )
    {
      return true;
    }
  else
    {
      return false;
    }
}

std::shared_ptr<tagnode> tagnode::get_next( tagnode arg ) {
  for( auto& p : efferents )
    {
      if( arg.sameas(*p) )
	{
	  return p;
	}
    }
  return nullptr;
}










std::shared_ptr<tagnode> taggraph::add_node( const tagnode arg ) {
  nodes.push_back( std::make_shared<tagnode>(arg) );
  return nodes.back();
}

std::shared_ptr<tagnode> taggraph::add_root_algo( const std::string& rootnick, algonode a ) {
  auto it = roots.find( rootnick );
  if( it != roots.end() )
    {
      fprintf(stderr, "Adding root with nick that already exists...[%s]...\n", rootnick.c_str() );
      exit(1);
    }
  roots[rootnick] = add_node( tagnode(a) );
  return roots[rootnick];
}

std::shared_ptr<tagnode> taggraph::specify_root_algo( const std::string& rootnick, const std::shared_ptr<tagnode>& ptr ) {
  auto it = roots.find( rootnick );
  if( it != roots.end() )
    {
      fprintf(stderr, "Adding root with nick that already exists...[%s]...\n", rootnick.c_str() );
      exit(1);
    }
  roots[rootnick] = ptr;
  return roots[rootnick];
}

std::shared_ptr<tagnode> taggraph::add_post_to_node( std::shared_ptr<tagnode>& node, const tagnode arg ) {
  
  nodes.push_back( node->add_post( arg ) );
  return nodes.back();
}

size_t taggraph::get_node_idx( std::shared_ptr<tagnode>& ptr ) {
  auto itr = std::find( nodes.begin(), nodes.end(), ptr );
  if( itr == nodes.end() )
    { fprintf(stderr, "Node not found...\n"); exit(1); }
  else
    {
      return std::distance( nodes.begin(), itr );
      
    }
}

void taggraph::clear_make_afferents() {
  for( auto& n : nodes )
    {
      n->afferents.clear();
    }
  for( auto& n : nodes )
    {
      for( auto& e : n->efferents )
	{
	  e->afferents.push_back( n );
	}
    }
}

std::string taggraph::graph_to_graphviz() {
  fprintf(stdout, "Printing graph...\n");

  clear_make_afferents();
    
  std::string mystring="";
  mystring+="digraph g {\n";
  
  mystring+="rankdir=LR;\n";
  

  
  

#define CLUSTERMETHOD
#define HTML_NEWLINE_STR "&#92;n"
#ifdef CLUSTERMETHOD

  size_t clusteridx=0;
  std::string idxstr;
  std::string shapestr;
  std::string labelstr;

    
  for( auto& n : nodes )
    {
      if( n->is_algo() )
	{
	  
	  std::string clusterstr = "cluster"+std::to_string(clusteridx);
	  ++clusteridx;
	    
	  
	  mystring+= "subgraph " + clusterstr + " {\n";
	    
	  idxstr = std::to_string( get_node_idx(n) );
	    
	  
	    
	  labelstr = n->get<algonode>().nickname;
	  
	  mystring+="label = \"" + labelstr + "\";";
	    

	  
	  shapestr = "record";
	  labelstr = n->get<algonode>().algo;
	  labelstr += " | ";
	  auto params = n->get<algonode>().params;
	  for( auto& p : params )
	    {
	      labelstr += p.first + " : " + p.second + HTML_NEWLINE_STR;
	    }

	  mystring+="node [shape=" + shapestr + " label=\"" + labelstr +  "\"] " + idxstr + ";\n";
	    
	    
	  
	  for( auto& p : n->afferents )
	    {
	      if( false == p->is_input() )
		{
		  fprintf(stderr, "Wtf input is not input?\n");
		  exit(1);
		}
	      idxstr = std::to_string( get_node_idx(p) );
	      shapestr = "oval"; labelstr = p->get<inputnode>().inputname;
	      mystring+="node [shape=" + shapestr + " label=\"" + labelstr +  "\"] " + idxstr + ";\n";

	      mystring += idxstr + " -> " + std::to_string( get_node_idx( n ) ) + ";\n";
		
	    }
	    
	  
	  for( auto& p : n->efferents )
	    {
	      if( false == p->is_map() )
		{
		  fprintf(stderr, "Wtf map is not map?\n");
		  exit(1);
		}

	      idxstr = std::to_string( get_node_idx(p) );
	      shapestr = "oval"; labelstr = p->get<mapnode>().localname;
	      mystring+="node [shape=" + shapestr + " label=\"" + labelstr +  "\"] " + idxstr + ";\n";
		
	      mystring += std::to_string( get_node_idx( n ) ) + " -> " + idxstr + ";\n";
	    }
	    
	  
	  mystring+="\n}\n";
	} 
      else if( n->is_time() )
	{
	  idxstr = std::to_string( get_node_idx(n) );
	  shapestr = "diamond"; labelstr = std::to_string(n->get<timenode>().reltime_nsec/1e6) + " ms";
	  mystring+="node [shape=" + shapestr + " label=\"" + labelstr +  "\"] " + idxstr + ";\n";

	  for( auto& a : n->afferents )
	    {
	      mystring += std::to_string( get_node_idx( a ) ) + " -> " + idxstr + ";\n";
	    }

	  for( auto& e : n->efferents )
	    {
	      mystring += idxstr + " -> " + std::to_string( get_node_idx( e ) ) + ";\n";
	    }
	}
      else
	{
	  
	}
    }
    
#else 
    
  
  for( idx=0; idx < nodes.size(); ++idx )
    {
      idxstr=std::to_string(idx);
      auto n=nodes[idx];
	
      if( n->is_algo() )
	{
	  shapestr = "box";
	  labelstr = n->get<algonode>().algo;
	  labelstr += "\n";
	  labelstr += n->get<algonode>().nickname;
	  labelstr += "\n";
	  auto params = n->get<algonode>().params;
	  for( auto& p : params )
	    {
	      labelstr += p.first + " : " + p.second + "\n";
	    }

	  fprintf(stdout, "Algo node [%s] has %ld afferents\n", n->get<algonode>().nickname.c_str(), n->afferents.size() );
	}
      else if( n->is_time() )
	{ shapestr = "diamond"; labelstr = std::to_string(n->get<timenode>().reltime_nsec/1e6) + " msec" ; }
      else if( n->is_input() )
	{ shapestr = "circle"; labelstr = n->get<inputnode>().inputname; }
      else if( n->is_map() )
	{ shapestr = "circle"; labelstr = n->get<mapnode>().localname; }
      else
	{ exit(1); }
	
      
      mystring+="node [shape=" + shapestr + " label=\"" + labelstr +  "\"] " + idxstr + ";\n";
      
      
	
    }
  
  for( idx=0; idx < nodes.size(); ++idx )
    {
      idxstr=std::to_string(idx);
      auto n=nodes[idx];
      for( auto& e : n->efferents )
	{
	  mystring += std::to_string(idx) + " -> " + std::to_string( get_node_idx( e ) ) + ";\n";
	}
    }
    
#endif 
    
    
  mystring+="}";

  
  return mystring;
}
