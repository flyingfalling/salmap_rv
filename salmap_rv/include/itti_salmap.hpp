#pragma once

#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>

namespace salmap_rv
{
  param_set itti_formal_default_params();
  param_set itti_formal_1998_params( const int wid, const int fps );
  void make_itti_dyadic_cpu_weighted_formal(  const param_set& p, SalMap& mysalmap, std::shared_ptr<ThreadPool> _tp );
}

