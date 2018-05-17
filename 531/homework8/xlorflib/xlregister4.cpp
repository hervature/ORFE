/** 
@file  xlregister4.cpp
@brief Registration of Excel callable functions
*/

#include <xlw/xlw.h>
using namespace xlw;

namespace {

  // Register the function ORF.EUROBSPDE
  XLRegistration::Arg OrfEuroBSPDEArgs[] = {
    { "PayoffType", "1: call; -1: put", "XLF_OPER" },
    { "Strike", "strike", "XLF_OPER" },
    { "TimeToExp", "time to expiration", "XLF_OPER" },
    { "Spot", "spot", "XLF_OPER" },
    { "DiscountCrv", "name of the discount curve", "XLF_OPER" },
    { "DivYield", "dividend yield (cont. cmpd.)", "XLF_OPER" },
    { "Vol", "volatility", "XLF_OPER" },
    { "PdeParams", "The PDE parameters", "XLF_OPER" },
    { "Headers", "TRUE for displaying the header", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfEuroBSPDE(
    "xlOrfEuroBSPDE", "ORF.EUROBSPDE", "Price of a European option in the Black-Scholes model using PDE.",
    "ORFLIB", OrfEuroBSPDEArgs, 9);

  // Register the function ORF.DIGIBSPDE
  XLRegistration::Arg OrfDigiBSPDEArgs[] = {
	  { "PayoffType", "1: call; -1: put", "XLF_OPER" },
	  { "Strike", "strike", "XLF_OPER" },
	  { "TimeToExp", "time to expiration", "XLF_OPER" },
	  { "Spot", "spot", "XLF_OPER" },
	  { "DiscountCrv", "name of the discount curve", "XLF_OPER" },
	  { "DivYield", "dividend yield (cont. cmpd.)", "XLF_OPER" },
	  { "Vol", "volatility", "XLF_OPER" },
	  { "PdeParams", "The PDE parameters", "XLF_OPER" },
	  { "Headers", "TRUE for displaying the header", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfDigiBSPDE(
	  "xlOrfDigiBSPDE", "ORF.DIGIBSPDE", "Price of a digital option in the Black-Scholes model using PDE.",
	  "ORFLIB", OrfDigiBSPDEArgs, 9);

}  // anonymous namespace
