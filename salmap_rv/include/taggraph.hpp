#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>


namespace salmap_rv
{
  struct algonode {
    std::string algo; 
    std::map<std::string,std::string> params; 
    std::string nickname; 
    algonode( const std::string& a,
	      const std::map<std::string,std::string>& m,
	      const std::string& n="" );
    bool sameas( const algonode& a1 ) const;
  };

  bool operator==( const algonode& a1, const algonode& a2 );
  struct timenode {
    int64_t reltime_nsec; 
    bool sameas(const timenode& t1 ) const;
    timenode( const int64_t& r );
  };
  bool operator==( const timenode& a1, const timenode& a2 );
  struct mapnode {
    std::string localname; 
    bool sameas(const mapnode& t1 ) const;
    mapnode( const std::string& i );
  };
  bool operator==( const mapnode& a1, const mapnode& a2 );
  struct inputnode {
    std::string inputname; 
    bool sameas(const inputnode& t1 ) const;
    inputnode( const std::string& i );
  };
  bool operator==( const inputnode& a1, const inputnode& a2 );

  struct tagnode {
    std::variant<algonode,timenode,mapnode,inputnode> details; 
    int64_t abstime_nsec; 
    std::deque< std::shared_ptr<tagnode> > efferents; 
    std::deque< std::shared_ptr<tagnode> > afferents; 
    bool is_algo() const;
    bool is_time() const;
    bool is_map() const;
    bool is_input() const;
    std::shared_ptr<tagnode> add_post( tagnode arg );
    std::shared_ptr<tagnode> add_post_ptr( std::shared_ptr<tagnode>& ptr );
    bool sameas( const tagnode& t ) const;
    std::shared_ptr<tagnode> get_next( tagnode arg );

    template <typename T>
    tagnode( const T& a );
    
    template <typename T>
    std::shared_ptr<tagnode> get_next( const T& a );
  
    
    template <typename T>
    std::shared_ptr<tagnode> get_prev( const T& a );
  
    
    template <typename T>
    T& get();
 
  };

  struct taggraph {
    std::deque< std::shared_ptr<tagnode> > nodes; 
    std::map<std::string,std::shared_ptr<tagnode>> roots; 
    std::shared_ptr<tagnode> add_node( const tagnode arg );
    std::shared_ptr<tagnode> add_root_algo( const std::string& rootnick, algonode a );
    std::shared_ptr<tagnode> specify_root_algo( const std::string& rootnick, const std::shared_ptr<tagnode>& ptr );
    std::shared_ptr<tagnode> add_post_to_node( std::shared_ptr<tagnode>& node, const tagnode arg );
    size_t get_node_idx( std::shared_ptr<tagnode>& ptr );
    void clear_make_afferents();
    std::string graph_to_graphviz();
  };
}

using namespace salmap_rv;

template <typename T>
tagnode::tagnode( const T& a )
    : details(a)
  {}

template <typename T>
std::shared_ptr<tagnode> tagnode::get_next( const T& a )
  {
    for( auto& p : efferents )
      {
	
	if( std::holds_alternative<T>(p->details) &&
	    a.sameas(std::get<T>(p->details)) )
	  {
	    return p;
	  }
      }
    
    return nullptr;
  }

  template <typename T>
  std::shared_ptr<tagnode> tagnode::get_prev( const T& a )
  {
    for( auto& p : afferents )
      {
	if( std::holds_alternative<T>(p->details) &&
	    a.sameas(std::get<T>(p->details)) )
	  {
	    return p;
	  }
      }
    return nullptr;
  }
  
  template <typename T>
  T& tagnode::get()
  {
    if( false == std::holds_alternative<T>(details) )
      {
	fprintf(stderr, "Illegal get, wrong type...\n");
	exit(1);
      }
    return std::get<T>(details);
  }
