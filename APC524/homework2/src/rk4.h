#ifndef RK4_H_
#define RK4_H_

#include "integrator.h"
class Model;      // Forward declaration

class RK4 : public Integrator
{
public:
  RK4(double dt, const Model &model); // constructor
  ~RK4();       // destructor

  int Step(double t, double *x);
  
private:
  const double dt_;     // timestep
  const Model &model_;    // functor to evaluate f(x,t)
  const int dimen_;   // dimension of state x

  void add_arrays_(double *x, double *y, double *res);
  void mult_array_(double *x, double r, double *res);
  void calculate_k_i(double t, double dt_coeff, double *x, double *k_i_1, double k_i_1_coeff, double *k_i);

  double *k_1; double *k_2; double *k_3; double *k_4;
};

#endif  // RK4_H_
