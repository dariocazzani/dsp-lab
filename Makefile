CXX=g++
CXXFLAGS=-Wall -O2 -std=c++11 -I/opt/homebrew/include -I./include
LDFLAGS=-L/opt/homebrew/lib -lportaudio

SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin


SOURCES=$(wildcard $(SRC_DIR)/*.cc)
OBJECTS=$(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET=$(BIN_DIR)/main

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/lowpass_filter.o: $(SRC_DIR)/lowpass_filter.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	rm -f $(OBJECTS) $(TARGET)