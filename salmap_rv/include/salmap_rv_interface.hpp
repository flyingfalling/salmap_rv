#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>







int salmap_vers();

void* salmap_ptr_init( const std::string& _paramset_fname, const double input_dva_wid, const double input_dva_hei );

void salmap_ptr_uninit( void* _salmap );

void salmap_add_input( void* _salmap, const std::string& _inputmapname, cv::Mat& inputmat );

int64_t salmap_update( void* _salmap );

  
cv::Mat salmap_get_map_now_nickname( void* _salmap, const std::string& _filtername, const std::string& _mapname );
  
  
cv::Mat salmap_get_map_pretty( void* _salmap, const std::string& _filtername, const std::string& _mapname, const std::string& _overlaywithinputmap, const double alpha, const int resize_wid_pix );


