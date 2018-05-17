/** 
    @file  xlfunctions0.cpp
    @brief Implementation of Excel callable functions
*/

#include <xlw/xlw.h>
#include <orflib/defines.hpp>
#include <orflib/math/interpol/piecewisepolynomial.hpp>
#include <xlorflib/xlutils.hpp>

using namespace xlw;
using namespace orf;

BEGIN_EXTERN_C

LPXLFOPER EXCEL_EXPORT xlOrfVersion() 
{
EXCEL_BEGIN;
  return XlfOper(ORF_VERSION);
EXCEL_END;
}

LPXLFOPER EXCEL_EXPORT xlOrfMatrixEcho(LPXLFOPER xlMat)
{
  EXCEL_BEGIN;
  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  orf::Matrix mat = xlOperToMatrix(XlfOper(xlMat));
  XlfOper xlRange = xlMatrixToOper(mat);
  return XlfOper(xlRange);
  EXCEL_END;
}

LPXLFOPER EXCEL_EXPORT xlOrfPPolyEval(LPXLFOPER xlBkPoints,
  LPXLFOPER xlValues,
  LPXLFOPER xlPolyOrder,
  LPXLFOPER xlXVec,
  LPXLFOPER xlDerivOrder)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  Vector bkpts = xlOperToVector(XlfOper(xlBkPoints));
  Vector vals = xlOperToVector(XlfOper(xlValues));
  int degree = XlfOper(xlPolyOrder).AsInt();
  Vector xx = xlOperToVector(XlfOper(xlXVec));
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int derivOrder = XlfOper(xlDerivOrder).AsInt();

  // create the curve
  PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);

  Vector yy(xx.size());
  pp.eval(xx.begin(), xx.end(), yy.begin(), derivOrder);

  XlfOper xlRange = xlVectorToOper(yy);
  return XlfOper(xlRange);
  EXCEL_END;
}

LPXLFOPER EXCEL_EXPORT xlOrfPPolyIntegral(LPXLFOPER xlBkPoints,
  LPXLFOPER xlValues,
  LPXLFOPER xlPolyOrder,
  LPXLFOPER xlXStart,
  LPXLFOPER xlXVecEnd)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  Vector bkpts = xlOperToVector(XlfOper(xlBkPoints));
  Vector vals = xlOperToVector(XlfOper(xlValues));
  ORF_ASSERT(bkpts.size() == vals.size(), "unequal number of breakpoints and vals");
  int degree = XlfOper(xlPolyOrder).AsInt();
  double x0 = XlfOper(xlXStart).AsDouble();
  Vector xx = xlOperToVector(XlfOper(xlXVecEnd));
  // create the curve
  PiecewisePolynomial pp(bkpts.begin(), bkpts.end(), vals.begin(), degree);
  // integrate
  Vector yy(xx.size());
  pp.integral(x0, xx.begin(), xx.end(), yy.begin());

  XlfOper xlRange = xlVectorToOper(yy);
  return XlfOper(xlRange);
  EXCEL_END;
}

LPXLFOPER EXCEL_EXPORT xlOrfPPolySum(LPXLFOPER xlBkPoints1,
  LPXLFOPER xlValues1,
  LPXLFOPER xlBkPoints2,
  LPXLFOPER xlValues2,
  LPXLFOPER xlPolyOrder)
{
  EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  Vector bkpts1 = xlOperToVector(XlfOper(xlBkPoints1));
  Vector vals1 = xlOperToVector(XlfOper(xlValues1));
  ORF_ASSERT(bkpts1.size() == vals1.size(), "unequal number of breakpoints and vals");

  Vector bkpts2 = xlOperToVector(XlfOper(xlBkPoints2));
  Vector vals2 = xlOperToVector(XlfOper(xlValues2));
  ORF_ASSERT(bkpts2.size() == vals2.size(), "unequal number of breakpoints and vals");

  int order = XlfOper(xlPolyOrder).AsInt();
  ORF_ASSERT(order < 2, "only polynomials of order 0 or 1 can be added with this function");

  // create the curves
  PiecewisePolynomial pp1(bkpts1.begin(), bkpts1.end(), vals1.begin(), order);
  PiecewisePolynomial pp2(bkpts2.begin(), bkpts2.end(), vals2.begin(), order);
  // add them
  PiecewisePolynomial psum = pp1 + pp2;
  Vector const& bkpts = psum.breakPoints();
  Vector vals(bkpts.size());
  psum.eval(bkpts.begin(), bkpts.end(), vals.begin(), 0);

  // return breakpoints and values
  XlfOper xlRange((RW)bkpts.size(), 2);
  for (RW i = 0; i < xlRange.rows(); ++i) {
    xlRange.SetElement(i, 0, bkpts(i));
    xlRange.SetElement(i, 1, vals(i));
  }

  return XlfOper(xlRange);
  EXCEL_END;
}


END_EXTERN_C
