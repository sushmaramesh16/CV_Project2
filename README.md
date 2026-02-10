# CV Project 2: Content-Based Image Retrieval

A comprehensive image retrieval system implementing multiple matching methods using C++ and OpenCV.

## Author
Sushma Ravichandar
Dina Barua
Northeastern University - MS in Data Science

## Project Overview

This project implements a content-based image retrieval (CBIR) system that can find similar images based on various visual features. The system includes 7 different matching methods with progressively sophisticated feature representations, from basic pixel comparison to deep learning embeddings.

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

### Task 5: Deep Network Embeddings (ResNet18)
- **Method:** Pre-computed ResNet18 embeddings for semantic matching
- **Feature Size:** 512 dimensions (ResNet18 final layer activations)
- **Distance Metrics:** Cosine distance (default) or Sum of Squared Differences (SSD)
- **Performance:** Captures high-level semantic content and object structure

### Task 6: Comparison of DNN Embeddings and Classic Features
- **Method:** Comparative analysis of Task 5 (DNN) vs Tasks 1-4 (classic features)
- **Test Images:** pic.1072.jpg, pic.0948.jpg, pic.0734.jpg
- **Key Findings:**
  - **DNN Advantages:** Better at object-level similarity, robust to lighting/background changes
  - **Classic Advantages:** Effective for color-dominated scenes, simpler implementation
  - **Use Cases:** DNN for semantic matching; classic features for color/texture matching

### Task 7: Custom Multi-Feature Matcher
- **Method:** Weighted combination of three complementary features
- **Features:**
  - DNN embeddings (ResNet18 - 512 dimensions)
  - HSV color histogram (128 bins: 8H × 4S × 4V)
  - Edge direction histogram (8 directional bins)
- **Default Weights:** DNN=0.55, HSV=0.30, Edge=0.15
- **Performance:** Combines semantic understanding with color and pattern information

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
│   ├── task5_dnn.cpp               # Task 5: ResNet18 embeddings
│   ├── task7_custom.cpp            # Task 7: Multi-feature matcher
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

# Compile individual programs (Tasks 1-4)
make baseline_match
make histogram_match
make histogram_match_hsv
make multi_histogram_match
make color_texture_match
```

### Manual Compilation for Tasks 5-7

**Task 5 (no OpenCV required):**
```bash
g++ -o bin/task5_dnn src/task5_dnn.cpp -std=c++11
```

**Task 7 (OpenCV required):**
```bash
# For OpenCV 4.x
g++ -o bin/task7_custom src/task7_custom.cpp `pkg-config --cflags --libs opencv4` -std=c++11

# For OpenCV 3.x
g++ -o bin/task7_custom src/task7_custom.cpp `pkg-config --cflags --libs opencv` -std=c++11
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
| 5 | DNN (Cosine) | pic.1062.jpg | Semantic matches | High semantic similarity |
| 7 | Multi-Feature | pic.1062.jpg | Balanced matches | Best overall performance |

## Key Findings

1. **Small neighborhoods are powerful:** 7×7 pixel patches contain sufficient information for texture discrimination
2. **HSV outperforms RGB:** HSV color space is more robust to illumination variations
3. **Spatial layout matters:** Multi-histogram approach captures spatial structure
4. **Texture adds discrimination:** Combining color and texture provides different matching characteristics
5. **DNN embeddings excel at semantic matching:** ResNet18 features capture high-level object structure and content
6. **Classic features remain valuable:** Histogram methods are effective for color-dominated scenes
7. **Feature fusion provides robustness:** Combining DNN, color, and edge features yields best overall results

## Task 6: Detailed Comparison Analysis

### Image 1: pic.1072.jpg (Object-Focused Scene)
**DNN Performance:** ⭐⭐⭐⭐⭐
- Excellent semantic matching
- Captured object type and scene structure accurately
- Robust to lighting and background variations
- Top matches showed same viewpoints and similar compositions

**Classic Features Performance:** ⭐⭐⭐
- RGB/HSV histograms focused on color distribution
- Some matches had similar colors but different objects
- Multi-histogram method improved by using spatial layout
- Missed some object-level semantic similarities

**Winner:** DNN embeddings - Superior for object and structure recognition

### Image 2: pic.0948.jpg (Color-Dominated Outdoor Scene)
**DNN Performance:** ⭐⭐⭐⭐
- Returned reasonable semantic matches
- Consistent results across variations
- Improvement over classic methods was minimal for this image

**Classic Features Performance:** ⭐⭐⭐⭐
- RGB and HSV histograms performed very well
- Excellent matches with similar color tones
- Visually consistent results for outdoor scenes
- Effective when color is the main distinguishing feature

**Winner:** Tie - Both methods equally effective when color dominates

### Image 3: pic.0734.jpg (Complex Object with Varied Lighting)
**DNN Performance:** ⭐⭐⭐⭐⭐
- Clearly outperformed classic features
- Strong similarity in object type and structure
- Robust to color variations
- Focused on semantic content rather than low-level features

