/*
 * ddo.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
  #define M_PI (3.14159265358979323846264338327950288)
#endif

#include "integrator.h"

double omega = 0.5;

void print_state(double t, double *x)
{
  printf("%15.8f %15.8f %15.8f\n", t, x[0], x[1]);
}

int ddo_dynamics(int n, double t, const double *x, double *Dx)
{

  double omega_0, beta;
  // ODE parameters (fixed)
  omega_0 = 1.0;
  beta = 0.25;

  // For vectorizing n-th order ODE
  for (int i = 0; i < n - 1; i++)
  {
    Dx[0] += x[i+1];
  }

  Dx[n-1] += cos(omega*t) - omega_0*omega_0*x[0] - 2*beta*x[1];

  return 0;
}

int ddo_path_sim(int N)
{

  double t_0, t_N;

  // time scale and initial boundary conditions
  t_0 = 0.0;
  t_N = 6*M_PI;

  // initial conditions
  double t, dt;
  int state_dim = 2; int error;
  double x[2] = {0.0, 0.0};

  dt = t_N / N;
  t = t_0;

  // Integrator
  FuncPtr rhs = &ddo_dynamics;
  Integrator *integrator = integrator_new(state_dim, dt, rhs);

  print_state(t, x);

  for (int i=0; i < N; ++i)
  {
    error = integrator_step(integrator, t, x);

    if (error != 0)
    {
      return error;
    }

    t += dt;
    print_state(t, x);
  }

  printf("\n");
  integrator_free(integrator);

  return 0;
}

int main(int argc, char *argv[])
{

  int omega_idx = 1; int N_idx = 2;

  // hyperparameters
  omega = strtod(argv[omega_idx], NULL);
  int N = atoi(argv[N_idx]);

  // Error checking
  if ( argc != 3 ) {
    printf("Program expects 2 arguments: omega and N\n");
    return 1;
  }

  if ( omega < 0.0) {
    printf("A non positive driving frequency is non-sensical\n");
    return 1;
  }

  if ( N <= 0) {
    printf("Number of timesteps must be a positive integer\n");
    return 1;
  }



  return ddo_path_sim(N);
}
