# ass2f19-48-base

## Build Instruction

Type `make` or `make all`.

## Basic Syntax of `ode_solve`

Following the guidelines of the homework, the usage is:

`./ode_solve <model> <params> <ICs> <integrator> <timestep> <numsteps>`

Where

- \<model\> is either `ddo` or `lv`
- \<params\> is a space0separated list of parameters for the model (see implementations of models for the order)
- \<ICs\> is a space-separated list of initial conditions of the model in the form "t x_0 ... x_n"
- \<integrator\> is the type of integrator. `euler`, `ab2`, and `rk4` are implemented.
- \<timestep\> is the `dt` of your path simulation
- \<numsteps\> is the number of steps you wish to run your simulation for. Total time is timestep\*numsteps

E.g.:

`./ode_solver ddo "1.0 1.0 0.25 1.0" "0.0 0.0 0.0" rk4 0.188495559 100`
or
`./ode_solver lv "1.0 0.2 0.1 0.2" "0.0 1.0 2.0" rk4 0.1 500`

## Code Structure

### Integrator.h

This class is an API for different integration schemes. The API only expects an implementation about how to move forward a state at a given time. For our purposes, the implementation of the API requires a model and a timestep and are immutable. The implementations are:

- euler.cc: forward euler method
- ab2.cc: Adams-Bashforth method
- rk4.cc: The classic Runge-Kutta method

### Model.h

This class is an API of the state dynamics of our ODEs. It requires the rhs of a multi-dimensional system of the form \dot x = f(x,t). We've implemented the following models:

- ddo.cc: Damped-Driven Oscillator Model
- lv.cc: Lotka-Volterra Predator Prey Model