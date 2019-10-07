CFLAGS = -g -Wall -Wextra -pedantic --std=c11
LDFLAGS = -lm

integrators = euler rk4 ab2
# Driver is 'ddo' (Damped, Driven Oscillator)
driver := ddo

SOURCES = $(patsubst %, %.c, $(integrators) $(driver))
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLES := $(patsubst %, $(driver)_%, $(integrators))
DEPS = integrator.h

all : $(EXECUTABLES)

$(OBJECTS) : %.o : %.c $(DEPS)
	$(CC) -c $(CFLAGS) $<

$(EXECUTABLES) : $(driver)_% : %.o $(driver).o
	$(CC) -o $@ $^ $(LDFLAGS)


clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLES)

