#include <salmap_rv/include/color_spaces.hpp>

using namespace salmap_rv;

double salmap_rv::inv_gam_sRGB(int ic)
{
    double c = ic/255.0;
    if ( c <= 0.04045 )
        return c/12.92;
    else 
        return pow(((c+0.055)/(1.055)),2.4);
}


int salmap_rv::gam_sRGB(double v)
{
    if(v<=0.0031308)
        v *= 12.92;
    else 
        v = 1.055*pow(v,1.0/2.4)-0.055;
    return int(v*255+0.5); 
                           
}


int salmap_rv::rgb_to_gray(int r, int g, int b)
{
  const double rY = 0.212655;
  const double gY = 0.715158;
  const double bY = 0.072187;

  return gam_sRGB(
		  rY*inv_gam_sRGB(r) +
		  gY*inv_gam_sRGB(g) +
		  bY*inv_gam_sRGB(b)
		  );
}




  

double salmap_rv::YtoLstar(const double Y)
{
  if ( Y <= (216/24389) )
    {       
      return (Y * (24389/27))/100;  
    }
  else
    {
      return (pow(Y,(1/3)) * 116 - 16)/100;
    }
}

  








void salmap_rv::RGBtoHSP( const double  R, const double  G, const double  B,
	       double& H, double& S, double& P )
{
  const double Pr  = .299;
  const double Pg  = .587;
  const double Pb  = .114;


  P=sqrt(R*R*Pr + G*G*Pg + B*B*Pb);
  
  
  
  if(R==G && R==B)
    {
      H=0.;
      S=0.;
      return;
    }
  if(R>=G && R>=B)
    {   
      if(B>=G)
	{
	  H=6./6.-1./6.*(B-G)/(R-G);
	  S=1.-G/R;
	}
      else
	{
	  H=0./6.+1./6.*(G-B)/(R-B);
	  S=1.-B/R;
	}
    }
  else if (G>=R && G>=B)
    {   
    if(R>=B)
      {
	H=2./6.-1./6.*(R-B)/(G-B);
	S=1.-B/G;
      }
    else
      {
	H=2./6.+1./6.*(B-R)/(G-R);
	S=1.-R/G;
      }
    }
  else
    {   
      if(G>=R)
	{
	  H=4./6.-1./6.*(G-R)/(B-R);
	  S=1.-R/B;
	}
      else
	{
	  H=4./6.+1./6.*(R-G)/(B-G);
	  S=1.-G/B;
	}
    }
}


void salmap_rv::HSPtoRGB( const double  _H, const double  S, const double  P,
	       double& R, double& G, double& B)
{
  const double Pr  = .299;
  const double Pg  = .587;
  const double Pb  = .114;


  double part, minOverMax=1. - S ;
  double H = _H;
  
  if (minOverMax>0.) {
    if      ( H<1./6.) {   
      H= 6.*( H-0./6.); part=1.+H*(1./minOverMax-1.);
      B=P/sqrt(Pr/minOverMax/minOverMax+Pg*part*part+Pb);
      R=(B)/minOverMax;
      G=(B)+H*((R)-(B)); }
    else if ( H<2./6.) {   
      H= 6.*(-H+2./6.); part=1.+H*(1./minOverMax-1.);
      B=P/sqrt(Pg/minOverMax/minOverMax+Pr*part*part+Pb);
      G=(B)/minOverMax;
      R=(B)+H*((G)-(B)); }
    else if ( H<3./6.) {   
      H= 6.*( H-2./6.); part=1.+H*(1./minOverMax-1.);
      R=P/sqrt(Pg/minOverMax/minOverMax+Pb*part*part+Pr);
      G=(R)/minOverMax;
      B=(R)+H*((G)-(R)); }
    else if ( H<4./6.) {   
      H= 6.*(-H+4./6.); part=1.+H*(1./minOverMax-1.);
      R=P/sqrt(Pb/minOverMax/minOverMax+Pg*part*part+Pr);
      B=(R)/minOverMax;
      G=(R)+H*((B)-(R)); }
    else if ( H<5./6.) {   
      H= 6.*( H-4./6.); part=1.+H*(1./minOverMax-1.);
      G=P/sqrt(Pb/minOverMax/minOverMax+Pr*part*part+Pg);
      B=(G)/minOverMax;
      R=(G)+H*((B)-(G)); }
    else               {   
      H= 6.*(-H+6./6.); part=1.+H*(1./minOverMax-1.);
      G=P/sqrt(Pr/minOverMax/minOverMax+Pb*part*part+Pg);
      R=(G)/minOverMax;
      B=(G)+H*((R)-(G)); }}
  else {
    if      ( H<1./6.) {   
      H= 6.*( H-0./6.);
      R=sqrt(P*P/(Pr+Pg*H*H));
      G=(R)*H;
      B=0.; }
    else if ( H<2./6.) {   
      H= 6.*(-H+2./6.);
      G=sqrt(P*P/(Pg+Pr*H*H));
      R=(G)*H;
      B=0.; }
    else if ( H<3./6.) {   
      H= 6.*( H-2./6.);
      G=sqrt(P*P/(Pg+Pb*H*H));
      B=(G)*H; R=0.; }
    else if ( H<4./6.) {   
      H= 6.*(-H+4./6.);
      B=sqrt(P*P/(Pb+Pg*H*H));
      G=(B)*H;
      R=0.; }
    else if ( H<5./6.) {   
      H= 6.*( H-4./6.);
      B=sqrt(P*P/(Pb+Pr*H*H));
      R=(B)*H;
      G=0.; }
    else               {   
      H= 6.*(-H+6./6.);
      R=sqrt(P*P/(Pr+Pb*H*H));
      B=(R)*H;
      G=0.;
    }
  }
}
