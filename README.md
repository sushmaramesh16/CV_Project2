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
│   ├── olympus/                    # Image dataset folder
│   ├── baseline_match.cpp          # Task 1: Baseline matching
│   ├── histogram_match.cpp         # Task 2: RGB histogram
│   ├── histogram_match_hsv.cpp     # Task 2: HSV histogram
│   ├── multi_histogram_match.cpp   # Task 3: Spatial histograms
│   ├── color_texture_match.cpp     # Task 4: Color + texture
│   ├── features.h/cpp              # Feature extraction functions
│   ├── distance.h/cpp              # Distance metrics
│   ├── csv_util.h/cpp              # CSV file utilities
│   └── ResNet18_olym.csv           # Pre-computed embeddings
├── bin/                            # Compiled executables
├── features/                       # Saved feature files
├── results/                        # Output results
├── Makefile
└── README.md
```

## Dependencies

- **C++ Compiler:** g++ with C++11 support
- **OpenCV 4.x** (or OpenCV 3.x with minor modifications) - Required for Tasks 1-4, 7
- **pkg-config** for OpenCV linking - Required for Tasks 1-4, 7
- **Standard C++ libraries** - Required for Task 5 (no OpenCV needed)

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

**Setup:** Place all `.jpg` files in the `src/olympus/` directory.

**ResNet18 Embeddings:** The `src/ResNet18_olym.csv` file contains pre-computed 512-dimensional feature vectors for each image in the dataset.

## Building the Project

### Compile All Programs
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

### Task 4: Color + Sobel Texture Matching
```bash
./bin/color_texture_match data/olympus/pic.0535.jpg data/olympus 5
```

### Extension: Color + Laws Texture Matching
```bash
./bin/laws_texture_match data/olympus/pic.0535.jpg data/olympus 5
```

### Task 5: DNN Embeddings (Dina)
```bash
./bin/task5_dnn data/olympus/pic.0893.jpg embeddings.csv 5
```

### Task 7: Custom CBIR (Dina)
```bash
./bin/task7_custom data/olympus/pic.1062.jpg embeddings.csv data/olympus 5
```

### Task 5: Deep Network Embeddings

**Using cosine distance (default, recommended):**
```bash
./bin/task5_dnn src/ResNet18_olym.csv pic.1062.jpg 5
```

**Using cosine distance (explicit):**
```bash
./bin/task5_dnn src/ResNet18_olym.csv pic.1062.jpg 5 cosine
```

**Using SSD distance:**
```bash
./bin/task5_dnn src/ResNet18_olym.csv pic.1062.jpg 5 ssd
```

**Testing with Task 6 comparison images:**
```bash
./bin/task5_dnn src/ResNet18_olym.csv pic.1072.jpg 5 cosine
./bin/task5_dnn src/ResNet18_olym.csv pic.0948.jpg 5 cosine
./bin/task5_dnn src/ResNet18_olym.csv pic.0734.jpg 5 cosine
```

**⚠️ Important:** Task 5 only needs the CSV file and image name - do NOT include the image folder path!

**Parameters:**
- `<csv_file>`: Path to CSV file with ResNet18 embeddings (e.g., `src/ResNet18_olym.csv`)
- `<target_image_name>`: Filename of target image (must exist in CSV) - just the filename, like `pic.1062.jpg`
- `<N>`: Number of top matches to return
- `[metric]`: Optional distance metric (`cosine` or `ssd`, default: `cosine`)

### Task 7: Custom Multi-Feature Matcher

**Using default weights (DNN=0.55, HSV=0.30, Edge=0.15):**
```bash
./bin/task7_custom src/ResNet18_olym.csv src/olympus pic.1062.jpg 5
```

**Using custom weights:**
```bash
./bin/task7_custom src/ResNet18_olym.csv src/olympus pic.1062.jpg 5 0.6 0.25 0.15
```

**Testing with Task 6 comparison images:**
```bash
./bin/task7_custom src/ResNet18_olym.csv src/olympus pic.1072.jpg 5
./bin/task7_custom src/ResNet18_olym.csv src/olympus pic.0948.jpg 5
./bin/task7_custom src/ResNet18_olym.csv src/olympus pic.0734.jpg 5
```

**Parameters:**
- `<csv_file>`: Path to CSV with ResNet18 embeddings (e.g., `src/ResNet18_olym.csv`)
- `<image_folder>`: Directory containing actual image files (e.g., `src/olympus`)
- `<target_image>`: Target image filename (e.g., `pic.1062.jpg`)
- `<N>`: Number of top matches to return
- `[wDNN]`: Optional weight for DNN features (default: 0.55)
- `[wHSV]`: Optional weight for HSV histogram (default: 0.30)
- `[wEDGE]`: Optional weight for edge histogram (default: 0.15)

### General Usage Pattern
```bash
# Tasks 1-4: Image-based matching
./bin/<program_name> <target_image> <image_directory> <num_matches>

# Task 5: CSV-based DNN matching (no image folder needed)
./bin/task5_dnn <csv_file> <target_image_name> <num_matches> [metric]

# Task 7: Combined CSV + image-based matching
./bin/task7_custom <csv_file> <image_folder> <target_image> <num_matches> [weights]
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

This project is for educational purposes as part of Northeastern University coursework (CS 5330 - Spring 2026).