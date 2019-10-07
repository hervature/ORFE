/*
 * Some things that are a good idea to understand:
 *
 * (a) the malloc+sizeof trick/pattern used in the constructor below,
 *
 * (b) what the `assert` statement is doing in the constructor,
 *
 * (c) why I might have added `assert`s in the destructor and the
 * stepper (not strictly needed as a matter of best practice, but some
 * (including me) think it's good to include it in places like thi as
 * a form of mini-documentation about some things which should never
 * ever be false at this stage in the flow of the code;
 *
 * (d) What a VLA is (C99 or later), and why I might have used one in
 * the stepper instead of malloc-ing space for the result of the RHS
 * evaluation.
 *
 *
 * None of this is super mission critical, but they're some C tidbits,
 * some of which relate to general good coding practice, some to more
 * C-specific style things.
 *
 * Also make sure you notice the C brace style convention I'm using
 * here (BSD/Allman style with and indent of 2 spaces for each new
 * block-nesting level.  YOU MUST FOLLOW THAT STYLE ON THIS
 * ASSIGNMENT, JUST TO SEE WHETHER YOU CAN (adhering to the style of
 * an existing project/API is good practice; else, for your own stuff
 * or for projects you spearhead, just pick one of the widely
 * prevailing styles and be consistent).
 *
 * Delete this whole comment block from the version of the file that
 * you hand in.
 *
 */

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
