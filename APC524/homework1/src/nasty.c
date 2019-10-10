/*
 * nasty.c
 */
#include <stdio.h>
#include <math.h>

int main()
{
  double t;
  double dt;
  double x;
  double dx;
  int i;
  
  t = 0.0;
  x = 0.0;
  dx = 0.0;
  printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  dt = 18.84955592153876 / 10;
  for (i=0; i < 10; ++i)
  {
    double xold = x;
    double dxold = dx;
    
    x += dt * dxold;
    dx += dt * (cos(0.5*t) - xold - 0.5*dxold);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }
  printf("\n");

  t = 0.0;
  x = 0.0;
  dx = 0.0;
  printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  dt = 18.84955592153876 / 20;
  for (i=0; i < 20; ++i)
  {
    double xold = x;
    double dxold = dx;
    
    x += dt * dxold;
    dx += dt * (cos(0.5*t) - xold - 0.5*dxold);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }
  printf("\n");

  t = 0.0;
  x = 0.0;
  dx = 0.0;
  printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  dt = 18.84955592153876 / 40;
  for (i=0; i < 40; ++i)
  {
    double xold = x;
    double dxold = dx;
    
    x += dt * dxold;
    dx += dt * (cos(0.5*t) - xold - 0.5*dxold);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }
  printf("\n");


  t = 0.0;
  x = 0.0;
  dx = 0.0;
  printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  dt = 18.84955592153876 / 10;
  for (i=0; i < 10; ++i)
  {
    double xold = x;
    double dxold = dx;
    
    x += dt * dxold;
    dx += dt * (cos(2.0*t) - xold - 0.5*dxold);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }
  printf("\n");

  t = 0.0;
  x = 0.0;
  dx = 0.0;
  printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  dt = 18.84955592153876 / 20;
  for (i=0; i < 20; ++i)
  {
    double xold = x;
    double dxold = dx;
    
    x += dt * dxold;
    dx += dt * (cos(2.0*t) - xold - 0.5*dxold);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }
  printf("\n");

  t = 0.0;
  x = 0.0;
  dx = 0.0;
  printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  dt = 18.84955592153876 / 40;
  for (i=0; i < 40; ++i)
  {
    double xold = x;
    double dxold = dx;
    
    x += dt * dxold;
    dx += dt * (cos(2.0*t) - xold - 0.5*dxold);
    t += dt;
    printf("%15.8f %15.8f %15.8f\n", t, x, dx);
  }
  printf("\n");


  return 0;
}
