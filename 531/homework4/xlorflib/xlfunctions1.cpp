/**
    @file  xlfunctions1.cpp
    @brief Implementation of Excel callable functions
*/

#include <orflib/defines.hpp>
#include <orflib/math/stats/errorfunction.hpp>
#include <orflib/math/stats/normaldistribution.hpp>
#include <orflib/pricers/simplepricers.hpp>

#include <xlw/xlw.h>


using namespace xlw;
using namespace orf;

BEGIN_EXTERN_C

LPXLFOPER EXCEL_EXPORT xlOrfErf(LPXLFOPER xlX)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  double x = XlfOper(xlX).AsDouble();
  ErrorFunction f;

  return XlfOper(f.erf(x));
  EXCEL_END;
}


LPXLFOPER EXCEL_EXPORT xlOrfInvErf(LPXLFOPER xlX)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  double x = XlfOper(xlX).AsDouble();
  ErrorFunction f;

  return XlfOper(f.inverf(x));
  EXCEL_END;
}


LPXLFOPER EXCEL_EXPORT xlOrfNormalCdf(LPXLFOPER xlX)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  double x = XlfOper(xlX).AsDouble();
  NormalDistribution f;

  return XlfOper(f.cdf(x));
  EXCEL_END;
}


LPXLFOPER EXCEL_EXPORT xlOrfNormalInvCdf(LPXLFOPER xlX)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  double x = XlfOper(xlX).AsDouble();
  NormalDistribution f;

  return XlfOper(f.invcdf(x));
  EXCEL_END;
}


LPXLFOPER EXCEL_EXPORT xlOrfFwdPrice(LPXLFOPER xlSpot,
                                     LPXLFOPER xlTimeToExp,
                                     LPXLFOPER xlIntRate,
                                     LPXLFOPER xlDivYield)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  double spot = XlfOper(xlSpot).AsDouble();
  double timeToExp = XlfOper(xlTimeToExp).AsDouble();
  double intRate = XlfOper(xlIntRate).AsDouble();
  double divYield = XlfOper(xlDivYield).AsDouble();

  double fwd = fwdPrice(spot, timeToExp, intRate, divYield);

  return XlfOper(fwd);
  EXCEL_END;
}


LPXLFOPER EXCEL_EXPORT xlOrfDigiBS(LPXLFOPER xlPayoffType,
                                   LPXLFOPER xlSpot,
                                   LPXLFOPER xlStrike,
                                   LPXLFOPER xlTimeToExp,
                                   LPXLFOPER xlIntRate,
                                   LPXLFOPER xlDivYield,
                                   LPXLFOPER xlVolatility)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  int payoffType = XlfOper(xlPayoffType).AsInt();
  double spot = XlfOper(xlSpot).AsDouble();
  double strike = XlfOper(xlStrike).AsDouble();
  double timeToExp = XlfOper(xlTimeToExp).AsDouble();
  double intRate = XlfOper(xlIntRate).AsDouble();
  double divYield = XlfOper(xlDivYield).AsDouble();
  double vol = XlfOper(xlVolatility).AsDouble();

  double price = digitalOptionBS(payoffType, spot, strike, timeToExp, intRate, divYield, vol);

  return XlfOper(price);
  EXCEL_END;
}

END_EXTERN_C
