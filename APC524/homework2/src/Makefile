CXXFLAGS = -g -Wall -Wextra -std=c++17
LDFLAGS = -lm

integrators = euler rk4 ab2
models = ddo lv

driver := ode_solver

SOURCES = $(patsubst %, %.cc, $(integrators) $(models) $(driver))
OBJECTS = $(SOURCES:.cc=.o)
EXECUTABLE := $(driver)
DEPS = integrator.h

all : $(OBJECTS) $(EXECUTABLE)

$(OBJECTS) : %.o : %.cc $(DEPS)
	$(CXX) -c $(CXXFLAGS) $<

$(EXECUTABLE) : % : %.o
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)


clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

