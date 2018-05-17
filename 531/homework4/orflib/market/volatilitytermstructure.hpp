/**
	@file  volatilitytermstructure.hpp
	@brief Class representing a volatility term structure
*/

#ifndef ORF_VOLATILITYTERMSTRUCTURE_HPP
#define ORF_VOLATILITYTERMSTRUCTURE_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/math/interpol/piecewisepolynomial.hpp>
#include <orflib/sptr.hpp>
#include <string>

BEGIN_NAMESPACE(orf)

/** The yield curve */
class VolatilityTermStructure
{
public:

	/** Used to qualify the type of quantities used for building the curve */
	enum VolType
	{
		SPOTVOL,
		FWDVOL
	};

	template<typename XITER, typename YITER>
	VolatilityTermStructure(XITER tMatBegin, XITER tMatEnd,
		YITER volBegin, YITER volEnd,
		VolType vtype = SPOTVOL);

	/** Returns the curve currency */
	std::string ccy() const { return ccy_; }

	double spotVol(double tMat) const;
	double fwdVol(double tMat1, double tMat2) const;

protected:

private:
	// helper functions
	void initFromSpotVols();
	void initFromFwdVols();

	std::string ccy_;  // the curve's currency
	PiecewisePolynomial fwdrates_;  // the piecewise constant forward rates
};

typedef std::shared_ptr<VolatilityTermStructure> SPtrVolatilityTermStructure;

// Inline implementations

template<typename XITER, typename YITER>
VolatilityTermStructure::VolatilityTermStructure(XITER tMatBegin, XITER tMatEnd,
	YITER volBegin, YITER volEnd,
	VolType voltype)
	: ccy_("USD"), fwdrates_(tMatBegin, tMatEnd, volBegin, 0)
{
	std::ptrdiff_t n = tMatEnd - tMatBegin;
	ORF_ASSERT(n == volEnd - volBegin, "VolatilityTermStructure: different number of maturities and rates");
	auto it = std::find_if_not(tMatBegin, tMatEnd, [](double x) {return x > 0.0; });
	ORF_ASSERT(it == tMatEnd, "VolatilityTermStructure: maturities must be positive");

	switch (voltype) {
	case VolatilityTermStructure::SPOTVOL:
		initFromSpotVols();
		break;
	case VolatilityTermStructure::FWDVOL:
		initFromFwdVols();
		break;
	default:
		ORF_ASSERT(0, "error: unknown yield curve input type");
	}
}

END_NAMESPACE(orf)

#endif // ORF_VOLATILITYTERMSTRUCTURE_HPP