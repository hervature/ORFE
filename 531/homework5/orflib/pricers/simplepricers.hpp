/**
    @file  simplepricers.hpp
    @brief Declaration of simple pricing functions
*/

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/market/market.hpp>


BEGIN_NAMESPACE(orf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield);

/** Price of a European digital option in the Black-Scholes model*/
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility);

double capFloorletBS(int payoffType, SPtrYieldCurve spyc, double strikeRate,
	double timeToReset, double tenor, double fwdRateVol);

orf::Vector cdsPV(SPtrYieldCurve rfreeYC, double credSprd, double cdsRate,
	double recov, double timeToMat, double payFreq);

END_NAMESPACE(orf)
