/**
@file  product.hpp
@brief Base class for all financial products.
*/

#ifndef ORF_PRODUCT_HPP
#define ORF_PRODUCT_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/sptr.hpp>
#include <orflib/math/matrix.hpp>

BEGIN_NAMESPACE(orf)

/** The abstract base class for all financial products.
    It must be inherited by specific product payoffs.
*/
class Product
{
public:
  /** Initializing ctor */
  explicit Product(std::string const& payccy = "USD");

  /** Dtor */
  virtual ~Product() {}

  /** Returns the fixing (observation) times */
  Vector const& fixTimes() const;

  /** Returns the payment times */
  Vector const& payTimes() const;

  /** Returns the payment amounts */
  Vector const& payAmounts() const;

  /** Returns the number of assets this product depends on */
  virtual size_t nAssets() const = 0;

  /** Evaluates the product given the passed-in path
      The "pricePath" matrix must have as many rows as the number of fixing times
  */
  virtual void eval(Matrix const& pricePath) = 0;

protected:
  std::string payccy_;
  Vector fixTimes_;       // the fixing (observation) times
  Vector payTimes_;       // the payment times
  Vector payAmounts_;     // the payment times
};

/** Smart pointer to Product */
using SPtrProduct = std::shared_ptr<Product>;

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
Product::Product(std::string const& payccy)
: payccy_(payccy)
{}

inline
Vector const& Product::fixTimes() const
{
  return fixTimes_;
}

inline
Vector const& Product::payTimes() const
{
  return payTimes_;
}

inline
Vector const& Product::payAmounts() const
{
  return payAmounts_;
}

END_NAMESPACE(orf)

#endif // ORF_PRODUCT_HPP
