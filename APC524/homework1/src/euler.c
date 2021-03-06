/*
 * euler.c
 */
#include "integrator.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Integrator object for Forward Euler method */
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

/* Stepper */
int integrator_step(Integrator *integrator, double t, double *x)
{
  assert(integrator);
  const int n = integrator->n;	/* Shorthand `n` for use
				   in-function */
  double fx[n];			/* NB: This is a VLA!! */

  /* Bubble up any errors from RHS function */
  int rhserr = integrator->rhs(n, t, x, fx);
  if (rhserr != 0)
  {
    return rhserr;
  }

  /* Forward Euler algorithm for dx */
  for (int i = 0; i < n; ++i)
  {
    x[i] += integrator->dt * fx[i];
  }

  /* Successful exit */
  return 0;
}
