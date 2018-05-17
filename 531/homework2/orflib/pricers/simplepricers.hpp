/**
    @file  simplepricers.hpp
    @brief Declaration of simple pricing functions
*/

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>


BEGIN_NAMESPACE(orf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield);

/** The quanto forward price of an asset */
double quantoFwdPrice(double spot, double timeToExp, double intRate, double divYield, double assetVol, double FXVol, double assetFXCor);

/** Price of a European digital option in the Black-Scholes model*/
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility);

/** Price of a European option in the Black-Scholes model*/
double europeanOptionBS(int payoffType, double spot, double strike, double timeToExp,
	double intRate, double divYield, double volatility);

END_NAMESPACE(orf)
