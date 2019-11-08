#ifndef DDO_H_
#define DDO_H_

#include "model.h"

// Damped-Driven Oscillator (DDOscillator):
//   \dot x = v
//   \dot v = F cos(omega t) - omega0^2 x - 2 beta v 
class DDOscillator : public Model
{
public:
  // Constructor (initialize with params)
  //
  // `params[]` should be an array of length 4, holding the following
  // parameters in order:
  // omega --> driving frequency
  // F --> driving amplitude
  // beta --> damping coefficient
  // omega0 --> natural frequency
  DDOscillator(double *params);
  ~DDOscillator();
  
  int rhs(double t, const double *x, double *fx) const;

  // Functions *defined* in the header are automatically `inline`-d --
  // `dimen() is so simple that we want to help the compiler figure
  // out that it could (should?)  inline it
  int dimen() const
  {
    return dimen_;
  }
private:
  // Parameters of the driver
  const double omega_;
  const double F_;

  // Parameters of the oscillator itself
  const double beta_;
  const double omega0_;


  // Dimension of state (`static` --> shared by all DDO instances)
  static const int dimen_ = 2;
};

#endif  // DDO_H_
