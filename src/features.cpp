/*
  Feature extraction implementations
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