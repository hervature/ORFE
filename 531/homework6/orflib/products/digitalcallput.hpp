#pragma once
/**
@file  digitalcallput.hpp
@brief The payoff of a Digital Call/Put option
*/

#ifndef ORF_DIGITALCALLPUT_HPP
#define ORF_DIGITALCALLPUT_HPP

#include <orflib/products/product.hpp>

BEGIN_NAMESPACE(orf)

/** The Digital call/put class
*/
class DigitalCallPut : public Product
{
public:
	/** Initializing ctor */
	DigitalCallPut(int payoffType, double strike, double timeToExp);

	/** Evaluates the product given the passed-in path
	The "pricePath" matrix must have as many rows as
	the number of fixing times
	*/
	virtual void eval(Matrix const& pricePath) override;

private:
	int payoffType_;     // 1: call; -1 put
	double strike_;
	double timeToExp_;
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
DigitalCallPut::DigitalCallPut(int payoffType, double strike, double timeToExp)
	: payoffType_(payoffType), strike_(strike), timeToExp_(timeToExp)
{
	ORF_ASSERT(payoffType == 1 || payoffType == -1, "EuropeanCallPut: the payoff type must be 1 (call) or -1 (put)!");
	ORF_ASSERT(strike > 0.0, "DigitalCallPut: the strike must be positive!");
	ORF_ASSERT(timeToExp > 0.0, "DigitalCallPut: the time to expiration must be positive!");

	// only one fixing time, the expiration
	fixTimes_.resize(1);
	fixTimes_[0] = timeToExp_;

	// assume that it will settle (pay) at expiration
	// TODO allow payment time later than expiration
	payTimes_.resize(1);
	payTimes_[0] = timeToExp_;

	// this product generates only one payment
	payAmounts_.resize(1);
}

inline void DigitalCallPut::eval(Matrix const& pricePath)
{
	double S_T = pricePath(0, 0);
	if (payoffType_ == 1)
		payAmounts_[0] = S_T >= strike_ ? 1.0 : 0.0;
	else
		payAmounts_[0] = S_T >= strike_ ? 0.0 : 1.0;
}

END_NAMESPACE(orf)

#endif // ORF_DIGITALCALLPUT_HPP