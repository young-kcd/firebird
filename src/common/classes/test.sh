# Test for library integrity
# this should be compiled with optimization turned off and with NDEBUG undefined
ulimit -s unlimited
g++ -ggdb -Wall class_test.cpp alloc.cpp 2> aa
./a.out

# Chose the best algorithm parameters for the target architecture
g++ -O3 -march=pentium3 -DNDEBUG -I../../include class_perf.cpp alloc.cpp \
../memory/memory_pool.cpp ../fb_exception.cpp ../memory/allocators.cpp 2> aa
#g++ -ggdb -I../../include class_perf.cpp alloc.cpp \
#../memory/memory_pool.cpp ../fb_exception.cpp ../memory/allocators.cpp 2> aa
./a.out
