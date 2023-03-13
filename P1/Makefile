EXES = test-P1-auto test-P1-consola
CXX = g++
CXXFLAGS = -g -Wall -std=c++14 -pedantic
CPPFLAGS = -DP1-I../Tests-auto -I.
VPATH = ../Tests-auto:.
CLASSES = cadena.o fecha.o
.PHONY: all clean
all: $(EXES)
test-P1-auto: test-caso0-fecha-auto.o test-caso0-cadena-auto.o test-auto.o $(CLASSES)
	$(CXX) $(CXXFLAGS) $^ -o $@
test-caso0-fecha-auto.o test-caso0-cadena-auto.o test-auto.o: \
test-caso0-fecha-auto.cpp test-caso0-cadena-auto.cpp \
test-auto.cpp test-auto.hpp fecha.hpp cadena.hpp
test-P1-consola: test-fechacadena-consola.o $(CLASSES)
	$(CXX) $(CXXFLAGS) $^ -o $@
test-fechacadena-consola.o: \
test-fechacadena-consola.cpp fecha.hpp cadena.hpp
clean:
	@$(RM) $(EXES) *.o
