/**
    @file  simplepricers.cpp
    @brief Implementation of simple pricing functions
*/

#include <orflib/pricers/simplepricers.hpp>
#include <orflib/math/stats/normaldistribution.hpp>

#include <cmath>

BEGIN_NAMESPACE(orf)

/** The forward price of an asset */
double fwdPrice(double spot, double timeToExp, double intRate, double divYield)
{
  ORF_ASSERT(spot >= 0.0, "spot must be non-negative");
  ORF_ASSERT(timeToExp >= 0.0, "time to expiration must be non-negative");
  ORF_ASSERT(intRate >= 0.0, "interest rate must be non-negative");
  ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");

  return spot * exp((intRate - divYield) * timeToExp);
}

double quantoFwdPrice(double spot, double timeToExp, double intRate, double divYield, double assetVol, double FXVol, double assetFXCor)
{
	ORF_ASSERT(spot >= 0.0, "spot must be non-negative");
	ORF_ASSERT(timeToExp >= 0.0, "time to expiration must be non-negative");
	ORF_ASSERT(intRate >= 0.0, "interest rate must be non-negative");
	ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
	ORF_ASSERT(assetVol >= 0.0, "Asset volatility must be non-negative");
	ORF_ASSERT(FXVol >= 0.0, "FX volatility must be non-negative");
	ORF_ASSERT(assetFXCor >= 0.0, "Asset/FX correlation must be non-negative");

	return exp(assetVol * FXVol * assetFXCor * timeToExp)*fwdPrice(spot, timeToExp, intRate, divYield);
}

/** Price of a European digital option in the Black-Scholes model*/
double digitalOptionBS(int payoffType, double spot, double strike, double timeToExp,
                       double intRate, double divYield, double volatility)
{
  ORF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
  ORF_ASSERT(strike >= 0.0, "strike must be non-negative");
  ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
  ORF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

  double phi = payoffType;
  double fwd = fwdPrice(spot, timeToExp, intRate, divYield);
  double sigT = volatility * sqrt(timeToExp);
  double d2 = log(fwd / strike) / sigT - 0.5 * sigT;
  NormalDistribution normal;

  double price = exp(-intRate * timeToExp) * normal.cdf(phi * d2);

  return price;
}

/** Price of a European option in the Black-Scholes model*/
orf::Vector europeanOptionBS(int payoffType, double spot, double strike, double timeToExp,
	double intRate, double divYield, double volatility)
{
	ORF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
	ORF_ASSERT(strike >= 0.0, "strike must be non-negative");
	ORF_ASSERT(divYield >= 0.0, "dividend yield must be non-negative");
	ORF_ASSERT(volatility >= 0.0, "volatility must be non-negative");

	double phi = payoffType;
	double fwd = fwdPrice(spot, timeToExp, intRate, divYield);
	double sigT = volatility * sqrt(timeToExp);
	double d1 = log(fwd / strike) / sigT + 0.5 * sigT;
	double d2 = d1 - sigT;
	NormalDistribution normal;

	double price = phi * exp(-divYield*timeToExp) * spot * normal.cdf(phi*d1) - phi * exp(-intRate*timeToExp) * strike * normal.cdf(phi*d2);
	
	double delta;

	if (payoffType == 1) {
		delta = exp(-divYield*timeToExp) * normal.cdf(d1);
	}
	else {
		delta = exp(-divYield*timeToExp) * (normal.cdf(d1)-1);
	}

	double gamma = exp(-divYield*timeToExp)* normal.pdf(d1) / (spot*sigT);

	double theta;

	if (payoffType == 1) {
		theta = -exp(-divYield*timeToExp)*spot*volatility*normal.pdf(d1) / (2*sqrt(timeToExp)) + divYield*exp(-divYield*timeToExp)*spot*normal.cdf(d1) - intRate*exp(-intRate*timeToExp)*strike*normal.cdf(d2);
	}
	else {
		theta = -exp(-divYield*timeToExp)*spot*volatility*normal.pdf(d1) / (2*sqrt(timeToExp)) - divYield*exp(-divYield*timeToExp)*spot*normal.cdf(-d1) + intRate*exp(-intRate*timeToExp)*strike*normal.cdf(-d2);
	}

	double vega = exp(-divYield*timeToExp)*spot*sqrt(timeToExp)*normal.pdf(d1);

	orf::Vector ret = { price, delta, gamma, theta, vega };

	return ret;
}


END_NAMESPACE(orf)
