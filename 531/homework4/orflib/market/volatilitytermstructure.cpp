/**
	@file  volatilitytermstructure.cpp
	@brief Class representing a volatility term structure
*/

#include <orflib/market/volatilitytermstructure.hpp>

BEGIN_NAMESPACE(orf)

using namespace std;

void VolatilityTermStructure::initFromSpotVols()
{
	auto cit = fwdrates_.coeff_begin(0);

	double T1 = fwdrates_.breakPoint(0);
	double R1 = *cit;
	fwdrates_.setBreakPoint(0, 0.0); // remember, the ppoly object is right-continuous
	++cit;
	for (size_t i = 1; i < fwdrates_.size(); ++i, ++cit) {
		double T2 = fwdrates_.breakPoint(i);
		double R2 = *cit;
		double F = R2 * R2 * T2 - R1 * R1 * T1;
		ORF_ASSERT(F >= 0.0,
			"VolatilityTermStructure: negative fwd volatility between T1 = " + to_string(T1) + " and T2 = " + to_string(T2));
		F /= (T2 - T1);
		F = sqrt(F);
		fwdrates_.setBreakPoint(i, T1);
		*cit = F;
		T1 = T2;
		R1 = R2;
	}
}

void VolatilityTermStructure::initFromFwdVols() {
	// just validate the fwd rates
	auto cit = fwdrates_.coeff_begin(0);
	double T1 = 0.0;
	for (size_t i = 0; i < fwdrates_.size(); ++i, ++cit) {
		double T2 = fwdrates_.breakPoints()(i);
		fwdrates_.setBreakPoint(i, T1);  // remember, the ppoly object is right-continuous
		double fwdrate = *cit;
		ORF_ASSERT(fwdrate >= 0.0,
			"VolatilityTermStructure: negative fwd volatility between T1 = " + to_string(T1) + " and T2 = " + to_string(T2));
		T1 = T2;
	}
}

double VolatilityTermStructure::spotVol(double tMat) const {
	ORF_ASSERT(tMat >= 0.0, "VolatilityTermStructure: negative spot volatility not allowed");
	double svol = fwdrates_.integral(0.0, tMat);
	return svol / tMat; // return annualized volatility
}

double VolatilityTermStructure::fwdVol(double tMat1, double tMat2) const {
	ORF_ASSERT(tMat1 >= 0.0, "VolatilityTermStructure: negative times not allowed");
	ORF_ASSERT(tMat1 <= tMat2, "VolatilityTermStructure: maturities are out of order");
	double fvol = fwdrates_.integral(tMat1, tMat2);
	return fvol / (tMat2 - tMat1); // return annualized volatility
}

END_NAMESPACE(orf)