# Test for library integrity
# this should be compiled with optimization turned off and with NDEBUG undefined
ulimit -s unlimited
#g++ -ggdb -Wall class_test.cpp alloc.cpp
g++ -ggdb -Wall class_test.cpp
./a.out

# Chose the best algorithm parameters for the target architecture
g++ -O3 -march=pentium3 -DNDEBUG class_perf.cpp
./a.out
