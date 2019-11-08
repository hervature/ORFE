#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

class Integrator
{
public:
  virtual ~Integrator() {}

  // `Step()` advances an ODE by one timestep (x[] is overwritten with
  // updated values at t+dt)
  virtual int Step(double t, double *x) = 0;
};

#endif  // INTEGRATOR_H_