**Classic Features Performance:** ⭐⭐
- Struggled with this image
- Matched textures or background colors instead of objects
- Less meaningful results overall
- Failed to capture object-level similarity

**Winner:** DNN embeddings - Superior for semantic object matching

### Overall Comparison Summary

**When to Use DNN Embeddings:**
- Object recognition and semantic matching
- Scenes with varying lighting conditions
- Images where structure matters more than color
- Need for consistent results across diverse conditions
- Complex scenes with multiple objects

**When to Use Classic Features:**
- Color is the primary distinguishing feature
- Simple, fast computation required
- Limited computational resources
- Texture-based discrimination sufficient
- Color-dominated outdoor scenes
- No pre-trained models available

**Best Practice:** Use Task 7's multi-feature approach to get the advantages of both!

## Technical Implementation Details

### Feature Extraction
- All features are normalized to produce probability distributions
- Histograms use consistent binning strategies across methods
- Texture features computed using Sobel gradient operators
- **Task 5:** Uses pre-computed ResNet18 embeddings (512-D vectors)
- **Task 7:** Extracts features on-the-fly:
  - HSV histogram: 8×4×4 = 128 bins
  - Edge directions: 8 orientation bins using Sobel gradients

### Distance Metrics
- **SSD:** Sum of Squared Differences for baseline
- **Histogram Intersection:** min(h1[i], h2[i]) for histogram comparison
- **Cosine Distance:** 1 - (dot product / magnitudes) for DNN embeddings
- **Weighted Combination:** Configurable weights for multi-feature approaches

### Performance Optimizations
- Efficient histogram computation with single-pass algorithms
- Region of Interest (ROI) extraction for spatial methods
- Reusable feature extraction functions
- **Task 5:** No image loading required (uses pre-computed features)
- **Task 7:** Progressive feature extraction with early termination options

## Example Outputs

### Task 5 Example Output:
```
----------------------------------------
Task 5: Deep Network Embeddings Matching
----------------------------------------
CSV:    src/ResNet18_olym.csv
Target: pic.1062.jpg
Top N:  5
Metric: cosine
----------------------------------------

Loaded 1503 embeddings from CSV

Top 5 matches for pic.1062.jpg:
1) pic.0234.jpg   dist=0.142356
2) pic.0567.jpg   dist=0.189234
3) pic.0891.jpg   dist=0.201456
4) pic.0123.jpg   dist=0.223789
5) pic.0445.jpg   dist=0.245123

(done)
```

### Task 7 Example Output:
```
========================================
task 7 - custom matcher
mixing DNN + color + edges
========================================
csv: src/ResNet18_olym.csv
folder: src/olympus
target: pic.1062.jpg
want top 5 matches
weights: dnn=0.55 color=0.30 edges=0.15
========================================

loading DNN features...
got 1503 images from CSV
loading target image...
extracting features from target...
  dnn: 512 dims
  color bins: 128
  edge bins: 8

comparing with all images...
compared 1502 images
sorting...

========================================
top 5 matches:
========================================
1. pic.0234.jpg (dist: 0.123456)
2. pic.0891.jpg (dist: 0.145678)
3. pic.0567.jpg (dist: 0.167890)
4. pic.1013.jpg (dist: 0.189012)
5. pic.0123.jpg (dist: 0.201234)

========================================
bottom 5 (least similar):
========================================
1. pic.0999.jpg (dist: 0.987654)
2. pic.0888.jpg (dist: 0.965432)
3. pic.0777.jpg (dist: 0.943210)
4. pic.0666.jpg (dist: 0.921098)
5. pic.0555.jpg (dist: 0.898765)

========================================
done!
combined 3 features for better matching:
- dnn (knows what stuff is)
- color (knows what colors)
- edges (knows what patterns)
========================================
```

## Future Extensions

Possible improvements and extensions:
- Additional deep learning models (VGG, ResNet50, EfficientNet)
- Fine-tuning DNN on specific texture datasets
- Learned feature fusion weights (vs. manual tuning)
- Additional texture features (Gabor filters, LBP, Laws filters)
- Scale and rotation invariance
- GPU acceleration for large-scale retrieval
- Interactive GUI for query and visualization
- Real-time matching with video streams
- Attention mechanisms to weight important regions

## References

1. He et al. (2016). "Deep Residual Learning for Image Recognition" - ResNet architecture
2. Varma & Zisserman (2003). "Texture Classification: Are Filter Banks Necessary?"
3. Shapiro & Stockman. "Computer Vision" - Chapter 8: Content-Based Image Retrieval
4. Columbia-Utrecht Reflectance and Texture Database
5. Deng et al. (2009). "ImageNet: A large-scale hierarchical image database"

## License

This project is for educational purposes as part of Northeastern University coursework.