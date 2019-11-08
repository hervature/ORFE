#ifndef LV_H_
#define LV_H_

#include "model.h"

// Lotka-Volterra :
//   \dot R = alpha R - beta R F
//   \dot F = delta R F - gamma F
class LV : public Model
{
public:
  // Constructor (initialize with params)
  //
  // `params[]` should be an array of length 4, holding the following
  // parameters in order:
  // alpha --> per capita birth rate of prey
  // beta --> per capita death rate of prey
  // delta --> per capita reproduction rate for predator for each prey consumed
  // gamma --> per capita death rate for unfed predator
  LV(double *params);
  ~LV();
  
  int rhs(double t, const double *x, double *fx) const;

  // Functions *defined* in the header are automatically `inline`-d --
  // `dimen() is so simple that we want to help the compiler figure
  // out that it could (should?)  inline it
  int dimen() const
  {
    return dimen_;
  }
private:
  // Parameters of the prey
  const double alpha_;
  const double beta_;

  // Parameters of the predator
  const double delta_;
  const double gamma_;


  // Dimension of state (`static` --> shared by all LV instances)
  static const int dimen_ = 2;
};

#endif  // LV_H_
