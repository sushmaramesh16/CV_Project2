# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

# Target directory
BINDIR = bin

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BINDIR))

# All targets
all: baseline_match histogram_match histogram_match_hsv multi_histogram_match \
     color_texture_match laws_texture_match gabor_texture_match task2_custom

# Baseline matching
baseline_match: src/baseline_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/baseline_match \
		src/baseline_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# RGB histogram matching
histogram_match: src/histogram_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/histogram_match \
		src/histogram_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# HSV histogram matching
histogram_match_hsv: src/histogram_match_hsv.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/histogram_match_hsv \
		src/histogram_match_hsv.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# Multi-histogram matching
multi_histogram_match: src/multi_histogram_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/multi_histogram_match \
		src/multi_histogram_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# Color + texture matching
color_texture_match: src/color_texture_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/color_texture_match \
		src/color_texture_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# Laws texture matching (Extension 1)
laws_texture_match: src/laws_texture_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/laws_texture_match \
		src/laws_texture_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# Gabor texture matching (Extension 2)
gabor_texture_match: src/gabor_texture_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/gabor_texture_match \
		src/gabor_texture_match.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# Custom task
task2_custom: src/task2_custom.cpp src/features.cpp src/distance.cpp src/csv_util.cpp
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/task2_custom \
		src/task2_custom.cpp src/features.cpp src/distance.cpp src/csv_util.cpp $(LDFLAGS)

# Clean
clean:
	rm -f $(BINDIR)/*

.PHONY: all clean