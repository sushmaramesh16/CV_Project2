/*
  Distance metrics for image matching
*/

#ifndef DISTANCE_H
#define DISTANCE_H

#include <vector>

/*
  Calculate Sum of Squared Differences (SSD) between two feature vectors
*/
float ssd_distance(const std::vector<float> &feat1, const std::vector<float> &feat2);

/*
  Calculate Histogram Intersection distance between two normalized histograms
  Returns 1 - intersection (so smaller = more similar)
*/
float histogram_intersection_distance(const std::vector<float> &hist1, const std::vector<float> &hist2);

/*
  Calculate combined color + texture distance
  First 512 bins are color (use histogram intersection)
  Last 16 bins are texture (use histogram intersection)
  Equal weighting: 0.5 * color_distance + 0.5 * texture_distance
*/
float color_texture_distance(const std::vector<float> &feat1, const std::vector<float> &feat2);

#endif