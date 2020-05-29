#include <salmap_rv/include/featfilter.hpp>

#include <salmap_rv/include/util_functs.hpp>
#include <salmap_rv/include/featmap.hpp>

using namespace salmap_rv;
salmap_rv::FeatFilter::FeatFilter()
    : algorithm(""), iterset(false)
  {
  }

 std::string salmap_rv::FeatFilter::getdesc() 
  {
    if( desc.empty() )
      {
	return nickname;
      }
    else
      {
	return desc;
      }
  }

std::map< std::string, FeatFilterImpl>::iterator salmap_rv::FeatFilter::getImplIter() 
  {
    if( false == iterset )
      {
	fprintf(stderr, "REV: attempting to get impliter without it being set!\n");
	exit(1);
      }
    else
      {
	return impliter;
      }
  }
  
  void salmap_rv::FeatFilter::setImplIter( const std::map< std::string, FeatFilterImpl>::iterator newiter )
  {
    iterset = true;
    impliter = newiter;
  }

size_t salmap_rv::FeatFilter::estimate_size_in_memory()
  {
    size_t s=0;
    s+=algorithm.size();
    s+=size_in_mem_strstrmap( params );
    s+=size_in_mem_strstrmap( inputs );
    return s;
  }
  

  void salmap_rv::FeatFilter::add_param( const std::string& s, const std::string& v )
  {
    params[s] = v;
  }
  
  
  void salmap_rv::FeatFilter::set_algorithm( const std::string& s )
  {
    algorithm = s;
  }
  
 
  
  void salmap_rv::FeatFilter::add_input( const std::string& s, const std::string& v )
  {
    inputs[s] = v;
  }

  std::string salmap_rv::FeatFilter::toStringNoNick() const
  {
    return ("@@" + toStringNoNickStub());
  }

void salmap_rv::FeatFilter::add_input( const std::string& s, FeatMap& f )
  {
    inputs[s] = f.toString();
  }


 std::string salmap_rv::FeatFilter::toStringNoNickStub() const
  {
    if( algorithm.empty() )
      {
	fprintf(stderr, "toStringNoNick with empty algo [%s] [%s]!\n", nickname.c_str(), toString().c_str());
	exit(1);
      }
    return ("@@@" + algorithm + "@PARAMS[" + print_str_str_map( params ) + "]INPUTS[" + print_str_str_map( inputs ) + "]");
  }
  
  
  std::string salmap_rv::FeatFilter::toStringStub() const
  {
    if( algorithm.empty() )
      {
	return ("@" + nickname);
      }
    else
      {
	
	return ("@" + nickname + "@" + desc + "@" + algorithm + "@PARAMS[" + print_str_str_map( params ) + "]INPUTS[" + print_str_str_map( inputs ) + "]"); 
      }
    }

std::string salmap_rv::FeatFilter::toString() const
  {
    std::string tmp = ("@@" + toStringStub());
    
    return tmp;
  }
