/*
  Name: Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Gabor texture matching program - Extension 2
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include "features.h"
#include "distance.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] 
                  << " <target_image> <database_dir> <N>" << std::endl;
        std::cout << "Example: ./gabor_texture_match src/olympus/pic.0535.jpg src/olympus 5" << std::endl;
        return -1;
    }
    
    std::string targetFile = argv[1];
    std::string dbDir = argv[2];
    int N = std::atoi(argv[3]);
    
    // Read target image
    cv::Mat target = cv::imread(targetFile);
    if (target.empty()) {
        std::cerr << "Error: Cannot read target image " << targetFile << std::endl;
        return -1;
    }
    
    std::cout << "Computing Gabor features for target image: " << targetFile << std::endl;
    
    // Compute target features
    std::vector<float> targetFeats = computeColorGaborFeatures(target);
    std::cout << "Target feature vector size: " << targetFeats.size() << " dimensions" << std::endl;
    
    // Store results: (filename, distance)
    std::vector<std::pair<std::string, float>> results;
    
    // Process all images in database
    int count = 0;
    for (const auto& entry : fs::directory_iterator(dbDir)) {
        if (entry.path().extension() == ".jpg" || 
            entry.path().extension() == ".jpeg") {
            
            std::string imgPath = entry.path().string();
            cv::Mat img = cv::imread(imgPath);
            
            if (!img.empty()) {
                std::vector<float> imgFeats = computeColorGaborFeatures(img);
                float dist = colorGaborDistance(targetFeats, imgFeats);
                results.push_back({imgPath, dist});
                count++;
            }
        }
    }
    
    std::cout << "Processed " << count << " images from database" << std::endl;
    
    // Sort by distance (ascending)
    std::sort(results.begin(), results.end(),
              [](const auto& a, const auto& b) { 
                  return a.second < b.second; 
              });
    
    // Print top N results
    std::cout << "\nTop " << N << " matches using Gabor texture features:" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    for (int i = 0; i < N && i < results.size(); i++) {
        // Extract just the filename for cleaner output
        fs::path p(results[i].first);
        std::cout << (i+1) << ". " << p.filename().string() 
                  << " (distance: " << results[i].second << ")" << std::endl;
    }
    
    return 0;
}