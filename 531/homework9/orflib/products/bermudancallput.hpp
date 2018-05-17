/**
    @file  bermudancallput.hpp
    @brief The payoff of a Bermudan Call/Put option
*/

#ifndef ORF_BERMUDANCALLPUT_HPP
#define ORF_BERMUDANCALLPUT_HPP

#include <orflib/products/product.hpp>
#include <algorithm>
#include <functional>

BEGIN_NAMESPACE(orf)

/** The Bermudan basket call/put class
*/
class BermudanCallPut : public Product
{
public:
  /** Initializing ctor */
	BermudanCallPut(int payoffType,
                     double strike,
                     Vector const& timesToExer);

  /** The number of assets this product depends on */
  virtual size_t nAssets() const override { return 1; }

  /** Evaluates the product given the passed-in path
  The "pricePath" matrix must have as many rows as
  the number of fixing times
  */
  virtual void eval(Matrix const& pricePath) override;

  /** Evaluates the product at fixing time index idx
  */
  virtual void eval(size_t idx, Vector const& spots, double contValue) override;

private:
  int payoffType_;          // 1: call; -1 put
  double strike_;
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
BermudanCallPut::BermudanCallPut(int payoffType,
                                       double strike,
                                       Vector const& timesToExer)
: payoffType_(payoffType), strike_(strike)
{
  ORF_ASSERT(payoffType == 1 || payoffType == -1, "BermudanCallPut: the payoff type must be 1 (call) or -1 (put)!");
  ORF_ASSERT(strike >= 0.0, "BermudanCallPut: the strike must be positive!");
  ORF_ASSERT(timesToExer[0] >= 0.0,
    "BermudanCallPut: the first exercise time must be non-negative!");
  Vector::const_iterator it(
    std::adjacent_find(timesToExer.begin(), timesToExer.end(), std::greater_equal<double>()));
  ORF_ASSERT(it == timesToExer.end(),
    "BermudanCallPut: the exercise times must be in strict increasing order");


  // set the fixing times
  fixTimes_ = timesToExer;

  // payment times are at exercise times
  payTimes_ = fixTimes_;
  payAmounts_.resize(payTimes_.size());
}

inline void BermudanCallPut::eval(Matrix const& pricePath)
{
	ORF_ASSERT(0, "not implemented!");
}

inline void BermudanCallPut::eval(size_t idx, Vector const& spots, double contValue)
{
	double spot = spots[0];

	if (idx == payAmounts_.size() - 1) { // this is the last index
		double payoff = (spot - strike_) * payoffType_;
		payAmounts_[idx] = payoff > 0.0 ? payoff : 0.0;
	}
	else {  // this is not the last index, check the exercise condition
		double intrinsicValue = (spot - strike_) * payoffType_;
		intrinsicValue = intrinsicValue >= 0.0 ? intrinsicValue : 0.0;
		payAmounts_[idx] = contValue >= intrinsicValue ? contValue : intrinsicValue;
		// zero out the amounts after this index
		for (size_t j = idx + 1; j < payAmounts_.size(); ++j)
			payAmounts_[j] = 0.0;
	}

}

END_NAMESPACE(orf)

#endif // ORF_ASIANBASKETCALLPUT_HPP
