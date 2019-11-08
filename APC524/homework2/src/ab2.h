#ifndef AB2_H_
#define AB2_H_

#include "integrator.h"
class Model;      // Forward declaration

class AB2 : public Integrator
{
public:
  AB2(double dt, const Model &model); // constructor
  ~AB2();       // destructor

  int Step(double t, double *x);
  
private:
  const double dt_;     // timestep
  const Model &model_;    // functor to evaluate f(x,t)
  const int dimen_;   // dimension of state x

  double *fx_;      // will point to temporary scratch
                    // space to hold f(x,t)
  double *fx_prev_;      // will point to temporary scratch
                    // space to hold f(x_prev,t_prev)
  double *x_prev_;  // will point to temporary scratch
                    // space to hold x_prev

  bool init_flag_;
};

#endif  // AB2_H_
