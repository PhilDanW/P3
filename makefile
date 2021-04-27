CXX = g++
CXXFLAGS = -std=c++0x -Wall -g -w

P2: main.cpp scanner.cpp parseTree.cpp
	$(CXX) $(CXXFLAGS) -o statSem main.cpp parseTree.cpp scanner.cpp semantics.cpp

clean:
	rm -f statSem *.o stdin.temp keyboard_input.txt
