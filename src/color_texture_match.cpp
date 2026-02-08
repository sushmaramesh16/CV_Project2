/*
  Color + Texture histogram image matching
  Combines RGB color histogram + Sobel gradient magnitude histogram
  
  Usage: ./color_texture_match <target_image> <image_directory> <num_matches>
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

int main(int argc, char *argv[]) {
    // Check arguments
    if(argc < 4) {
        printf("Usage: %s <target_image> <image_directory> <num_matches>\n", argv[0]);
        printf("Example: ./color_texture_match data/olympus/pic.0535.jpg data/olympus 5\n");
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
    
    // Extract color + texture features from target
    std::vector<float> target_features;
    color_texture_feature(target, target_features);
    
    printf("Target image: %s\n", target_filename);
    printf("Feature vector size: %lu (512 color + 16 texture)\n", target_features.size());
    
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
            
            // Extract color + texture features
            std::vector<float> features;
            color_texture_feature(img, features);
            
            // Calculate combined distance
            float distance = color_texture_distance(target_features, features);
            
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
    printf("\nTop %d matches (Color + Texture):\n", num_matches);
    for(int i = 0; i < num_matches && i < matches.size(); i++) {
        printf("%d. %s (distance: %.6f)\n", i+1, matches[i].filename.c_str(), matches[i].distance);
    }
    
    return 0;
}