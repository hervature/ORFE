/**
@file  worstofdigitalcallput.hpp
@brief The payoff of the worst digital call/put of a basket of assets
*/

#ifndef ORF_WORSTOFDIGITALCALLPUT_HPP
#define ORF_WORSTOFDIGITALCALLPUT_HPP

#include <orflib/products/product.hpp>
#include <algorithm>
#include <functional>

BEGIN_NAMESPACE(orf)

/** The worst digital call/put class
*/

class WorstOfDigitalCallPut : public Product
{
public:
	/** Initializing ctor */
	WorstOfDigitalCallPut(int payoffType,
		double strike,
		Vector const& fixingTimes,
		int n_assets);

	/** The number of assets this product depends on */
	virtual size_t nAssets() const;

	/** Evaluates the product given the passed-in path
	The "pricePath" matrix must have as many rows as
	the number of fixing times
	*/
	virtual void eval(Matrix const& pricePath) override;

private:
	int payoffType_;          // 1: call; -1 put
	double strike_;
	int n_assets_;
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
size_t WorstOfDigitalCallPut::nAssets() const
{
	return n_assets_;
}

inline
WorstOfDigitalCallPut::WorstOfDigitalCallPut(int payoffType,
	double strike,
	Vector const& fixingTimes,
	int n_assets)
	: payoffType_(payoffType), strike_(strike), n_assets_(n_assets)
{
	ORF_ASSERT(payoffType == 1 || payoffType == -1, "WorstOfDigitalCallPut: the payoff type must be 1 (call) or -1 (put)!");
	ORF_ASSERT(strike >= 0.0, "WorstOfDigitalCallPut: the strike must be positive!");
	ORF_ASSERT(fixingTimes[0] >= 0.0,
		"WorstOfDigitalCallPut: the first fixing time must be non-negative!");
	Vector::const_iterator it(
		std::adjacent_find(fixingTimes.begin(), fixingTimes.end(), std::greater_equal<double>()));
	ORF_ASSERT(it == fixingTimes.end(),
		"WorstOfDigitalCallPut: the fixing times must be in strict increasing order");
	ORF_ASSERT(fixingTimes.size() == 2,
		"WorstOfDigitalCallPut: the fixing times must be of length 2");

	// set the fixing times
	fixTimes_ = fixingTimes;
	// assume that it will settle (pay) at expiration
	// TODO allow payment time later than expiration
	payTimes_.resize(1);
	payTimes_[0] = fixingTimes[fixingTimes.size() - 1];

	// this product generates only one payment
	payAmounts_.resize(1);
}

inline void WorstOfDigitalCallPut::eval(Matrix const& pricePath)
{
	size_t nfixings = pricePath.n_rows;
	ORF_ASSERT(fixTimes_.size() == nfixings,
		"WorstOfDigitalCallPut: number of fixings mismatch in price path!");
	size_t nassets = pricePath.n_cols;
	ORF_ASSERT(this->nAssets() == nassets,
		"WorstOfDigitalCallPut: number of assets mismatch in price path!");

	double worst_digital = pricePath(1,0)/pricePath(0,0);
	for (size_t j = 1; j < nassets; j++) {
		if ( (pricePath(1, j) / pricePath(0, j)) < worst_digital) {
			worst_digital = pricePath(1, j) / pricePath(0, j);
		}
	}

	if (payoffType_ == 1)
		payAmounts_[0] = worst_digital >= strike_ ? 1 : 0.0;
	else
		payAmounts_[0] = worst_digital >= strike_ ? 0.0 : 1;
}

END_NAMESPACE(orf)

#endif // ORF_WORSTOFDIGITALCALLPUT_HPP