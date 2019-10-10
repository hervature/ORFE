/*
 * rk4.c
 */
#include "integrator.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Integrator object for 4th-order Runge-Kutta method */
struct integrator_t
{
  int n;		   	/* dimension of state vector */
  double dt;		  	/* time step */
  FuncPtr rhs;		      	/* right-hand-side of \dot x = f(x,t) */
};
  
/* "Constructor" */
Integrator *integrator_new(const int n, const double dt, FuncPtr rhs)
{
  Integrator *integrator = (Integrator *) malloc(sizeof(*integrator));
  assert(integrator);
  
  integrator->n = n;
  integrator->dt = dt;
  integrator->rhs = rhs;

  return integrator;
}

/* "Destructor" */
void integrator_free(Integrator *integrator)
{
  assert(integrator);
  free(integrator);
}

// add arrays x and y and store in res
void add(int n, double *x, double *y, double *res)
{
  for(int i = 0; i < n; ++i)
  {
    res[i] = x[i]+y[i];
  }
}

// mutiply array x by r and store in res
void mult(int n, double *x, double r, double *res)
{
  for(int i = 0; i < n; ++i)
  {
    res[i] = r*x[i];
  }
}

int calculate_k_i(Integrator *integrator, int n, double t, double dt, double dt_coeff, double *x, double *k_i_1, double k_i_1_coeff, double *k_i)
{
  double tmp[n];
  mult(n, k_i_1, k_i_1_coeff, tmp);
  add(n, x, tmp, tmp);
  int rhserr = integrator->rhs(n, t+(dt*dt_coeff), tmp, k_i);
  if (rhserr != 0)
  {
    return rhserr;
  }
  
  mult(n, k_i, dt, k_i);

  return 0;
}


/* Stepper */
int integrator_step(Integrator *integrator, double t, double *x)
{
  assert(integrator);
  const int n = integrator->n;	/* Shorthand `n` for use
				   in-function */
  const double dt = integrator->dt;  /* Shorthand `dt` for use
           in-function */

  /* 4th-order Runge-Kutta algorithm for dx */
  double k_1[n]; double k_2[n]; double k_3[n]; double k_4[n];

  // k_1
  int rhserr = calculate_k_i(integrator, n, t, dt, 0.0, x, k_1, 0.0, k_1);
  if (rhserr != 0)
  {
    return rhserr;
  }

  // k_2
  rhserr = calculate_k_i(integrator, n, t, dt, 0.5, x, k_1, 0.5, k_2);
  if (rhserr != 0)
  {
    return rhserr;
  }

  // k_3
  rhserr = calculate_k_i(integrator, n, t, dt, 0.5, x, k_2, 0.5, k_3);
  if (rhserr != 0)
  {
    return rhserr;
  }

  // k_4
  rhserr = calculate_k_i(integrator, n, t, dt, 1.0, x, k_3, 1.0, k_4);
  if (rhserr != 0)
  {
    return rhserr;
  }

  for (int i = 0; i < n; ++i)
  {
    x[i] += (k_1[i]+k_4[i])/6.0+(k_2[i]+k_3[i])/3.0;
  }

  /* Successful exit */
  return 0;
}
