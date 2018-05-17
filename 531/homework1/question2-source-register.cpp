#include <xlw/xlw.h>
using namespace xlw;

// Registration of Excel functions
namespace 
{
  // Register the function SAYHELLO.
  XLRegistration::Arg AmorTableArgs[] = {
    { "NPeriods", "Number of Periods", "XLF_OPER" },
	{ "PeriodRate", "Interest rate", "XLF_OPER" },
	{ "Amount", "Principal Amount", "XLF_OPER" }
  };
  XLRegistration::XLFunctionRegistrationHelper registerAmorTable(
    "xlAmorTable", "AMORTABLE", "Calculates Amortization Table",
    "XlwTest", AmorTableArgs, 3);

} // namespace
