
#include <salmap_rv/include/Timer.hpp>

using namespace salmap_rv;

Timer::Timer() : beg_(clock_::now()) {}

void Timer::reset() { beg_ = clock_::now(); }

double Timer::elapsed() const {
    return std::chrono::duration_cast<second_>
      (clock_::now() - beg_).count();
  }
