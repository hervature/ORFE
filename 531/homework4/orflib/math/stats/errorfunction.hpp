/**
    @file  errorfunction.hpp
    @brief Error function, its complement and inverse
*/

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>

#ifndef ORF_ERRORFUNCTION_HPP
#define ORF_ERRORFUNCTION_HPP

BEGIN_NAMESPACE(orf)

/** The error function; adapted from Numerical Recipes in C++ */
class ErrorFunction
{
public:
  /** Returns the error function erf(x) */
  double erf(double x) const;

  /** Returns the complement 1-erf(x) */
  double erfc(double x) const;

  /** Returns the inverse of the error function */
  double inverf(double p)  const;

  /** Returns the inverse of the complement of the error function */
  double inverfc(double p) const;

protected:
  double erfccheb(double z) const;

  // state
  static const int ncof = 28;
  static const double cof[28];
};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline double ErrorFunction::erf(double x) const
{
  // Return erf.x/ for any x.
  if (x >= 0.)
    return 1.0 - erfccheb(x);
  else
    return erfccheb(-x) - 1.0;
}

inline double ErrorFunction::erfc(double x) const
{
  // Return erfc.x/ for any x.
  if (x >= 0.)
    return erfccheb(x);
  else
    return 2.0 - erfccheb(-x);
}

inline double ErrorFunction::inverf(double p) const
{
  return inverfc(1. - p);
}

END_NAMESPACE(orf)

#endif // ORF_ERRORFUNCTION_HPP
