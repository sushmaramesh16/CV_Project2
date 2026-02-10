# CV Project 2: Content-Based Image Retrieval

A comprehensive image retrieval system implementing multiple matching methods using C++ and OpenCV.

## Authors
- **Sushma Ramesh** 
- **Dina Barua** 

## Operating System & IDE
- **OS:** macOS (Apple Silicon M-series)
- **IDE:** Visual Studio Code
- **Compiler:** g++ (Homebrew) with C++17 support
- **OpenCV Version:** 4.x installed via Homebrew

## Project Overview

This project implements a content-based image retrieval (CBIR) system that can find similar images based on various visual features. The system includes multiple matching methods with progressively sophisticated feature representations, plus two texture-based extensions.

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

### Task 5: Deep Network Embeddings
- **Method:** ResNet18 pre-trained embeddings from ImageNet
- **Feature Size:** 512 dimensions (from global average pooling layer)
- **Distance Metric:** Cosine distance (default) or SSD
- **Performance:** Captures object-level and scene-level similarity

### Task 6: DNN vs Classic Features Comparison
- **Images Tested:** pic.1072.jpg, pic.0948.jpg, pic.0734.jpg
- **Finding:** DNN embeddings excel at object/scene similarity; classic features work well for color-dominated scenes

### Task 7: Custom CBIR Design
- **Method:** Combined DNN embeddings + HSV histogram + Edge direction histogram
- **Weighting:** DNN (0.55), HSV (0.30), Edge (0.15)
- **Performance:** Leverages strengths of both semantic and low-level features

## Extensions Implemented

### Extension 1: Laws Texture Energy Filters
- **Method:** 9 Laws filter combinations (L5, E5, S5)
- **Feature Size:** 521 dimensions (512 RGB + 9 Laws texture)
- **Distance Metric:** 0.5 × histogram intersection + 0.5 × normalized L2
- **Performance:** Tighter clustering (0.12-0.14) vs Sobel (0.18-0.19), captures fine-grained texture patterns

### Extension 2: Gabor Texture Energy Filters
- **Method:** 12 Gabor filters (3 scales × 4 orientations)
- **Feature Size:** 524 dimensions (512 RGB + 12 Gabor)
- **Distance Metric:** 0.5 × histogram intersection + 0.5 × normalized L2
- **Performance:** Captures oriented texture patterns at multiple scales, excellent for directional textures

## Project Structure
```
CV_PROJECT2/
├── src/
│   ├── olympus/                    # Image dataset folder
│   ├── baseline_match.cpp          # Task 1: Baseline matching
│   ├── histogram_match.cpp         # Task 2: RGB histogram
│   ├── histogram_match_hsv.cpp     # Task 2: HSV histogram
│   ├── multi_histogram_match.cpp   # Task 3: Spatial histograms
│   ├── color_texture_match.cpp     # Task 4: Color + Sobel texture
│   ├── laws_texture_match.cpp      # Extension 1: Laws filters
│   ├── gabor_texture_match.cpp     # Extension 2: Gabor filters
│   ├── task2_custom.cpp            # Task 7: Custom CBIR
│   ├── features.h/cpp              # Feature extraction functions
│   ├── distance.h/cpp              # Distance metrics
│   ├── csv_util.h/cpp              # CSV file utilities
│   └── ResNet18_olym.csv           # Pre-computed embeddings
├── bin/                            # Compiled executables
├── Makefile
└── README.md
```

## Dependencies

- **C++ Compiler:** g++ with C++17 support
- **OpenCV 4.x** - Required for Tasks 1-4, Extensions, Task 7
- **pkg-config** for OpenCV linking
- **Standard C++ libraries** - Required for Task 5

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

**Setup:** Place all `.jpg` files in the `src/olympus/` directory.

**ResNet18 Embeddings:** The `src/ResNet18_olym.csv` file contains pre-computed 512-dimensional feature vectors for each image in the dataset.

## Building the Project

### Compile All Programs
```bash
# Clean previous builds
make clean

# Compile all programs
make all

# Or compile individual programs
make baseline_match
make histogram_match
make histogram_match_hsv
make multi_histogram_match
make color_texture_match
make laws_texture_match        # Extension 1
make gabor_texture_match       # Extension 2
make task2_custom              # Task 7
```

