#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>



#include <ctime>
#include <chrono> 

namespace salmap_rv
{
  struct Timer {
    Timer();
    void reset();
    double elapsed() const;
    typedef std::chrono::high_resolution_clock clock_; 
    typedef std::chrono::duration<double, std::ratio<1> > second_; 
    std::chrono::time_point<clock_> beg_; 
  };
}
