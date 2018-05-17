/**
  @file  market.hpp
  @brief Definition of the market singleton and the market() free function
*/

#ifndef ORF_MARKET_HPP
#define ORF_MARKET_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <orflib/sptrmap.hpp>
#include <orflib/market/yieldcurve.hpp>

BEGIN_NAMESPACE(orf)

class Market
{
public:

  /** Returns the unique instance */
  static Market& instance();

  /** Clears the market of all objects */
  void clear();

  /** Returns the yield curves map */
  SPtrMap<YieldCurve>& yieldCurves() { return ycmap_; }

private:

  /** allow private default ctor */
  Market() {}

  /** forbid copy ctor and copy-assignment operator */
  Market(Market const& rhs) = delete;
  Market& operator=(Market const&) = delete;

  // state
  SPtrMap<YieldCurve> ycmap_;
};

/** Free function returning the market singleton */
Market& market();

END_NAMESPACE(orf)

#endif // ORF_MARKET_HPP
