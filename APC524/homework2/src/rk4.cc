#include "rk4.h"
#include "model.h"

// RK4 objects should be initialized with a timestep and a Model
RK4::RK4(double dt, const Model &model)
  : dt_(dt), model_(model), dimen_(model.dimen()) // Initialization
              // list
{
  // Constructor pre-allocates heap space for rhs() results
  k_1 = new double[dimen_];
  k_2 = new double[dimen_];
  k_3 = new double[dimen_];
  k_4 = new double[dimen_];
}

// Destructor housekeeping -- free space used for rhs() results
RK4::~RK4()
{
  delete [] k_1;
  delete [] k_2;
  delete [] k_3;
  delete [] k_4;
}

// add arrays x and y and store in res
void RK4::add_arrays_(double *x, double *y, double *res)
{
  for (int i = 0; i < dimen_; ++i)
  {
    res[i] = x[i]+y[i];
  }
}

// mutiply array x by r and store in res
void RK4::mult_array_(double *x, double r, double *res)
{
  for (int i = 0; i < dimen_; ++i)
  {
    res[i] = r*x[i];
  }
}

void RK4::calculate_k_i(double t, double dt_coeff, double *x, double *k_i_1, double k_i_1_coeff, double *k_i)
{
  double tmp[dimen_];
  this->mult_array_(k_i_1, k_i_1_coeff, tmp);
  this->add_arrays_(x, tmp, tmp);
  model_.rhs(t+(dt_*dt_coeff), tmp, k_i);
  
  this->mult_array_(k_i, dt_, k_i);
}


int RK4::Step(double t, double *x)
{

  // k_1
  calculate_k_i(t, 0.0, x, k_1, 0.0, k_1);

  // k_2
  calculate_k_i(t, 0.5, x, k_1, 0.5, k_2);

  // k_3
  calculate_k_i(t, 0.5, x, k_2, 0.5, k_3);

  // k_4
  calculate_k_i(t,  1.0, x, k_3, 1.0, k_4);

  for (int i = 0; i < dimen_; ++i)
  {
    x[i] += (k_1[i]+k_4[i])/6.0+(k_2[i]+k_3[i])/3.0;
  }
  
  return 0;
}
