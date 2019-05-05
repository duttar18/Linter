CXX = g++
CXXFLAGS = -Wall -Werror -pedantic --std=c++11 -g

lint: lint.exe
	./lint.exe

lint.exe: lint.cpp cppLint.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rvf lint.exe
