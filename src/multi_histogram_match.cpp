/*
  Name: Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Task 3 - Multi-histogram matching using spatial layout (top/bottom halves)
*/

#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include "features.h"
#include "distance.h"
#include "csv_util.h"

// Structure to hold image filename and its distance to target
struct ImageMatch {
    std::string filename;
    float distance;
    
    // For sorting
    bool operator<(const ImageMatch &other) const {
        return distance < other.distance;
    }
};

/*
  Custom distance for multi-histogram features
  Computes histogram intersection for each region separately
  Then combines with equal weighting
*/
float multi_histogram_distance(const std::vector<float> &feat1, const std::vector<float> &feat2) {
    if(feat1.size() != feat2.size()) {
        return -1.0f;
    }
    
    // Each histogram is 512 bins
    int bins_per_histogram = 512;
    int num_histograms = feat1.size() / bins_per_histogram;
    
    float total_distance = 0.0f;
    
    // Compute distance for each histogram separately
    for(int h = 0; h < num_histograms; h++) {
        int start_idx = h * bins_per_histogram;
        
        // Compute histogram intersection for this region
        float intersection = 0.0f;
        for(int i = 0; i < bins_per_histogram; i++) {
            intersection += std::min(feat1[start_idx + i], feat2[start_idx + i]);
        }
        
        // Convert to distance (1 - intersection)
        float distance = 1.0f - intersection;
        
        // Equal weighting for all regions
        total_distance += distance;
    }
    
    // Average the distances
    return total_distance / num_histograms;
}

int main(int argc, char *argv[]) {
    // Check arguments
    if(argc < 4) {
        printf("Usage: %s <target_image> <image_directory> <num_matches>\n", argv[0]);
        printf("Example: ./multi_histogram_match data/olympus/pic.0274.jpg data/olympus 5\n");
        return -1;
    }
    
    char *target_filename = argv[1];
    char *directory = argv[2];
    int num_matches = atoi(argv[3]);
    
    // Read target image
    cv::Mat target = cv::imread(target_filename);
    if(target.empty()) {
        printf("Error: Cannot read target image %s\n", target_filename);
        return -1;
    }
    
    // Extract multi-histogram features from target
    std::vector<float> target_features;
    multi_histogram_feature(target, target_features);
    
    printf("Target image: %s\n", target_filename);
    printf("Feature vector size: %lu (multi-histogram: top + bottom)\n", target_features.size());
    
    // Open directory
    DIR *dirp = opendir(directory);
    if(dirp == NULL) {
        printf("Cannot open directory %s\n", directory);
        return -1;
    }
    
    // Store all matches
    std::vector<ImageMatch> matches;
    
    // Loop through all images in directory
    struct dirent *dp;
    while((dp = readdir(dirp)) != NULL) {
        // Check if it's an image file
        if(strstr(dp->d_name, ".jpg") || 
           strstr(dp->d_name, ".png") || 
           strstr(dp->d_name, ".JPG") ||
           strstr(dp->d_name, ".PNG")) {
            
            // Build full path
            char filepath[256];
            strcpy(filepath, directory);
            strcat(filepath, "/");
            strcat(filepath, dp->d_name);
            
            // Read image
            cv::Mat img = cv::imread(filepath);
            if(img.empty()) {
                continue;
            }
            
            // Extract multi-histogram features
            std::vector<float> features;
            multi_histogram_feature(img, features);
            
            // Calculate custom multi-histogram distance
            float distance = multi_histogram_distance(target_features, features);
            
            // Store match
            ImageMatch match;
            match.filename = std::string(dp->d_name);
            match.distance = distance;
            matches.push_back(match);
        }
    }
    closedir(dirp);
    
    // Sort matches by distance
    std::sort(matches.begin(), matches.end());
    
    // Print top N matches
    printf("\nTop %d matches:\n", num_matches);
    for(int i = 0; i < num_matches && i < matches.size(); i++) {
        printf("%d. %s (distance: %.6f)\n", i+1, matches[i].filename.c_str(), matches[i].distance);
    }
    
    return 0;
}