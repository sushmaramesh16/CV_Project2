/*
  Name: Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Extension - Color and Laws texture energy filter matching
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
  Custom distance for color + Laws texture features
  First 512 values are color histogram
  Last 9 values are Laws texture energy
  Equal weighting: 0.5 color + 0.5 texture
*/
float color_laws_distance(const std::vector<float> &feat1, const std::vector<float> &feat2) {
    if(feat1.size() != 521 || feat2.size() != 521) {
        return -1.0f;
    }
    
    // Color histogram intersection (first 512 bins)
    float color_intersection = 0.0f;
    for(int i = 0; i < 512; i++) {
        color_intersection += std::min(feat1[i], feat2[i]);
    }
    float color_distance = 1.0f - color_intersection;
    
    // Laws texture: use Euclidean distance (last 9 values)
    float texture_distance = 0.0f;
    for(int i = 512; i < 521; i++) {
        float diff = feat1[i] - feat2[i];
        texture_distance += diff * diff;
    }
    texture_distance = std::sqrt(texture_distance);
    
    // Normalize texture distance to [0,1] range (max possible is sqrt(9) = 3)
    texture_distance /= 3.0f;
    
    // Equal weighting
    return 0.5f * color_distance + 0.5f * texture_distance;
}

int main(int argc, char *argv[]) {
    // Check arguments
    if(argc < 4) {
        printf("Usage: %s <target_image> <image_directory> <num_matches>\n", argv[0]);
        printf("Example: ./laws_texture_match data/olympus/pic.0535.jpg data/olympus 5\n");
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
    
    // Extract color + Laws texture features from target
    std::vector<float> target_features;
    color_laws_texture_feature(target, target_features);
    
    printf("Target image: %s\n", target_filename);
    printf("Feature vector size: %lu (512 color + 9 Laws texture)\n", target_features.size());
    
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
            
            // Extract color + Laws texture features
            std::vector<float> features;
            color_laws_texture_feature(img, features);
            
            // Calculate combined distance
            float distance = color_laws_distance(target_features, features);
            
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
    printf("\nTop %d matches (Color + Laws Texture):\n", num_matches);
    for(int i = 0; i < num_matches && i < matches.size(); i++) {
        printf("%d. %s (distance: %.6f)\n", i+1, matches[i].filename.c_str(), matches[i].distance);
    }
    
    return 0;
}