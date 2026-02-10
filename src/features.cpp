/*
  Name: Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Implementation of feature extraction functions including baseline, histograms, and texture features
*/
#include "features.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

/*
  Extract 7x7 baseline feature from center of image
  Uses RGB values directly (3 channels x 49 pixels = 147 features)
*/
int baseline_feature(cv::Mat &src, std::vector<float> &feature) {
    // Clear the feature vector
    feature.clear();
    
    // Get image dimensions
    int rows = src.rows;
    int cols = src.cols;
    
    // Calculate center position
    int center_row = rows / 2;
    int center_col = cols / 2;
    
    // Extract 7x7 square from center (from -3 to +3 around center)
    // Store in row-major order: all channels for each pixel
    for(int i = -3; i <= 3; i++) {
        for(int j = -3; j <= 3; j++) {
            int row = center_row + i;
            int col = center_col + j;
            
            // OpenCV stores as BGR
            cv::Vec3b pixel = src.at<cv::Vec3b>(row, col);
            
            // Store as B, G, R for each pixel
            feature.push_back((float)pixel[0]); // Blue
            feature.push_back((float)pixel[1]); // Green
            feature.push_back((float)pixel[2]); // Red
        }
    }
    
    return 0;
}

/*
  Compute 3D RGB color histogram
  Uses 8 bins per channel (8x8x8 = 512 total bins)
  Returns normalized histogram
*/
int histogram_feature(cv::Mat &src, std::vector<float> &feature) {
    // Clear feature vector
    feature.clear();
    
    // Initialize histogram with 512 bins (8x8x8), all zeros
    int bins_per_channel = 8;
    int total_bins = bins_per_channel * bins_per_channel * bins_per_channel;
    std::vector<int> histogram(total_bins, 0);
    
    // Count pixels in each bin
    int total_pixels = 0;
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(i, j);
            
            // Map pixel values [0-255] to bin indices [0-7]
            int b_bin = pixel[0] / 32;  // Blue:  0-255 -> 0-7
            int g_bin = pixel[1] / 32;  // Green: 0-255 -> 0-7
            int r_bin = pixel[2] / 32;  // Red:   0-255 -> 0-7
            
            // Compute 1D index from 3D bins
            int bin_index = r_bin * bins_per_channel * bins_per_channel + 
                           g_bin * bins_per_channel + 
                           b_bin;
            
            histogram[bin_index]++;
            total_pixels++;
        }
    }
    
    // Normalize histogram by total pixel count
    for(int i = 0; i < total_bins; i++) {
        feature.push_back((float)histogram[i] / (float)total_pixels);
    }
    
    return 0;
}

/*
  Compute HSV color histogram
  Uses 8 bins for Hue, 4 bins for Saturation, 4 bins for Value (8x4x4 = 128 bins)
  Returns normalized histogram
*/
int histogram_feature_hsv(cv::Mat &src, std::vector<float> &feature) {
    // Clear feature vector
    feature.clear();
    
    // Convert BGR to HSV
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    
    // Initialize histogram with 128 bins (8x4x4), all zeros
    int h_bins = 8;   // Hue: 0-179 -> 8 bins
    int s_bins = 4;   // Saturation: 0-255 -> 4 bins
    int v_bins = 4;   // Value: 0-255 -> 4 bins
    int total_bins = h_bins * s_bins * v_bins;
    std::vector<int> histogram(total_bins, 0);
    
    // Count pixels in each bin
    int total_pixels = 0;
    for(int i = 0; i < hsv.rows; i++) {
        for(int j = 0; j < hsv.cols; j++) {
            cv::Vec3b pixel = hsv.at<cv::Vec3b>(i, j);
            
            // Map HSV values to bin indices
            // OpenCV: H is 0-179, S is 0-255, V is 0-255
            int h_bin = pixel[0] / 23;  // Hue: 0-179 -> 0-7 (179/23 ≈ 7.8)
            int s_bin = pixel[1] / 64;  // Saturation: 0-255 -> 0-3
            int v_bin = pixel[2] / 64;  // Value: 0-255 -> 0-3
            
            // Clamp to valid range (just in case)
            if(h_bin >= h_bins) h_bin = h_bins - 1;
            if(s_bin >= s_bins) s_bin = s_bins - 1;
            if(v_bin >= v_bins) v_bin = v_bins - 1;
            
            // Compute 1D index from 3D bins
            int bin_index = h_bin * s_bins * v_bins + 
                           s_bin * v_bins + 
                           v_bin;
            
            histogram[bin_index]++;
            total_pixels++;
        }
    }
    
    // Normalize histogram by total pixel count
    for(int i = 0; i < total_bins; i++) {
        feature.push_back((float)histogram[i] / (float)total_pixels);
    }
    
    return 0;
}

