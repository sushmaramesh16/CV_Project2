/*
  Name: Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Task 1 - Baseline image matching using 7x7 center square and SSD distance
*/

#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include "features.h"
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

// Calculate Sum of Squared Differences between two feature vectors
float calculate_ssd(const std::vector<float> &feat1, const std::vector<float> &feat2) {
    if(feat1.size() != feat2.size()) {
        printf("Error: Feature vectors have different sizes!\n");
        return -1.0f;
    }
    
    float ssd = 0.0f;
    for(int i = 0; i < feat1.size(); i++) {
        float diff = feat1[i] - feat2[i];
        ssd += diff * diff;
    }
    
    return ssd;
}

int main(int argc, char *argv[]) {
    // Check arguments
    if(argc < 4) {
        printf("Usage: %s <target_image> <image_directory> <num_matches>\n", argv[0]);
        printf("Example: ./baseline_match data/olympus/pic.1016.jpg data/olympus 5\n");
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
    
    // Extract features from target
    std::vector<float> target_features;
    baseline_feature(target, target_features);
    
    printf("Target image: %s\n", target_filename);
    printf("Feature vector size: %lu\n", target_features.size());
    
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
            
            // Extract features
            std::vector<float> features;
            baseline_feature(img, features);
            
            // Calculate distance
            float distance = calculate_ssd(target_features, features);
            
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
        printf("%d. %s (distance: %.2f)\n", i+1, matches[i].filename.c_str(), matches[i].distance);
    }
    
    return 0;
}