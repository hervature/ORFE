/*
 * ab2.c
 */
#include "integrator.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Integrator object for 2nd-order Adams-Bashforth method */
struct integrator_t
{
  int n;		   	/* dimension of state vector */
  double dt;		  	/* time step */
  FuncPtr rhs;		      	/* right-hand-side of \dot x = f(x,t) */
  double x_prev[]; /* keep track of previous state */
};
  
/* "Constructor" */
Integrator *integrator_new(const int n, const double dt, FuncPtr rhs)
{
  // account for dynamic array of x_prev
  Integrator *integrator = (Integrator *) malloc(sizeof(*integrator) + n * sizeof(double));
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

/* Stepper */
int integrator_step(Integrator *integrator, double t, double *x)
{
  assert(integrator);
  const int n = integrator->n;	/* Shorthand `n` for use
				   in-function */
  double fx[n];			/* NB: This is a VLA!! */
  double fx_prev[n];

  /* Bubble up any errors from RHS function */
  int rhserr = integrator->rhs(n, t, x, fx);
  if (rhserr != 0)
  {
    return rhserr;
  }

  if (t > integrator->dt)
  {
    // perform Adams-Bashforth algorithm

    rhserr = integrator->rhs(n, t-integrator->dt, integrator->x_prev, fx_prev);
    if (rhserr != 0)
    {
      return rhserr;
    }

    /* 2nd-order Adams-Bashforth algorithm for dx */
    for (int i = 0; i < n; ++i)
    {
      integrator->x_prev[i] = x[i];
      x[i] += 3.0/2.0*integrator->dt * fx[i] - 1/2.0*integrator->dt * fx_prev[i];
    }
  }
  else
  {
    // default to Forward Euler
    for (int i = 0; i < n; ++i)
    {
      x[i] += integrator->dt * fx[i];
    }
  }

  /* Successful exit */
  return 0;
}
