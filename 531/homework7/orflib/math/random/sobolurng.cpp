/**
    @file  sobolurng.cpp
    @brief Implementation of the Sobol sequence generator
*/

#include <orflib/math/random/sobolurng.hpp>
#include <orflib/math/random/primitivepolynomials.hpp>

BEGIN_NAMESPACE(orf)

long SobolURng::polyInit(long dimension)
{
  ORF_ASSERT(dimension <= MAX_PRIMITIVEPOLY, "too many dimensions in Sobol URNG");

  otpol[0] = 0;
  deg[0] = 0;

  long dimCount = 1;
  long degCount = 1;
  long curCount = 0;

  while (dimCount <= dimension && dimCount <= MAX_PRIMITIVEPOLY && degCount <= MAX_PRIMITIVEDEGREE) {
    if (PrimitivePolynomials[degCount - 1][curCount] < 0) {
      ++degCount;
      curCount = 0;
    }

    otpol[dimCount] = PrimitivePolynomials[degCount - 1][curCount];
    deg[dimCount] = degCount;

    // loop
    ++curCount;
    ++dimCount;
  }
  return degCount;
}


long SobolURng::recInit(long dimension)
{
  // initialize the polynomials
  long maxdeg = polyInit(dimension);

  if (!maxdeg)
    return 0;

  // allocate the vector of the initial values
  rinit = (long*)calloc(dimension * MAXBIT + 1, sizeof(long));

  if (rinit == NULL)
    return 0;

  long lim = 2;
  // loop over degrees and dimensions and set the initial values
  for (long j = 0; j < maxdeg; ++j) {
    long val = 1;

    for (long k = 0; k < dimension; ++k) {
      val += 2;  // val ranges over the odd numbers
      rinit[1 + dimension * j + k] = val % lim;
    }
    lim <<= 1;  // times 2
  }

  return maxdeg;
}


void SobolURng::init(size_t dimension)
{
  // initialize polynomials and initial values
  long memerror = recInit((long) dimension);
  if (!memerror) return;

  // set vector iu to provide 2D access into rinit
  for (size_t j = 1, k = 0; j <= MAXBIT; j++, k += dimension)
    iu[j] = &rinit[k];

  for (size_t k = 1; k <= dimension; k++) {
    long poldeg = deg[k];

    for (size_t j = 1; j <= (size_t) poldeg; j++)
      iu[j][k] <<= (MAXBIT - j);

    for (size_t j = poldeg + 1; j <= MAXBIT; j++) {
      long ipp = otpol[k];
      long i = iu[j - poldeg][k];
      i ^= (i >> poldeg);

      for (ptrdiff_t l = poldeg - 1; l >= 1; l--) {
        if (ipp & 1) i ^= iu[j - l][k];
        ipp >>= 1;
      }
      iu[j][k] = i;
    }
  }

  fac = 1.00 / (1L << MAXBIT);
  in = (long) (dimension * dimension * dimension);

  // the otpol and deg vectors are not needed any more
  // we could reclaim their storage.
}

END_NAMESPACE(orf)
