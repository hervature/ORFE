#include "euler.h"
#include "model.h"

// Euler objects should be initialized with a timestep and a Model
Euler::Euler(double dt, const Model &model)
  : dt_(dt), model_(model), dimen_(model.dimen()) // Initialization
						  // list
{
  // Constructor pre-allocates heap space for rhs() results
  fx_ = new double[dimen_];
}

// Destructor housekeeping -- free space used for rhs() results
Euler::~Euler()
{
  delete [] fx_;
}


int Euler::Step(double t, double *x)
{
  model_.rhs(t, x, fx_);
  
  for (int i = 0; i < dimen_; ++i)
  {
    x[i] += dt_ * fx_[i];
  }
  
  return 0;
}
