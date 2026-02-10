# -----------------------------
# CV Project 2 - Makefile
# Builds Tasks 1-4 + Extension + Task 5 + Task 7
# -----------------------------

CXX      = g++
CXXFLAGS = -std=c++11 -Wall

# OpenCV flags (Homebrew on Mac usually uses opencv4)
OPENCV = `pkg-config --cflags --libs opencv4`

SRC_DIR = src
BIN_DIR = bin

# Make sure bin/ exists
$(shell mkdir -p $(BIN_DIR))

# -----------------------------
# Programs to build (binaries go into bin/)
# -----------------------------
ALL_PROGS = baseline_match histogram_match histogram_match_hsv multi_histogram_match color_texture_match laws_texture_match task5_dnn task7_custom

all: $(ALL_PROGS)

# -----------------------------
# Task 1-4 binaries
# -----------------------------
baseline_match: $(SRC_DIR)/baseline_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/baseline_match $^ $(OPENCV)

histogram_match: $(SRC_DIR)/histogram_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/histogram_match $^ $(OPENCV)

histogram_match_hsv: $(SRC_DIR)/histogram_match_hsv.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/histogram_match_hsv $^ $(OPENCV)

multi_histogram_match: $(SRC_DIR)/multi_histogram_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/multi_histogram_match $^ $(OPENCV)

color_texture_match: $(SRC_DIR)/color_texture_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/color_texture_match $^ $(OPENCV)

# -----------------------------
# Extension: Laws Texture
# -----------------------------
laws_texture_match: $(SRC_DIR)/laws_texture_match.cpp $(SRC_DIR)/features.cpp $(SRC_DIR)/distance.cpp $(SRC_DIR)/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/laws_texture_match $^ $(OPENCV)

# -----------------------------
# Task 5 binary (ResNet CSV matcher)
# -----------------------------
task5_dnn: $(SRC_DIR)/task5_dnn.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/task5_dnn $^ $(OPENCV)

# -----------------------------
# Task 7 binary (Custom CBIR: DNN + HSV + Edges)
# -----------------------------
task7_custom: $(SRC_DIR)/task7_custom.cpp
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/task7_custom $^ $(OPENCV)

# -----------------------------
# Clean
# -----------------------------
clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean $(ALL_PROGS)