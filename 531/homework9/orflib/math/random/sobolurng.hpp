/**
*   @file  sobolurng.hpp
*   @brief Generator of Sobol sequences
*/

#ifndef ORF_SOBOLURNG_HPP
#define ORF_SOBOLURNG_HPP


#include <orflib/defines.hpp>
#include <orflib/exception.hpp>
#include <vector>


BEGIN_NAMESPACE(orf)

/** Generator of a Sobol low discrepancy sequence.
*/
class SobolURng
{

public:

  /** Required for compatibility with std generators */
  typedef double result_type;

  /** Initializing ctor */
  explicit SobolURng(size_t dimension);

  /** Dtor */
  ~SobolURng();

  /** Returns the dimension of the generator */
  size_t dim() const;

  /** Returns a batch of random deviates
      CAUTION: it requires end - begin == dimension() 
   */
  template <typename ITER>
  void next(ITER begin, ITER end);

  /** Returns the next Sobol number.
      This method is provided to make SobolURng compatible with the URNGs in std.
      It should be called exactly dim() times to get one Sobol point (vector).
  */
  double operator()();

  double min() { return 2.0e-16; }

  double max() { return 1.0; }

  /** A Sobol generator cannot be seeded like a pseudorandom generator.
      This is for compatibility with URNGs.
      */
  void seed(unsigned long x0 = 0) {};

protected:

  /** Method with the initializing logic */
  void init(size_t dimension);

private:
  // no access to copy ctor or assignment op 
  SobolURng(SobolURng const&);
  SobolURng& operator=(SobolURng const&);

  // state
  enum { MAXBIT = 30 };

  size_t  dim_;               // the number of dimensions
  std::vector<double> point_; // the Sobol point in dim_ dimensions
  size_t curridx_;            // the current index in the point_ vector

  //	long*	pol;
  std::vector<long> otpol;  // vector with the polynomial encodings
  std::vector<long> deg;    // vector with the corresponding polynomial degrees
  long*	rinit;
  long	in;
  std::vector<long>	ix;   // the vector of components
  std::vector<long*>	iu;   // allows 2D access into rinit
  double	fac;              // the 1/2^MAXBIT normalizing factor

  // helper methods
  /** Initializes the polynomials */
  long polyInit(long dimension);
  /** Initializes direction numbers */
  long recInit(long dimension);

};

///////////////////////////////////////////////////////////////////////////////
// Inline definitions

inline
SobolURng::SobolURng(size_t dimension)
: dim_(dimension), point_(dimension), curridx_(dimension),
otpol(dimension + 1), deg(dimension + 1), ix(dimension + 1), iu(MAXBIT + 1)
{
  ORF_ASSERT(dimension > 0, "the dimension must be positive!");
  init(dimension);
}

inline
SobolURng::~SobolURng()
{
  free(rinit);
  rinit = 0;
}

inline
size_t SobolURng::dim() const
{
  return dim_;
}

template <typename ITER>
inline
void SobolURng::next(ITER begin, ITER end)
{
  size_t im = in++;
  size_t j;
  for (j = 1; j <= MAXBIT; ++j) {
    if (!(im & 1)) break;
    im >>= 1;
  }
  im = (j - 1) * dim_;

  ITER it = begin;
  for (size_t k = 1; k <= dim_; ++k, ++it) {
    ix[k] ^= rinit[im + k];
    *it = ix[k] * fac;
  }
  in++;
}

inline
double SobolURng::operator()()
{
  if (curridx_ == dim_) {  // generate a new point
    next(point_.begin(), point_.end());
    curridx_ = 0;
  }
  return point_[curridx_++];
}

END_NAMESPACE(orf)

#endif // ORF_SOBOLURNG_HPP
