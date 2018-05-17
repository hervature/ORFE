#include <xlw/xlw.h>

using namespace xlw;
using namespace std;

extern "C" {

LPXLFOPER EXCEL_EXPORT xlAmorTable(LPXLFOPER xlNPeriods, LPXLFOPER xlPeriodRate, LPXLFOPER xlAmount) {
EXCEL_BEGIN;

  if (XlfExcel::Instance().IsCalledByFuncWiz())
    return XlfOper(true);

  // allocate the returned matrix
  RW n1 = XlfOper(xlNPeriods).AsInt() + 1;
  COL n2 = 4;
  XlfOper ret(n1, n2);

  // Set the header
  ret.SetElement(0, 0, "Period");
  ret.SetElement(0, 1, "Interest");
  ret.SetElement(0, 2, "Principal");
  ret.SetElement(0, 3, "Balance");

  // Calculate the amortization table
  double periods = XlfOper(xlNPeriods).AsDouble();
  double rate = XlfOper(xlPeriodRate).AsDouble();
  double amount = XlfOper(xlAmount).AsDouble();
  double C;
  if (rate == 0) {
	  C = amount/periods;
  }
  else {
	  C = amount / ((pow(1 + rate, periods) - 1) / (rate*pow(1 + rate, periods)));
  }
  double P;
  double I;
  double B_old = amount;
  double B_new;

  for (RW i = 1; i < n1; i++) {
	I = B_old*rate;
    P = C - I;
	B_new = B_old - P;
	B_old = B_new;
	ret.SetElement(i, 0, i);
	ret.SetElement(i, 1, I);
	ret.SetElement(i, 2, P);
	ret.SetElement(i, 3, B_new);
  }
  return ret;
EXCEL_END;
}

} // extern "C"
