/**
    @file  brownianbridge.hpp
    @brief Brownian bridge path generator
*/

#ifndef ORF_BROWNIANBRIDGE_HPP
#define ORF_BROWNIANBRIDGE_HPP

#include <orflib/methods/montecarlo/pathgenerator.hpp>
#include <orflib/math/random/rng.hpp>
#include <list>

BEGIN_NAMESPACE(orf)

/** Creates standard normal increments populating the time line sequentially.
    It is templetized on the underlying normal deviate generator.
*/
template <typename NRNG>
class BrownianBridge : public PathGenerator
{
public:

  /** Structure with info about a bridge point, its neighbors, weights and priority */
  struct BridgePoint
  {
    ptrdiff_t first_point;
    ptrdiff_t second_point;
    ptrdiff_t middle_point;

    double first_weight;
    double second_weight;
    double volatility;
    int priority;

    // comparison op, allows for ordering of Bridge points by priority
    bool operator <(BridgePoint const& rhs)
    {
      return (priority < rhs.priority);
    }
  };


  /** Ctor for generating increments for correlated factors.
      If the correlation matrix is not passed in, it assumes independent factors
  */
  template<typename ITER>
  BrownianBridge(ITER timestepsBegin, ITER timestepsEnd, size_t nfactors,
                 Matrix const& correlMat = Matrix());

  /** Returns the dimension of the generator */
  size_t dim() const;

  /** Returns the next price path */
  virtual void next(Matrix& pricePath) override;

protected:

  // helper method for creating the list of bridge points
  template<typename ITER>
  void initBridgePoints(ITER timestepsBegin, ITER timestepsEnd,
    ITER first_point, ITER last_point,
    int priority = 1);

  // helper method that creates the actual path
  void createPath(double sqrtLastTime, Vector const& normalDevs, Matrix& path, size_t factorIdx);

  // state
  NRNG nrng_;
  std::list<BridgePoint> bridgePoints_;    // the list of bridge points sorted by priority
  double sqrtLastTime_;                    // the square root of the last time step
  Vector normalDevs_;                      // scratch array
};


///////////////////////////////////////////////////////////////////////////////
// Inline definitions

template <typename NRNG>
template <typename ITER>
inline BrownianBridge<NRNG>::BrownianBridge(ITER timestepsBegin,
                                            ITER timestepsEnd,
                                            size_t nfactors,
                                            Matrix const& correlMat)
  : PathGenerator((timestepsEnd - timestepsBegin), nfactors, correlMat),
  nrng_((timestepsEnd - timestepsBegin) * nfactors, 0.0, 1.0)
{
  sqrtLastTime_ = sqrt(*(timestepsEnd - 1));
  Vector timePoints(ntimesteps_ + 1);    // temp array with times, including time 0.0 point
  timePoints[0] = 0.0;
  std::copy(timestepsBegin, timestepsEnd, timePoints.begin() + 1);
  initBridgePoints(timePoints.begin(), timePoints.end(), timePoints.begin(), timePoints.end() - 1);
  bridgePoints_.sort();
  normalDevs_.resize(timestepsEnd - timestepsBegin);
}

template <typename NRNG>
template <typename ITER>
inline void
BrownianBridge<NRNG>::initBridgePoints(ITER timestepsBegin,
                                       ITER timestepsEnd,
                                       ITER first_point,
                                       ITER last_point,
                                       int priority)
{
  if (last_point - first_point <= 1) {
    // there is no point in between, nothing to do
    return;
  }

  double T1 = *first_point;
  double T2 = *last_point;
  ITER mid;

  if (T1 == T2) {
    // nothing to do
    return;
  }
  if (T1 > T2) {
    throw Exception("Time steps must be increasing order");
  }
  if (last_point - first_point == 2) {
    // there is only one point in between
    // no need to search further
    mid = first_point + 1;
  }
  else {
    double T = (T1 + T2) / 2.0;

    // find closest to middle point
    mid = upper_bound(first_point + 1, last_point + 1, T);

    if (mid > first_point + 1) {
      // there's potentially a choice between the time step just above T and the one just below T
      if (fabs(*mid - T) - fabs(*(mid - 1) - T) > DBL_EPSILON)
        mid--;
    }
    if (mid == first_point)
      mid++;
    if (mid == last_point)
      mid--;
  }

  BridgePoint point;
  point.first_point = first_point - timestepsBegin;
  point.second_point = last_point - timestepsBegin;
  point.middle_point = mid - timestepsBegin;

  double Ti = *mid;

  point.first_weight = (T2 - Ti) / (T2 - T1);
  point.second_weight = (Ti - T1) / (T2 - T1);
  point.volatility = sqrt((Ti - T1) * (T2 - Ti) / (T2 - T1));
  point.priority = priority;
  bridgePoints_.push_back(point);
  // recursive call, create new bridge points to the left and to the right of mid
  initBridgePoints(timestepsBegin, timestepsEnd, first_point, mid, 2 * priority);
  initBridgePoints(timestepsBegin, timestepsEnd, mid, last_point, 2 * priority);

}


template <typename NRNG>
inline void
BrownianBridge<NRNG>::createPath(double sqrtLastTime,
                                 Vector const& normalDevs,
                                 Matrix& path,
                                 size_t factorIdx)
{
  ORF_ASSERT(path.n_rows== normalDevs.size() + 1,
    "the path must have one more row than the number of normal deviates!");

  path(0, factorIdx) = 0.0;
  // generate last point
  int it2 = 0;
  path(path.n_rows - 1, factorIdx) = sqrtLastTime_ * normalDevs[it2];
  it2++;
  std::list <BridgePoint>::const_iterator it;
  for (it = bridgePoints_.begin(); it != bridgePoints_.end(); ++it, ++it2) {
    ptrdiff_t i1 = it->first_point;
    ptrdiff_t i2 = it->second_point;
    ptrdiff_t i = it->middle_point;
    double w1 = it->first_weight;
    double w2 = it->second_weight;
    double v = it->volatility;
    path(i, factorIdx) = w1 * path(i1, factorIdx) + w2 * path(i2, factorIdx) + v * normalDevs[it2];
  }
  return;
}

template <typename NRNG>
inline void BrownianBridge<NRNG>::next(Matrix& pricePath)
{
  pricePath.resize(ntimesteps_ + 1, nfactors_);

  for (size_t j = 0; j < nfactors_; ++j) {
    nrng_.next(normalDevs_.begin(), normalDevs_.end());
    createPath(sqrtLastTime_, normalDevs_, pricePath, j);
  }
  // now compute the increments
  for (size_t j = 0; j < nfactors_; ++j)
    for (size_t i = 0; i < ntimesteps_; ++i)
      pricePath(i, j) = pricePath(i + 1, j) - pricePath(i, j);
  // remove the top row
  pricePath.resize(ntimesteps_, nfactors_);

  // finally apply the Cholesky factor if not empty
  if (sqrtCorrel_.n_rows != 0) {
    for (size_t i = 0; i < ntimesteps_; ++i) {
      for (size_t j = 0; j < nfactors_; ++j) {
        double sum = 0.0;
        for (size_t k = 0; k < nfactors_; ++k) {
          sum += sqrtCorrel_(j, k) * pricePath(i, k);
        }
        pricePath(i, j) = sum;
      }
    }
  }
}

END_NAMESPACE(orf)

#endif // ORF_BROWNIANBRIDGE_HPP
