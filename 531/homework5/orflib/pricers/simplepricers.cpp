/**
    @file  simplepricers.cpp
    @brief Implementation of simple pricing functions
*/

#include <orflib/pricers/simplepricers.hpp>
#include <orflib/math/stats/normaldistribution.hpp>
#include <orflib/utils.hpp>

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

double capFloorletBS(int payoffType, SPtrYieldCurve spyc, double strikeRate,
	double timeToReset, double tenor, double fwdRateVol)
{
	ORF_ASSERT(payoffType == 1 || payoffType == -1, "payoffType must be 1 or -1");
	ORF_ASSERT(strikeRate >= 0.0, "strike rate must be non-negative");
	ORF_ASSERT(timeToReset >= 0.0, "time to reset must be non-negative");
	ORF_ASSERT(tenor >= 0.0, "tenor must be non-negative");
	ORF_ASSERT(fwdRateVol >= 0.0, "forward volatility must be non-negative");

	int phi = payoffType;
	double sigF = fwdRateVol*sqrt(timeToReset);
	double Ti = timeToReset + tenor;
	double fwdRate = spyc->fwdRate(timeToReset, Ti);
	double rate = spyc->spotRate(Ti);
	fwdRate = fromContCmpd(fwdRate, 2);

	double d1 = log(fwdRate / strikeRate)/sigF + sigF/2;
	double d2 = d1 - sigF;

	NormalDistribution normal;

	double V = exp(-rate*Ti)*(phi*fwdRate*normal.cdf(phi*d1) - phi*strikeRate*normal.cdf(phi*d2))*tenor;
	return V;
}

double survival_probability(double credSprd, double recov, double t) {
	return (exp(-credSprd*t) - 1) / (1 - recov) + 1;
}

orf::Vector cdsPV(SPtrYieldCurve rfreeYC, double credSprd, double cdsRate,
	double recov, double timeToMat, double payFreq)
{
	ORF_ASSERT(credSprd >= 0.0, "credit spread must be non-negative");
	ORF_ASSERT(recov >= 0.0, "recovery rate must be non-negative");
	ORF_ASSERT(timeToMat >= 0.0, "time to maturity must be non-negative");
	ORF_ASSERT(payFreq >= 0.0, "pay frequency must be non-negative");

	double sum_dl = 0;
	double sum_pl = 0;
	double rate = 0;
	double timeFreq = 1 / payFreq;

	for (double t = timeToMat; t > 0; t = t - timeFreq) {

		rate = rfreeYC->spotRate(t);

		// Default leg
		if (t - timeFreq >= 0) {
			sum_dl = sum_dl + (survival_probability(credSprd, recov, t-timeFreq) - survival_probability(credSprd, recov, t))*exp(-rate*t);
		}
		else {
			// stub payment
			sum_dl = sum_dl + (survival_probability(credSprd, recov, 0) - survival_probability(credSprd, recov, t))*exp(-rate*t);
		}
		
		// Premium leg
		sum_pl = sum_pl + survival_probability(credSprd, recov, t)*exp(-rate*t);
	}

	double dl = sum_dl*(1 - recov);
	double pl = sum_pl*cdsRate*timeFreq;

	orf::Vector ret = { dl, pl };
	return ret;
}


END_NAMESPACE(orf)
