CXX = clang++
STD = -std=c++23 -latomic
TEST = -g

slb: slb.hpp slb-test.cpp
	$(CXX) $(STD) $(TEST) slb-test.cpp -o slb

slb-atomics: slb-atomics.hpp slb-atomics-test.cpp
	$(CXX) $(STD) $(TEST) slb-atomics-test.cpp -o slb-atomics