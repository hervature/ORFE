/** 
  @file  xlregister0.cpp
  @brief Registration of Excel callable functions
*/

#include <xlw/xlw.h>
using namespace xlw;

namespace {

// Register the function ORF.VERSION
XLRegistration::Arg OrfVersionArgs[] = {
  { }
};
XLRegistration::XLFunctionRegistrationHelper regOrfVersion(
  "xlOrfVersion", "ORF.VERSION", "The orflib version string.",
  "ORFLIB", OrfVersionArgs, 0);

// Register the function ORF.MATRIXECHO
XLRegistration::Arg OrfMatrixEchoArgs[] = {
  { "Matrix", "Input matrix", "XLF_OPER" }
};
XLRegistration::XLFunctionRegistrationHelper regOrfMatrixEcho(
  "xlOrfMatrixEcho", "ORF.MATRIXECHO", "Echoing the input matrix",
  "ORFLIB", OrfMatrixEchoArgs, 1);

// Register the function ORF.PPOLYEVAL
XLRegistration::Arg OrfPPolyEvalArgs[] = {
  { "BkPoints", "vector of breakpoints", "XLF_OPER" },
  { "Values", "vector of values", "XLF_OPER" },
  { "PolyOrder", "0: piecewise flat; 1: linear continuous", "XLF_OPER" },
  { "X", "Vector of x-vals at which the curve is evaluated", "XLF_OPER" },
  { "DerivOrder", "the order of the derivative to be evaluated; 0 for values", "XLF_OPER" }
};
XLRegistration::XLFunctionRegistrationHelper regOrfPPolyEval(
  "xlOrfPPolyEval", "ORF.PPOLYEVAL", "Creates and evaluates a piecewise polynomial curve ",
  "ORFLIB", OrfPPolyEvalArgs, 5);

// Register the function ORF.PPOLYINTEGRAL
XLRegistration::Arg OrfPPolyIntegralArgs[] = {
  { "BkPoints", "vector of breakpoints", "XLF_OPER" },
  { "Values", "vector of values", "XLF_OPER" },
  { "PolyOrder", "0: piecewise flat; 1: linear continuous", "XLF_OPER" },
  { "XStart", "The x value at which every integration starts", "XLF_OPER" },
  { "XEnd", "Vector of x-vals at which each integration ends", "XLF_OPER" }
};
XLRegistration::XLFunctionRegistrationHelper regOrfPPolyIntegral(
  "xlOrfPPolyIntegral", "ORF.PPOLYINTEGRAL", "Creates and integrates a piecewise polynomial curve ",
  "ORFLIB", OrfPPolyIntegralArgs, 5);

// Register the function ORF.PPOLYSUM
XLRegistration::Arg OrfPPolySumArgs[] = {
  { "BkPoints1", "First vector of breakpoints", "XLF_OPER" },
  { "Values1", "First vector of values", "XLF_OPER" },
  { "BkPoints2", "Second vector of breakpoints", "XLF_OPER" },
  { "Values2", "Second vector of values", "XLF_OPER" },
  { "PolyOrder", "Common ppoly order; 0: piecewise flat; 1: linear continuous", "XLF_OPER" },
};
XLRegistration::XLFunctionRegistrationHelper regOrfPPolySum(
  "xlOrfPPolySum", "ORF.PPOLYSUM", "Creates two polynomial curves and adds them; returns bkpts and values ",
  "ORFLIB", OrfPPolySumArgs, 5);

}