/*
  Helper function to compute histogram for a region of interest (ROI)
*/
void compute_histogram_roi(cv::Mat &src, std::vector<float> &histogram) {
    int bins_per_channel = 8;
    int total_bins = bins_per_channel * bins_per_channel * bins_per_channel;
    std::vector<int> hist(total_bins, 0);
    
    // Count pixels in each bin
    int total_pixels = 0;
    for(int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            cv::Vec3b pixel = src.at<cv::Vec3b>(i, j);
            
            // Map pixel values [0-255] to bin indices [0-7]
            int b_bin = pixel[0] / 32;
            int g_bin = pixel[1] / 32;
            int r_bin = pixel[2] / 32;
            
            // Compute 1D index from 3D bins
            int bin_index = r_bin * bins_per_channel * bins_per_channel + 
                           g_bin * bins_per_channel + 
                           b_bin;
            
            hist[bin_index]++;
            total_pixels++;
        }
    }
    
    // Normalize and append to feature vector
    for(int i = 0; i < total_bins; i++) {
        histogram.push_back((float)hist[i] / (float)total_pixels);
    }
}

/*
  Compute multi-resolution histogram feature
  Computes histogram for top half and bottom half of image
  Concatenates both histograms (2 x 512 = 1024 features)
*/
int multi_histogram_feature(cv::Mat &src, std::vector<float> &feature) {
    // Clear feature vector
    feature.clear();
    
    int mid_row = src.rows / 2;
    
    // Extract top half
    cv::Mat top_half = src(cv::Rect(0, 0, src.cols, mid_row));
    compute_histogram_roi(top_half, feature);
    
    // Extract bottom half
    cv::Mat bottom_half = src(cv::Rect(0, mid_row, src.cols, src.rows - mid_row));
    compute_histogram_roi(bottom_half, feature);
    
    return 0;
}

/*
  Compute gradient magnitude histogram using Sobel filters
  Uses 16 bins for gradient magnitude (0-255)
  Returns normalized histogram
*/
int gradient_magnitude_histogram(cv::Mat &src, std::vector<float> &feature) {
    // Clear feature vector
    feature.clear();
    
    // Convert to grayscale
    cv::Mat gray;
    if(src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Compute Sobel gradients
    cv::Mat grad_x, grad_y;
    cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3);  // dx
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3);  // dy
    
    // Compute gradient magnitude
    cv::Mat abs_grad_x, abs_grad_y;
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    
    cv::Mat magnitude;
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, magnitude);
    
    // Build histogram with 16 bins
    int num_bins = 16;
    std::vector<int> histogram(num_bins, 0);
    
    int total_pixels = 0;
    for(int i = 0; i < magnitude.rows; i++) {
        for(int j = 0; j < magnitude.cols; j++) {
            int mag_value = magnitude.at<uchar>(i, j);
            int bin_index = mag_value / 16;  // 0-255 -> 0-15
            if(bin_index >= num_bins) bin_index = num_bins - 1;
            
            histogram[bin_index]++;
            total_pixels++;
        }
    }
    
    // Normalize
    for(int i = 0; i < num_bins; i++) {
        feature.push_back((float)histogram[i] / (float)total_pixels);
    }
    
    return 0;
}

