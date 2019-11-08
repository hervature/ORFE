#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <cassert>

using namespace std;

// Integrators
#include "euler.h"
#include "ab2.h"
#include "rk4.h"

// Models
#include "ddo.h"
#include "lv.h"

void print_state(double t, vector<double> x)
{
  printf("%15.8f", t);
  for(unsigned long i = 0; i < x.size(); i++){
    printf(" %15.8f", x[i]);
  }
  printf("\n");
}

vector<double> explode(const string& str, const char& ch) {
    // take list of parameters as string and return vector of doubles
    string next;
    vector<double> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(stod(next));
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(stod(next));

    return result;
}

int path_sim(Integrator* integrator, vector<double> ICs, double dt, int N)
{
  // initialize everything, starting time in first spot
  double t = ICs[0];

  vector<double> x(ICs.size()-1);
  copy(++ICs.begin(), ICs.end(), x.begin());

  for (int i = 0; i < N; i++) {
    print_state(t, x);
    integrator->Step(t, &x[0]);
    t += dt;
  }
  print_state(t, x);

  return 0;
}

int main(int argc, char *argv[])
{

  // Error checking
  if ( argc != 7 ) {
    printf("Program expects 6 arguments: model, params, ICs, integrator, timestep, numsteps\n");
    return 1;
  }

  string model_str = argv[1];

  string params_str = argv[2];
  vector<double> params;
  params = explode(params_str, ' ');

  string ICs_str = argv[3];
  vector<double> ICs;
  ICs = explode(ICs_str, ' ');

  Model* model;

  if (model_str == "ddo") {
    // IC is t, 2 dimensional system
    assert(ICs.size() == 3);
    assert(params.size() == 4);

    model = new DDOscillator(&params[0]);
  }
  else if (model_str == "lv") {
    // Repetition but could change for different models
    assert(ICs.size() == 3);
    assert(params.size() == 4);

    model = new LV(&params[0]);
  }
  else {
    printf("Model specified is not available.");
    return 1;
  }

  string integrator_str = argv[4];
  double dt = stod(argv[5]);

  // time step must be positive
  assert(dt > 0);

  Integrator* integrator;

  if (integrator_str == "euler") {
    integrator = new Euler(dt, *model);
  }
  else if (integrator_str == "ab2") {
    integrator = new AB2(dt, *model);
  }
  else if (integrator_str == "rk4") {
    integrator = new RK4(dt, *model);
  }
  else {
    printf("Integrator specified is not available.");
    return 1;
  }

  int N = stoi(argv[6]);

  // number of steps should be positive
  assert(N > 0);

  return path_sim(integrator, ICs, dt, N);
}
