/**
@file  xlfunctions4.cpp
@brief Implementation of Excel callable functions
*/

#include <orflib/market/market.hpp>
#include <orflib/products/europeancallput.hpp>
#include <orflib/products/digitalcallput.hpp>
#include <orflib/methods/pde/pde1dsolver.hpp>

#include <xlorflib/xlutils.hpp>
#include <xlw/xlw.h>

#include <cmath>

using namespace xlw;
using namespace orf;

BEGIN_EXTERN_C

LPXLFOPER EXCEL_EXPORT xlOrfEuroBSPDE(LPXLFOPER xlPayoffType,
                                      LPXLFOPER xlStrike,
                                      LPXLFOPER xlTimeToExp,
                                      LPXLFOPER xlSpot,
                                      LPXLFOPER xlDiscountCrv,
                                      LPXLFOPER xlDivYield,
                                      LPXLFOPER xlVolatility,
                                      LPXLFOPER xlPdeParams,
                                      LPXLFOPER xlHeaders)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  int payoffType = XlfOper(xlPayoffType).AsInt();
  double spot = XlfOper(xlSpot).AsDouble();
  double strike = XlfOper(xlStrike).AsDouble();
  double timeToExp = XlfOper(xlTimeToExp).AsDouble();

  std::string name = xlStripTick(XlfOper(xlDiscountCrv).AsString());
  SPtrYieldCurve spyc = market().yieldCurves().get(name);
  ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

  double divYield = XlfOper(xlDivYield).AsDouble();

  // get the volatility term structure or create on the fly
  SPtrVolatilityTermStructure volatilityTermStructure;

  if (XlfOper(xlVolatility).IsString())
  {
	  std::string name = xlStripTick(XlfOper(xlVolatility).AsString());
	  volatilityTermStructure = market().volatilities().get(name);
	  ORF_ASSERT(volatilityTermStructure, "error: volatility term structure " + name + " not found");
  }
  else
  {
	  VolatilityTermStructure::VolType voltype = VolatilityTermStructure::SPOTVOL;
	  Vector tmats = { 1 };
	  Vector vals = { XlfOper(xlVolatility).AsDouble() };
	  std::pair<std::string, unsigned long> pr =
		  market().volatilities().set("tmp",
			  new VolatilityTermStructure(tmats.begin(), tmats.end(), vals.begin(), vals.end(), voltype)
		  );
	  volatilityTermStructure = market().volatilities().get("tmp");
  }

  // read the PDE parameters
  PdeParams pdeparams = xlOperToPdeParams(xlPdeParams);
  // handling the xlHeaders argument
  bool headers;
  if (XlfOper(xlHeaders).IsMissing() || XlfOper(xlHeaders).IsNil())
    headers = false;
  else
    headers = XlfOper(xlHeaders).AsBool();

  // create the product
  SPtrProduct spprod(new EuropeanCallPut(payoffType, strike, timeToExp));
  // create the PDE solver
  Pde1DResults results;
  Pde1DSolver solver(spprod, spyc, spot, divYield, volatilityTermStructure, results);
  solver.solve(pdeparams);

  // write results to the outbound XlfOper
  RW offset = headers ? 1 : 0;
  XlfOper xlRet(1 + offset, 1); // construct a range of size 2 x 1
  if (headers) {
    xlRet(0, 0) = "Price";
  }
  xlRet(offset, 0) = results.prices[0];

  return xlRet;

  EXCEL_END;
}

LPXLFOPER EXCEL_EXPORT xlOrfDigiBSPDE(LPXLFOPER xlPayoffType,
	LPXLFOPER xlStrike,
	LPXLFOPER xlTimeToExp,
	LPXLFOPER xlSpot,
	LPXLFOPER xlDiscountCrv,
	LPXLFOPER xlDivYield,
	LPXLFOPER xlVolatility,
	LPXLFOPER xlPdeParams,
	LPXLFOPER xlHeaders)
{
	EXCEL_BEGIN;

	if (XlfExcel::Instance().IsCalledByFuncWiz())
		return XlfOper(true);

	int payoffType = XlfOper(xlPayoffType).AsInt();
	double spot = XlfOper(xlSpot).AsDouble();
	double strike = XlfOper(xlStrike).AsDouble();
	double timeToExp = XlfOper(xlTimeToExp).AsDouble();

	std::string name = xlStripTick(XlfOper(xlDiscountCrv).AsString());
	SPtrYieldCurve spyc = market().yieldCurves().get(name);
	ORF_ASSERT(spyc, "error: yield curve " + name + " not found");

	double divYield = XlfOper(xlDivYield).AsDouble();

	// get the volatility term structure or create on the fly
	SPtrVolatilityTermStructure volatilityTermStructure;

	if (XlfOper(xlVolatility).IsString())
	{
		std::string name = xlStripTick(XlfOper(xlVolatility).AsString());
		volatilityTermStructure = market().volatilities().get(name);
		ORF_ASSERT(volatilityTermStructure, "error: volatility term structure " + name + " not found");
	}
	else
	{
		VolatilityTermStructure::VolType voltype = VolatilityTermStructure::SPOTVOL;
		Vector tmats = { 1 };
		Vector vals = { XlfOper(xlVolatility).AsDouble() };
		std::pair<std::string, unsigned long> pr =
			market().volatilities().set("tmp",
				new VolatilityTermStructure(tmats.begin(), tmats.end(), vals.begin(), vals.end(), voltype)
			);
		volatilityTermStructure = market().volatilities().get("tmp");
	}

	// read the PDE parameters
	PdeParams pdeparams = xlOperToPdeParams(xlPdeParams);
	// handling the xlHeaders argument
	bool headers;
	if (XlfOper(xlHeaders).IsMissing() || XlfOper(xlHeaders).IsNil())
		headers = false;
	else
		headers = XlfOper(xlHeaders).AsBool();

	// create the product
	SPtrProduct spprod(new DigitalCallPut(payoffType, strike, timeToExp));
	// create the PDE solver
	Pde1DResults results;
	Pde1DSolver solver(spprod, spyc, spot, divYield, volatilityTermStructure, results);
	solver.solve(pdeparams);

	// write results to the outbound XlfOper
	RW offset = headers ? 1 : 0;
	XlfOper xlRet(1 + offset, 1); // construct a range of size 2 x 1
	if (headers) {
		xlRet(0, 0) = "Price";
	}
	xlRet(offset, 0) = results.prices[0];

	return xlRet;

	EXCEL_END;
}

END_EXTERN_C
