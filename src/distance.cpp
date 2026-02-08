/*
  Distance metric implementations
*/

#include "distance.h"
#include <vector>
#include <algorithm>
#include <cmath>

/*
  Calculate Sum of Squared Differences (SSD) between two feature vectors
*/
float ssd_distance(const std::vector<float> &feat1, const std::vector<float> &feat2) {
    if(feat1.size() != feat2.size()) {
        return -1.0f;
    }
    
    float ssd = 0.0f;
    for(int i = 0; i < feat1.size(); i++) {
        float diff = feat1[i] - feat2[i];
        ssd += diff * diff;
    }
    
    return ssd;
}

/*
  Calculate Histogram Intersection distance
  Intersection = sum of min(hist1[i], hist2[i])
  Returns 1 - intersection (so smaller value = more similar)
*/
float histogram_intersection_distance(const std::vector<float> &hist1, const std::vector<float> &hist2) {
    if(hist1.size() != hist2.size()) {
        return -1.0f;
    }
    
    float intersection = 0.0f;
    for(int i = 0; i < hist1.size(); i++) {
        intersection += std::min(hist1[i], hist2[i]);
    }
    
    // Return 1 - intersection so smaller distance = more similar
    // (intersection is already normalized between 0 and 1)
    return 1.0f - intersection;
}

/*
  Calculate combined color + texture distance
  First 512 bins are color (RGB histogram)
  Last 16 bins are texture (gradient magnitude histogram)
  Equal weighting: 0.5 * color_distance + 0.5 * texture_distance
*/
float color_texture_distance(const std::vector<float> &feat1, const std::vector<float> &feat2) {
    if(feat1.size() != feat2.size()) {
        return -1.0f;
    }
    
    // Expected size: 512 (color) + 16 (texture) = 528
    if(feat1.size() != 528) {
        return -1.0f;
    }
    
    // Calculate color histogram intersection (first 512 bins)
    float color_intersection = 0.0f;
    for(int i = 0; i < 512; i++) {
        color_intersection += std::min(feat1[i], feat2[i]);
    }
    float color_distance = 1.0f - color_intersection;
    
    // Calculate texture histogram intersection (last 16 bins)
    float texture_intersection = 0.0f;
    for(int i = 512; i < 528; i++) {
        texture_intersection += std::min(feat1[i], feat2[i]);
    }
    float texture_distance = 1.0f - texture_intersection;
    
    // Equal weighting
    return 0.5f * color_distance + 0.5f * texture_distance;
}