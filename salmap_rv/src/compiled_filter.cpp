#include <salmap_rv/include/compiled_filter.hpp>

#include <salmap_rv/include/phraseparser.hpp>
#include <salmap_rv/include/mapphrase.hpp>

using namespace salmap_rv;


salmap_rv::compiled_filter::compiled_filter()
  {
  }
  
  salmap_rv::compiled_filter::compiled_filter( const std::string& filt )
  {
    phraseparser salmaprvparser;
    mapphrase p = salmaprvparser.parse( filt );
    f = p.toFilter();
  }
