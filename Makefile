CXX=g++
CXXFLAGS=-Wall -O2 -std=c++11 -I/opt/homebrew/include
LDFLAGS=-L/opt/homebrew/lib -lportaudio

all: filter

filter: filter.cc
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f filter
