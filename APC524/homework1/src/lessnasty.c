/*
 * lessnasty.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
  #define M_PI (3.14159265358979323846264338327950288)
#endif

void ode_path_sim(double t_0, double t_N, double x_0, double dx_0, double omega, int N)
{

  double x, dx, x_old, dx_old, t, dt, omega_0, beta;

  // initial conditions
  dt = t_N / N;
  x = x_0;
  t = t_0;
  dx = dx_0;

  // ODE parameters (fixed)
  omega_0 = 1.0;
  beta = 0.25;

  printf("%15.8f %15.8f %15.8f\n", t, x, dx);

  for (int i=0; i < N; ++i){
    x_old = x;
    dx_old = dx;
    
    // ODE dynamics
    x += dt * dx_old;
    dx += dt * (cos(omega*t) - pow(omega_0, 2.0)*x_old - 2*beta*dx_old);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }

  printf("\n");
}

int main(int argc, char *argv[])
{

  int omega_idx = 1; int N_idx = 2;

  // hyperparameters
  double omega = strtod(argv[omega_idx], NULL);
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

  double t_0, x_0, dx_0, t_N;

  // time scale and initial boundary conditions
  t_0 = 0.0;
  t_N = 6*M_PI;
  x_0 = 0.0;
  dx_0 = 0.0;

  ode_path_sim(t_0, t_N, x_0, dx_0, omega, N);

  return 0;
}
