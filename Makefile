CXX = g++
STD = -std=c++23
RELEASE = -g

no-locks:
	$(CXX) $(STD) $(RELEASE) slb2-no-locks.cpp -o no-locks

with-locks:
	$(CXX) $(STD) $(RELEASE) slb2.cpp -o with-locks

clean:
	rm with-locks no-locks