/*
  Compute combined color + texture histogram feature
  Concatenates RGB histogram (512 bins) + gradient magnitude histogram (16 bins)
  Total: 528 features
*/
int color_texture_feature(cv::Mat &src, std::vector<float> &feature) {
    // Clear feature vector
    feature.clear();
    
    // Compute RGB color histogram (512 bins)
    std::vector<float> color_hist;
    histogram_feature(src, color_hist);
    
    // Compute gradient magnitude histogram (16 bins)
    std::vector<float> texture_hist;
    gradient_magnitude_histogram(src, texture_hist);
    
    // Concatenate: color + texture
    feature.insert(feature.end(), color_hist.begin(), color_hist.end());
    feature.insert(feature.end(), texture_hist.begin(), texture_hist.end());
    
    return 0;
}

/*
  Apply 1D convolution (horizontal or vertical)
  FIXED VERSION - handles both uchar and float Mat types
*/
void convolve1D(cv::Mat &src, cv::Mat &dst, const std::vector<int> &kernel, bool horizontal) {
    int ksize = kernel.size();
    int offset = ksize / 2;
    
    dst = cv::Mat::zeros(src.size(), CV_32F);
    
    for(int i = offset; i < src.rows - offset; i++) {
        for(int j = offset; j < src.cols - offset; j++) {
            float sum = 0.0f;
            
            if(horizontal) {
                for(int k = 0; k < ksize; k++) {
                    // FIXED: Check Mat type before accessing
                    if(src.type() == CV_32F) {
                        sum += src.at<float>(i, j - offset + k) * kernel[k];
                    } else {
                        sum += (float)src.at<uchar>(i, j - offset + k) * kernel[k];
                    }
                }
            } else {
                for(int k = 0; k < ksize; k++) {
                    // FIXED: Check Mat type before accessing
                    if(src.type() == CV_32F) {
                        sum += src.at<float>(i - offset + k, j) * kernel[k];
                    } else {
                        sum += (float)src.at<uchar>(i - offset + k, j) * kernel[k];
                    }
                }
            }
            
            dst.at<float>(i, j) = sum;
        }
    }
}

/*
  Compute Laws texture energy features (OPTIMIZED VERSION)
  Uses 9 texture energy measures from Laws filters
  Returns a 9-dimensional feature vector
*/
int laws_texture_feature(cv::Mat &src, std::vector<float> &feature) {
    feature.clear();
    
    // Convert to grayscale
    cv::Mat gray;
    if(src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Convert to float for filtering
    cv::Mat gray_float;
    gray.convertTo(gray_float, CV_32F);
    
    // Define 1D Laws kernels
    float L5_data[] = {1, 4, 6, 4, 1};
    float E5_data[] = {-1, -2, 0, 2, 1};
    float S5_data[] = {-1, 0, 2, 0, -1};
    
    cv::Mat L5(1, 5, CV_32F, L5_data);
    cv::Mat E5(1, 5, CV_32F, E5_data);
    cv::Mat S5(1, 5, CV_32F, S5_data);
    
    std::vector<cv::Mat> kernels_h = {L5, E5, S5};
    std::vector<cv::Mat> kernels_v = {L5.t(), E5.t(), S5.t()};  // Transpose for vertical
    
    // Apply all 9 filter combinations using separable filters
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            cv::Mat temp, result;
            
            // Apply horizontal filter
            cv::filter2D(gray_float, temp, CV_32F, kernels_h[j]);
            
            // Apply vertical filter
            cv::filter2D(temp, result, CV_32F, kernels_v[i]);
            
            // Compute energy (mean absolute value)
            cv::Mat abs_result;
            abs_result = cv::abs(result);
            cv::Scalar mean_energy = cv::mean(abs_result);
            
            feature.push_back(mean_energy[0]);
        }
    }
    
    // Normalize features by dividing by the sum
    float sum = 0.0f;
    for(float val : feature) {
        sum += val;
    }
    
    if(sum > 0) {
        for(int i = 0; i < feature.size(); i++) {
            feature[i] /= sum;
        }
    }
    
    return 0;
}

