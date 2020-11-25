#pragma once
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>




#include <cstdlib>
#include <cstdio>
#include <string>

#include <fstream>
#include <vector>
#include <map>
#include <deque>
#include <list>
#include <set>

#include <mutex>
#include <thread>
#include <utility>

#include <limits>
#include <variant>
#include <memory>

#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>


#include <boost/fusion/include/std_pair.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>


#include <boost/tokenizer.hpp>


#include <opencv2/opencv.hpp>


#include <json/json.hpp>


#ifdef USE_OPENCL
#include <opencv2/core/ocl.hpp>
#endif


#include <salmap_rv/include/Timer.hpp>
