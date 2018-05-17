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

	double spotVol(double tMat) const;
	double fwdVol(double tMat1, double tMat2) const;
};

END_NAMESPACE(orf)

#endif // ORF_VOLATILITYTERMSTRUCTURE_HPP