/** 
  @file  xlregister2.cpp
  @brief Registration of Excel callable functions
*/

#include <xlw/xlw.h>
using namespace xlw;

namespace {

  // Register the function ORF.MKTLIST
  XLRegistration::Arg OrfMktListArgs[] = {
    {}
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfMktList(
    "xlOrfMktList", "ORF.MKTLIST", "Lists all market objects",
    "ORFLIB", OrfMktListArgs, 0);

  // Register the function ORF.MKTCLEAR
  XLRegistration::Arg OrfMktClearArgs[] = {
    {}
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfMktClear(
    "xlOrfMktClear", "ORF.MKTCLEAR", "Deletes all market objects",
    "ORFLIB", OrfMktClearArgs, 0);

  // Register the function ORF.YCCREATE
  XLRegistration::Arg OrfYCCreateArgs[] = {
    { "YCName", "Yield curve name", "XLF_OPER" },
    { "Mats", "Time to each maturity", "XLF_OPER" },
    { "Rates", "Interest rates or zero coupon bond prices", "XLF_OPER" },
    { "InputType", "0: spot rates, 1: fwd rates, 2: zero coupon bonds", "XLF_OPER" },
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfYCCreate(
    "xlOrfYCCreate", "ORF.YCCREATE", "Creates a yield curve",
    "ORFLIB", OrfYCCreateArgs, 4);

  // Register the function ORF.DF
  XLRegistration::Arg OrfDiscountArgs[] = {
    { "YCName", "Yield curve name", "XLF_OPER" },
    { "Mat", "Time to maturity", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfDiscount(
    "xlOrfDiscount", "ORF.DF", "Returns the discount factor to maturity",
    "ORFLIB", OrfDiscountArgs, 2);

  // Register the function ORF.FDF
  XLRegistration::Arg OrfFwdDiscountArgs[] = {
    { "YCName", "Yield curve name", "XLF_OPER" },
    { "Mat1", "Time to first maturity", "XLF_OPER" },
    { "Mat2", "Time to second maturity", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfFwdDiscount(
    "xlOrfFwdDiscount", "ORF.FWDDF", "Returns the fwd discount factor between the two maturities",
    "ORFLIB", OrfFwdDiscountArgs, 3);

  // Register the function ORF.SPOTRATE
  XLRegistration::Arg OrfSpotRateArgs[] = {
    { "YCName", "Yield curve name", "XLF_OPER" },
    { "Mat", "Time to maturity", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfSpotRate(
    "xlOrfSpotRate", "ORF.SPOTRATE", "Returns the spot rate to maturity",
    "ORFLIB", OrfSpotRateArgs, 2);

  // Register the function ORF.FWDRATE
  XLRegistration::Arg OrfFwdRateArgs[] = {
    { "YCName", "Yield curve name", "XLF_OPER" },
    { "Mat1", "Time to first maturity", "XLF_OPER" },
    { "Mat2", "Time to second maturity", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfFwdRate(
    "xlOrfFwdRate", "ORF.FWDRATE", "Returns the fwd rate between the two maturities",
    "ORFLIB", OrfFwdRateArgs, 3);

  // Register the function ORF.CAPFlOORLETBS
  XLRegistration::Arg OrfCapFloorletBSArgs[] = {
	  { "PayoffType", "Payoff type", "XLF_OPER" },
	  { "YCName", "Yield curve name", "XLF_OPER" },
	  { "StrikeRate", "Strike rate", "XLF_OPER" },
	  { "TimeToReset", "Time to reset", "XLF_OPER" },
	  { "Tenor", "Tenor", "XLF_OPER" },
	  { "FwdRateVol", "Volatility of the forward rate", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfCapFloorletBS(
	  "xlOrfCapFloorletBS", "ORF.CAPFLOORLETBS", "Returns the price of a caplet or a floorlet",
	  "ORFLIB", OrfCapFloorletBSArgs, 6);

  // Register the function ORF.CDSPV
  XLRegistration::Arg OrfCDSPVArgs[] = {
	  { "YCName", "Yield curve name", "XLF_OPER" },
	  { "CredSprd", "Credit spread", "XLF_OPER" },
	  { "CDSRate", "CDS rate", "XLF_OPER" },
	  { "Recov", "Recovery rate", "XLF_OPER" },
	  { "TimeToMat", "Time to maturity", "XLF_OPER" },
	  { "PayFreq", "Payment Frequency", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper regOrfCDSPV(
	  "xlOrfCDSPV", "ORF.CDSPV", "Returns the PV of the CDS",
	  "ORFLIB", OrfCDSPVArgs, 6);

}  // anonymous namespace
