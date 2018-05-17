/**
  @file  utls.hpp
  @brief Utilities for quantity conversions, etc.
*/

#ifndef ORF_UTILS_HPP
#define ORF_UTILS_HPP

#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <cmath>

BEGIN_NAMESPACE(orf)

inline
double toContCmpd(double rate, size_t annfreq)
{
  ORF_ASSERT(annfreq >= 1.0, "compounding frequency less than 1 not allowed");
  double tmp = std::pow(1.0 + rate / annfreq, annfreq);
  return log(tmp);
}

inline
double fromContCmpd(double rate, size_t annfreq)
{
  ORF_ASSERT(annfreq >= 1.0, "compounding frequency less than 1 not allowed");
  double tmp = std::exp(rate/annfreq);
  return (tmp - 1.0)*annfreq;
}

END_NAMESPACE(orf)

#endif // ORF_UTILS_HPP
