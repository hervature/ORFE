#include "ddo.h"

#include <math.h>

DDOscillator::DDOscillator(double *params) : omega_(params[0]),
					     F_(params[1]),
					     beta_(params[2]),
					     omega0_(params[3])
{}				// Empty constructor body
				// (constructor's only job for this
				// class is to initialize some const
				// members, and const members must be
				// initialized in the initializer
				// list)

// Empty destructor (no extra memory/objects to free/delete)
DDOscillator::~DDOscillator() {}

// rhs()
int DDOscillator::rhs(double t, const double *x, double *fx) const
{
  fx[0] = x[1];
  fx[1] = F_*cos(omega_*t) - (omega0_*omega0_)*x[0] - 2.0*beta_*x[1];

  return 0;
}