/*
  Compute combined color + Laws texture feature
  Concatenates RGB histogram (512 bins) + Laws texture (9 values)
  Total: 521 features
*/
int color_laws_texture_feature(cv::Mat &src, std::vector<float> &feature) {
    feature.clear();
    
    // Compute RGB color histogram (512 bins)
    std::vector<float> color_hist;
    histogram_feature(src, color_hist);
    
    // Compute Laws texture features (9 values)
    std::vector<float> laws_features;
    laws_texture_feature(src, laws_features);
    
    // Concatenate: color + texture
    feature.insert(feature.end(), color_hist.begin(), color_hist.end());
    feature.insert(feature.end(), laws_features.begin(), laws_features.end());
    
    return 0;
}

/**
 * Compute Gabor texture features
 * Returns 12-dimensional feature vector: 3 scales × 4 orientations
 */
std::vector<float> computeGaborFeatures(const cv::Mat& src) {
    std::vector<float> features;
    cv::Mat gray;
    
    // Convert to grayscale
    if (src.channels() == 3) {
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = src.clone();
    }
    
    // Normalize to float
    gray.convertTo(gray, CV_32F, 1.0/255.0);
    
    // Gabor parameters
    int ksize = 21;          // Kernel size
    double sigma = 5.0;      // Gaussian envelope std dev
    double gamma = 0.5;      // Aspect ratio
    
    // 3 scales (wavelengths)
    std::vector<double> lambdas = {4.0, 8.0, 16.0};
    
    // 4 orientations
    std::vector<double> thetas = {0, CV_PI/4, CV_PI/2, 3*CV_PI/4};
    
    // Compute Gabor responses for each scale and orientation
    for (double lambda : lambdas) {
        for (double theta : thetas) {
            // Get Gabor kernel
            cv::Mat kernel = cv::getGaborKernel(
                cv::Size(ksize, ksize), 
                sigma, 
                theta, 
                lambda, 
                gamma, 
                0,           // psi (phase offset)
                CV_32F
            );
            
            // Apply filter
            cv::Mat filtered;
            cv::filter2D(gray, filtered, CV_32F, kernel);
            
            // Compute mean absolute response as feature
            cv::Scalar meanVal = cv::mean(cv::abs(filtered));
            features.push_back(meanVal[0]);
        }
    }
    
    return features;  // 12-dimensional vector
}

/**
 * Compute combined Color + Gabor features
 * Returns: RGB histogram (512) + Gabor (12) = 524 dimensions
 */
std::vector<float> computeColorGaborFeatures(const cv::Mat& src) {
    std::vector<float> combined;
    
    // 1. RGB Histogram (512 bins: 8×8×8)
    std::vector<float> rgbHist;
    histogram_feature(const_cast<cv::Mat&>(src), rgbHist);
    combined.insert(combined.end(), rgbHist.begin(), rgbHist.end());
    
    // 2. Gabor texture features (12 features)
    std::vector<float> gaborFeats = computeGaborFeatures(src);
    combined.insert(combined.end(), gaborFeats.begin(), gaborFeats.end());
    
    return combined;  // 524-dimensional
}

/**
 * Distance metric for Color+Gabor features
 * Uses histogram intersection for color, normalized L2 for Gabor
 */
float colorGaborDistance(const std::vector<float>& f1, const std::vector<float>& f2) {
    // Split features
    std::vector<float> color1(f1.begin(), f1.begin() + 512);
    std::vector<float> color2(f2.begin(), f2.begin() + 512);
    std::vector<float> gabor1(f1.begin() + 512, f1.end());
    std::vector<float> gabor2(f2.begin() + 512, f2.end());
    
    // Color distance: histogram intersection
    float intersection = 0.0;
    for (size_t i = 0; i < 512; i++) {
        intersection += std::min(color1[i], color2[i]);
    }
    float colorDist = 1.0 - intersection;
    
    // Gabor distance: normalized Euclidean
    float gaborDist = 0.0;
    for (size_t i = 0; i < gabor1.size(); i++) {
        float diff = gabor1[i] - gabor2[i];
        gaborDist += diff * diff;
    }
    gaborDist = std::sqrt(gaborDist) / std::sqrt(12.0);  // Normalize by dimension
    
    // Weighted combination (equal weights)
    return 0.5 * colorDist + 0.5 * gaborDist;
}