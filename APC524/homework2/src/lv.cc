#include "lv.h"

#include <math.h>

LV::LV(double *params) : alpha_(params[0]),
               beta_(params[1]),
               delta_(params[2]),
               gamma_(params[3])
{}        // Empty constructor body
        // (constructor's only job for this
        // class is to initialize some const
        // members, and const members must be
        // initialized in the initializer
        // list)

// Empty destructor (no extra memory/objects to free/delete)
LV::~LV() {}

// rhs()
int LV::rhs(double t, const double *x, double *fx) const
{
  // hunting dynamics independent of time, this silences warnings
  (void) t;
  fx[0] = alpha_*x[0]-beta_*x[0]*x[1];
  fx[1] = delta_*x[0]*x[1] - gamma_*x[1];

  return 0;
}

