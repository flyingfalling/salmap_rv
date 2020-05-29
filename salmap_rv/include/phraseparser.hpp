#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/mapphrase_grammar.hpp>

namespace salmap_rv
{

struct phraseparser {
  mapphrase_grammar<std::string::const_iterator> grammar; 
  mapphrase parse( const std::string& storage );
};
}
