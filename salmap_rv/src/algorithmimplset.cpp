#include <salmap_rv/include/algorithmimplset.hpp>
#include <salmap_rv/include/featmapimplinst.hpp>


using namespace salmap_rv;
void salmap_rv::algorithmImplSet::clear()
  {
    algorithms.clear();
  }
  
void salmap_rv::algorithmImplSet::register_algorithm( const std::string& name, const algoFunct_t updatef, const algoFunct_t initf, const algoFunct_t destroyf )
  {
    algorithmImpl a( name );

    auto i = algorithms.find( name );
    if( i == algorithms.end() )
      {
	a.setUpdate( updatef );
	a.setInit( initf );
	a.setDestroy( destroyf );
	algorithms[ name ] = a;
      }
    else
      {
	fprintf(stderr, "REV: attempting to add algorithm that exists [%s]\n", name.c_str() );
	exit(1);
      }
  }

  bool salmap_rv::algorithmImplSet::get( const std::string& name, algorithmImpl& reta )
  {
    auto iter = algorithms.find(name);
    if( iter == algorithms.end() )
      {
	return false;
      }
    else
      {
	reta = iter->second;
	return true;
      }
  }
