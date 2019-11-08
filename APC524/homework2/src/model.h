#ifndef MODEL_H_
#define MODEL_H_

class Model
{
public:
  virtual ~Model() {}

  // Calculates d/dt of the state
  // variables `x` at `t` (using f(t,x) ), returns result in `fx[]`.
  //
  // Should return 0 if successful, nonzero if error.
  virtual int rhs(double t, const double *x, double *fx) const = 0;

  // "getter" method for number of state variables associated with the
  // system of ODEs (i.e. the size of x)
  virtual int dimen() const = 0;
};

#endif  // MODEL_H_
