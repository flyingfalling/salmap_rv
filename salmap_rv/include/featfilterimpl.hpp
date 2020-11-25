#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

#include <salmap_rv/include/algorithmimpl.hpp>
#include <salmap_rv/include/compiled_map.hpp>

#include <salmap_rv/include/featmapcollectionptr.hpp>


namespace salmap_rv {

  struct FeatFilterImpl
  {
    std::string name; 
    algorithmImpl algorithm;  
    
    std::map<std::string,std::string> inputs;
    
    std::map<std::string,compiled_map> compiled_inputs;

    std::vector<FeatMapImplPtr> ptrinputs; 
    FeatMapCollectionPtr ptrcollection;
    
    compiled_map compiled_name; 

    bool inputs_compiled;
    bool collection_compiled;

    std::map<std::string, std::shared_ptr<FeatMapImplInst>> localinputs;

    void set_param( const std::string& fullparamname, const std::string& newval ); 
    
    std::map<std::string,std::string> get_params( ) const;
      
    std::string toString( const FilterSet& fs ) const;
    void clear();
    FeatFilterImpl();
    size_t estimate_size_in_memory();
    void compile( const FeatFilterImplSet& filtimpls, const FilterSet& fs, const std::shared_ptr<FeatMapImplSet>& mapsetptr );
    
    bool check_fill_inputs( const std::shared_ptr<FeatMapImplSet>& maps, const uint64_t& curr_time, localmapdict& retlocalinputs, const FeatFilterImplSet& filtimpls, const FilterSet& fs
#ifdef GPU
			    , cv::cuda::Stream& stream
#endif
			    );
    bool update( const std::shared_ptr<FeatMapImplSet>& maps, const uint64_t& curr_time, const FeatFilterImplSet& filtimpls, const FilterSet& fs
#ifdef GPU
		 , cv::cuda::Stream& stream
#endif
	       );
};
}
