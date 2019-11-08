#ifndef EULER_H_
#define EULER_H_

#include "integrator.h"
class Model;			// Forward declaration

class Euler : public Integrator
{
public:
  Euler(double dt, const Model &model); // constructor
  ~Euler();				// destructor

  int Step(double t, double *x);
  
private:
  const double dt_; 		// timestep
  const Model &model_;		// functor to evaluate f(x,t)
  const int dimen_;		// dimension of state x

  double *fx_;			// will point to temporary scratch
				// space to hold f(x,t)
};

#endif  // EULER_H
