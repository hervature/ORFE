#include "ab2.h"
#include "rk4.h"
#include "model.h"

// AB2 objects should be initialized with a timestep and a Model
AB2::AB2(double dt, const Model &model)
  : dt_(dt), model_(model), dimen_(model.dimen()) // Initialization
              // list
{
  // Constructor pre-allocates heap space for rhs() results
  fx_ = new double[dimen_];
  fx_prev_ = new double[dimen_];
  x_prev_ = new double[dimen_];

  // Needed to check if first run should default to RK4
  init_flag_ = true;
}

// Destructor housekeeping -- free space used for rhs() results
AB2::~AB2()
{
  delete [] fx_;
  delete [] fx_prev_;
  delete [] x_prev_;
}


int AB2::Step(double t, double *x)
{

  model_.rhs(t, x, fx_);

  if (!init_flag_)
  {
    // perform Adams-Bashforth algorithm
    model_.rhs(t-dt_, x_prev_, fx_prev_);

    /* 2nd-order Adams-Bashforth algorithm for dx */
    for (int i = 0; i < dimen_; ++i)
    {
      x_prev_[i] = x[i];
      x[i] += 3.0/2.0*dt_ * fx_[i] - 1/2.0*dt_ * fx_prev_[i];
    }
  }
  else
  {
    init_flag_ = false;

    // default to RK4
    RK4* rk4 = new RK4(dt_, model_);
    rk4->Step(t, x);
  }
  
  return 0;
}
