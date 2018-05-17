/**
    @file  xlregister1.cpp
    @brief Registration of Excel callable functions
*/

#include <xlw/xlw.h>
using namespace xlw;

namespace {

  // Register the function ORF.ERF
  XLRegistration::Arg OrfErfArgs[] = {
    { "x", "x", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfErf(
    "xlOrfErf", "ORF.ERF", "The error function.",
    "ORFLIB", OrfErfArgs, 1);

  // Register the function ORF.INVERF
  XLRegistration::Arg OrfInvErfArgs[] = {
    { "y", "y", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfInvErf(
    "xlOrfInvErf", "ORF.INVERF", "The inverse error function.",
    "ORFLIB", OrfInvErfArgs, 1);

  // Register the function ORF.NORMALCDF
  XLRegistration::Arg OrfNormalCdfArgs[] = {
    { "x", "x", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfNormalCdf(
    "xlOrfNormalCdf", "ORF.NORMALCDF", "The standard normal cumulative distribution.",
    "ORFLIB", OrfNormalCdfArgs, 1);

  // Register the function ORF.NORMALINVCDF
  XLRegistration::Arg OrfNormalInvCdfArgs[] = {
    { "p", "p", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfNormalInvCdf(
    "xlOrfNormalInvCdf", "ORF.NORMALINVCDF", "The inverse standard normal cumulative distribution.",
    "ORFLIB", OrfNormalInvCdfArgs, 1);

  // Register the function ORF.FWDPRICE
  XLRegistration::Arg OrfFwdPriceArgs[] = {
    { "Spot", "spot", "XLF_OPER" },
    { "TimeToExp", "time to expiration", "XLF_OPER" },
    { "IntRate", "risk-free interest rate (cont. cmpd.)", "XLF_OPER" },
    { "DivYield", "dividend yield (cont. cmpd.)", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfFwdPrice(
    "xlOrfFwdPrice", "ORF.FWDPRICE", "The forward price of an asset.",
    "ORFLIB", OrfFwdPriceArgs, 4);

  // Register the function ORF.DIGIBS
  XLRegistration::Arg OrfDigiBSArgs[] = {
    { "PayoffType", "1: call; -1: put", "XLF_OPER" },
    { "Spot", "spot", "XLF_OPER" },
    { "Strike", "strike", "XLF_OPER" },
    { "TimeToExp", "time to expiration", "XLF_OPER" },
    { "IntRate", "risk-free interest rate (cont. cmpd.)", "XLF_OPER" },
    { "DivYield", "dividend yield (cont. cmpd.)", "XLF_OPER" },
    { "Vol", "volatility", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfDigiBS(
    "xlOrfDigiBS", "ORF.DIGIBS", "Price of a digital option in the Black-Scholes model.",
    "ORFLIB", OrfDigiBSArgs, 7);

}
