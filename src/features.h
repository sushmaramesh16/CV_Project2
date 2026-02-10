/*
  Name: Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Header file declaring feature extraction function prototypes for CBIR system
*/

#ifndef FEATURES_H
#define FEATURES_H

#include <opencv2/opencv.hpp>
#include <vector>

/*
  Extract 7x7 baseline feature from center of image
  Returns a 49-element feature vector (7x7 pixels flattened)
*/
int baseline_feature(cv::Mat &src, std::vector<float> &feature);

/*
  Compute color histogram for entire image
  Uses 3D RGB histogram with 8 bins per channel (8x8x8 = 512 bins)
  Returns a 512-element feature vector
*/
int histogram_feature(cv::Mat &src, std::vector<float> &feature);

/*
  Compute HSV color histogram
  Uses 8 bins for Hue, 4 bins for Saturation, 4 bins for Value (8x4x4 = 128 bins)
  Returns normalized histogram
*/
int histogram_feature_hsv(cv::Mat &src, std::vector<float> &feature);

/*
  Compute multi-resolution histogram feature
  Computes histogram for top half and bottom half of image
  Concatenates both histograms (2 x 512 = 1024 features)
*/
int multi_histogram_feature(cv::Mat &src, std::vector<float> &feature);

/*
  Compute gradient magnitude histogram using Sobel filters
  Uses 16 bins for gradient magnitude
  Returns normalized histogram
*/
int gradient_magnitude_histogram(cv::Mat &src, std::vector<float> &feature);

/*
  Compute combined color + texture histogram feature
  Concatenates RGB histogram (512 bins) + gradient magnitude histogram (16 bins)
  Total: 528 features
*/
int color_texture_feature(cv::Mat &src, std::vector<float> &feature);
/*
  Compute Laws texture energy features
  Uses 9 texture energy measures from Laws filters
  Returns a 9-dimensional feature vector
*/
int laws_texture_feature(cv::Mat &src, std::vector<float> &feature);

/*
  Compute combined color + Laws texture feature
  Concatenates RGB histogram (512 bins) + Laws texture (9 values)
  Total: 521 features
*/
int color_laws_texture_feature(cv::Mat &src, std::vector<float> &feature);
#endif