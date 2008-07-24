# GServer2 Makefile

# Definitions
INCDIR  = .
LIBDIR  = ../lib
SOURCES = $(dir)

LD = g++
CFLAGS = -I$(INCDIR) -Wall -O2
LDFLAGS = -L$(LIBDIR) -lstdc++ -lc -lm -lz

# Generate List of Files
CPPS = $(wildcard *.cpp)
OBJS = $(CPPS:.cpp=.o)

# Actual Makefile
all: $(CPPS) gserver2

%.o: %.cpp
	$(LD) $(CFLAGS) -c $<

gserver2: $(OBJS)
	$(LD) -o ../bin/GServer2 $(OBJS) $(CPPOFDB) $(LDFLAGS)

help:
	@echo "GServer2 Makefile"
	@echo
	@echo "make"
	@echo "  Compiles GServer2 for 217"
	@echo
	@echo "make clean_o"
	@echo "  Only clean objects"
	@echo
	@echo "make clean"
	@echo "  Clean everything up"

clean_o:
	rm -rf *.o

clean:
	rm -rf *.o ../bin/GServer2
