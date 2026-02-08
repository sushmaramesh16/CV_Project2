# Makefile for CV Project 2
# Image matching system

CXX = g++
CXXFLAGS = -std=c++11 -Wall
OPENCV = `pkg-config --cflags --libs opencv4`

# If opencv4 doesn't work, try opencv instead
# OPENCV = `pkg-config --cflags --libs opencv`

SRC_DIR = src
BIN_DIR = bin

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

all: baseline_match histogram_match histogram_match_hsv multi_histogram_match color_texture_match

baseline_match: $(SRC_DIR)/baseline_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/baseline_match $(SRC_DIR)/baseline_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/csv_util.cpp $(OPENCV)

histogram_match: $(SRC_DIR)/histogram_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/histogram_match $(SRC_DIR)/histogram_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp $(OPENCV)

histogram_match_hsv: $(SRC_DIR)/histogram_match_hsv.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/histogram_match_hsv $(SRC_DIR)/histogram_match_hsv.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp $(OPENCV)

multi_histogram_match: $(SRC_DIR)/multi_histogram_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/multi_histogram_match $(SRC_DIR)/multi_histogram_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp $(OPENCV)

color_texture_match: $(SRC_DIR)/color_texture_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/color_texture_match $(SRC_DIR)/color_texture_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp $(OPENCV)

clean:
	rm -f $(BIN_DIR)/*
	rm -rf $(BIN_DIR)

.PHONY: all clean