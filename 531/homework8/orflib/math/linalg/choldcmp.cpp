/**
*  @file   choldcmp.cpp
*  @brief  Implementation of the Cholesky decomposition
*/

#include <orflib/math/linalg/linalg.hpp>
#include <orflib/exception.hpp>

BEGIN_NAMESPACE(orf)

/** Cholesky decomposition of a positive semi-definite matrix inMat.
*/
void choldcmp(Matrix const& inMat, Matrix& outMat, bool upper)
{
  ORF_ASSERT(inMat.is_square(), "choldcmp: input matrix must be square!");
  if (upper)
    arma::chol(outMat, inMat);
  else // lower
    arma::chol(outMat, inMat, "lower");
  return;
}

END_NAMESPACE(orf)
