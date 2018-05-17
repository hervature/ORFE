/**
@file  simplepricers.hpp
@brief Declaration of simple pricing functions
*/

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/math/matrix.hpp>
#include <orflib/market/yieldcurve.hpp>


BEGIN_NAMESPACE(orf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield);

/** The quanto forward price of an asset */
double quantoFwdPrice(double spot, double timeToExp, double intRate, double divYield,
                      double assetVol, double fxVol, double correl);

/** Price of a European digital option in the Black-Scholes model*/
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility);

/** Price of a European option in the Black-Scholes model*/
Vector europeanOptionBS(int payoffType, double spot, double strike,
                        double timeToExp, double intRate,
                        double divYield, double volatility);

/** Price of a European caplet/floorlet in the Black-Scholes model*/
double capFloorletBS(int payoffType, SPtrYieldCurve spyc, double strikeRate,
  double timeToReset, double tenor, double fwdRateVol);

/** Present value of a credit default swap */
orf::Vector cdsPV(SPtrYieldCurve spyc, double creadSpread, double cdsRate,
  double recov, double timeToMat, size_t payFreq);

END_NAMESPACE(orf)
