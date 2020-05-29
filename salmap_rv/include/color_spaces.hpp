#pragma once
#include <salmap_rv/include/salmap_rv_includes.hpp>
#include <salmap_rv/include/salmap_rv_types.hpp>
#include <salmap_rv/include/typedefs.hpp>
#include <salmap_rv/include/defines.hpp>








namespace salmap_rv
{

  
  double inv_gam_sRGB(int ic);

  
  int gam_sRGB(double v);

  
  int rgb_to_gray(int r, int g, int b);


  
  
  

  double YtoLstar(const double Y);

  
  
  

  
  
  
  void RGBtoHSP( const double  R, const double  G, const double  B,
		 double& H, double& S, double& P );


  void HSPtoRGB( const double  _H, const double  S, const double  P,
		 double& R, double& G, double& B );
}
