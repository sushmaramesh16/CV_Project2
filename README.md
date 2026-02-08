# CV Project 2: Content-Based Image Retrieval

A comprehensive image retrieval system implementing multiple matching methods using C++ and OpenCV.

## Author
Sushma Ravichandar
Northeastern University - MS in Data Science

## Project Overview

This project implements a content-based image retrieval (CBIR) system that can find similar images based on various visual features. The system includes 4 different matching methods with progressively sophisticated feature representations.

## Features Implemented

### Task 1: Baseline Matching
- **Method:** 7×7 center square with Sum of Squared Differences (SSD)
- **Feature Size:** 147 dimensions (7×7×3 RGB values)
- **Performance:** 100% match on test image pic.1016.jpg

### Task 2: Histogram Matching
- **RGB Histogram:** 8 bins per channel (512 total bins)
- **HSV Histogram:** 8×4×4 bins (128 total bins) - improved results
- **Distance Metric:** Histogram Intersection
- **Performance:** Successfully matched pic.0164.jpg to expected results

### Task 3: Multi-Histogram Matching (Spatial Layout)
- **Method:** Top half + bottom half RGB histograms
- **Feature Size:** 1024 dimensions (2×512 bins)
- **Distance Metric:** Weighted histogram intersection per region
- **Performance:** 100% match on pic.0274.jpg - all 3 expected images found

### Task 4: Color + Texture Features
- **Color:** RGB histogram (512 bins)
- **Texture:** Sobel gradient magnitude histogram (16 bins)
- **Feature Size:** 528 dimensions
- **Distance Metric:** Equal weighting (0.5 color + 0.5 texture)
- **Performance:** Demonstrates different matching behavior when texture is included

## Project Structure
```
CV_PROJECT2/
├── src/
│   ├── baseline_match.cpp          # Task 1: Baseline matching
│   ├── histogram_match.cpp         # Task 2: RGB histogram
│   ├── histogram_match_hsv.cpp     # Task 2: HSV histogram
│   ├── multi_histogram_match.cpp   # Task 3: Spatial histograms
│   ├── color_texture_match.cpp     # Task 4: Color + texture
│   ├── features.h/cpp              # Feature extraction functions
│   ├── distance.h/cpp              # Distance metrics
│   └── csv_util.h/cpp              # CSV file utilities
├── data/
│   └── olympus/                    # Image dataset (not included)
├── bin/                            # Compiled executables
├── features/                       # Saved feature files
├── results/                        # Output results
├── Makefile
└── README.md
```

## Dependencies

- **C++ Compiler:** g++ with C++11 support
- **OpenCV 4.x** (or OpenCV 3.x with minor modifications)
- **pkg-config** for OpenCV linking

### Installing OpenCV on macOS:
```bash
brew install opencv
```

### Installing OpenCV on Linux:
```bash
sudo apt-get install libopencv-dev
```

## Dataset

This project uses the **Columbia-Utrecht (CUReT) Texture Database**.

The dataset contains 61 texture classes, each photographed under 205 different viewing and illumination conditions. For this project, we use 92 images per class (46 for training, 46 for testing).

**Download:** Contact your instructor or visit the CUReT database website.

**Setup:** Place all `.jpg` files in the `data/olympus/` directory.

## Building the Project
```bash
# Clean previous builds
make clean

# Compile all programs
make

# Compile individual programs
make baseline_match
make histogram_match
make histogram_match_hsv
make multi_histogram_match
make color_texture_match
```

## Usage

### Task 1: Baseline Matching
```bash
./bin/baseline_match data/olympus/pic.1016.jpg data/olympus 5
```

### Task 2: RGB Histogram Matching
```bash
./bin/histogram_match data/olympus/pic.0164.jpg data/olympus 5
```

### Task 2: HSV Histogram Matching
```bash
./bin/histogram_match_hsv data/olympus/pic.0164.jpg data/olympus 5
```

### Task 3: Multi-Histogram Matching
```bash
./bin/multi_histogram_match data/olympus/pic.0274.jpg data/olympus 5
```

### Task 4: Color + Texture Matching
```bash
./bin/color_texture_match data/olympus/pic.0535.jpg data/olympus 5
```

### General Usage Pattern
```bash
./bin/<program_name> <target_image> <image_directory> <num_matches>
```

## Results Summary

| Task | Method | Target Image | Top Matches | Accuracy |
|------|--------|--------------|-------------|----------|
| 1 | Baseline (7×7 SSD) | pic.1016.jpg | pic.0986, pic.0641, pic.0547, pic.1013 | Perfect ✓ |
| 2 | RGB Histogram | pic.0164.jpg | pic.0110, pic.1032, pic.0092 | Good |
| 2 | HSV Histogram | pic.0164.jpg | pic.0080, pic.0599, pic.0898, pic.1032 | Better ✓ |
| 3 | Multi-Histogram | pic.0274.jpg | pic.0273, pic.1031, pic.0409 | Perfect ✓ |
| 4 | Color + Texture | pic.0535.jpg | pic.0171, pic.0454, pic.0629, pic.0004 | Different from color-only |

## Key Findings

1. **Small neighborhoods are powerful:** 7×7 pixel patches contain sufficient information for texture discrimination
2. **HSV outperforms RGB:** HSV color space is more robust to illumination variations
3. **Spatial layout matters:** Multi-histogram approach captures spatial structure
4. **Texture adds discrimination:** Combining color and texture provides different matching characteristics

## Technical Implementation Details

### Feature Extraction
- All features are normalized to produce probability distributions
- Histograms use consistent binning strategies across methods
- Texture features computed using Sobel gradient operators

### Distance Metrics
- **SSD:** Sum of Squared Differences for baseline
- **Histogram Intersection:** min(h1[i], h2[i]) for histogram comparison
- **Weighted Combination:** Equal weighting for multi-feature approaches

### Performance Optimizations
- Efficient histogram computation with single-pass algorithms
- Region of Interest (ROI) extraction for spatial methods
- Reusable feature extraction functions

## Future Extensions

Possible improvements and extensions:
- Deep learning features (ResNet embeddings)
- Additional texture features (Gabor filters, LBP, Laws filters)
- Scale and rotation invariance
- GPU acceleration for large-scale retrieval
- Interactive GUI for query and visualization

## References

1. Varma & Zisserman (2003). "Texture Classification: Are Filter Banks Necessary?"
2. Shapiro & Stockman. "Computer Vision" - Chapter 8: Content-Based Image Retrieval
3. Columbia-Utrecht Reflectance and Texture Database

## License

This project is for educational purposes as part of Northeastern University coursework.