## Usage

### Task 1: Baseline Matching
```bash
./bin/baseline_match src/olympus/pic.1016.jpg src/olympus 5
```

### Task 2: RGB Histogram Matching
```bash
./bin/histogram_match src/olympus/pic.0164.jpg src/olympus 5
```

### Task 2: HSV Histogram Matching
```bash
./bin/histogram_match_hsv src/olympus/pic.0164.jpg src/olympus 5
```

### Task 3: Multi-Histogram Matching
```bash
./bin/multi_histogram_match src/olympus/pic.0274.jpg src/olympus 5
```

### Task 4: Color + Sobel Texture Matching
```bash
./bin/color_texture_match src/olympus/pic.0535.jpg src/olympus 5
```

### Extension 1: Laws Texture Energy Filters
```bash
./bin/laws_texture_match src/olympus/pic.0535.jpg src/olympus 5
```

### Extension 2: Gabor Texture Energy Filters
```bash
./bin/gabor_texture_match src/olympus/pic.0535.jpg src/olympus 5
```

### Task 7: Custom CBIR Design
```bash
./bin/task2_custom src/olympus/pic.1062.jpg src/olympus 5
```

## Results Summary

| Task | Method | Target Image | Top Matches | Accuracy |
|------|--------|--------------|-------------|----------|
| 1 | Baseline (7×7 SSD) | pic.1016.jpg | pic.0986, pic.0641, pic.0547, pic.1013 | Perfect ✓ |
| 2 | RGB Histogram | pic.0164.jpg | pic.0110, pic.1032, pic.0092 | Good |
| 2 | HSV Histogram | pic.0164.jpg | pic.0080, pic.0599, pic.0898, pic.1032 | Better ✓ |
| 3 | Multi-Histogram | pic.0274.jpg | pic.0273, pic.1031, pic.0409 | Perfect ✓ |
| 4 | Color + Texture | pic.0535.jpg | pic.0171, pic.0454, pic.0629, pic.0004 | Different from color-only |
| Ext 1 | Laws Filters | pic.0535.jpg | pic.0285, pic.0628, pic.0731, pic.0337 | Tighter clustering |
| Ext 2 | Gabor Filters | pic.0535.jpg | pic.0605, pic.0778, pic.0805, pic.0941 | Orientation-specific |

## Key Findings

1. **Small neighborhoods are powerful:** 7×7 pixel patches contain sufficient information for texture discrimination
2. **HSV outperforms RGB:** HSV color space is more robust to illumination variations
3. **Spatial layout matters:** Multi-histogram approach captures spatial structure effectively
4. **Texture adds discrimination:** Combining color and texture provides different matching characteristics
5. **Different texture methods capture different patterns:** Laws, Sobel, and Gabor each capture complementary texture information
6. **DNN embeddings capture semantic similarity:** ResNet18 features excel at object-level matching but aren't always superior to classic features

## Technical Implementation Details

### Feature Extraction
- All features are normalized to produce probability distributions
- Histograms use consistent binning strategies across methods
- Texture features computed using Sobel, Laws, and Gabor operators

### Distance Metrics
- **SSD:** Sum of Squared Differences for baseline
- **Histogram Intersection:** min(h1[i], h2[i]) for histogram comparison
- **Cosine Distance:** 1 - cos(θ) for DNN embeddings
- **Weighted Combination:** Equal or custom weighting for multi-feature approaches

### Performance Optimizations
- Efficient histogram computation with single-pass algorithms
- Region of Interest (ROI) extraction for spatial methods
- Reusable feature extraction functions
- Pre-computed DNN embeddings stored in CSV

## Time Travel Days Used
No time travel days were used for this submission.

## GitHub Repository
https://github.com/sushmaramesh16/CV_Project2

## References

1. Varma & Zisserman (2003). "Texture Classification: Are Filter Banks Necessary?"
2. Shapiro & Stockman. "Computer Vision" - Chapter 8: Content-Based Image Retrieval
3. OpenCV Documentation for image processing and feature extraction
4. Laws, K.I. (1980). "Textured Image Segmentation"
5. Gabor, D. (1946). "Theory of Communication"

## License

This project is for educational purposes as part of Northeastern University coursework (CS 5330 - Spring 2026).