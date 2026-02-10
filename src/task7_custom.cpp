/*
  Task 7 - My Custom Image Matcher
  Name(s):Dina Barua and Sushma Ramesh
  Date: 02.07.2026

  Description:Task 7 implements a custom image retrieval system that
  combines deep network embeddings with color (HSV) and edge direction features
  to better match images based on both appearance and structure.
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;

// just stores image name and how far away it is from target
struct Match {
  string name;
  float dist;
};

// helper function to keep values in range
float clampf(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// compares two DNN feature vectors
// basically just checks angle between them
// closer angle = more similar
float cosineDistance(const vector<float>& a, const vector<float>& b) {
  if (a.size() != b.size()) return 2.0f;

  float dot = 0.0f;
  float magA = 0.0f;
  float magB = 0.0f;

  // the math
  for (size_t i = 0; i < a.size(); i++) {
    dot += a[i] * b[i];
    magA += a[i] * a[i];
    magB += b[i] * b[i];
  }

  magA = sqrt(magA);
  magB = sqrt(magB);

  if (magA == 0.0f || magB == 0.0f) return 2.0f;

  float cosSim = dot / (magA * magB);
  cosSim = clampf(cosSim, -1.0f, 1.0f);

  return 1.0f - cosSim;
}

// compares two histograms
// just sees how much they overlap
float histIntersectionDist(const vector<float>& h1, const vector<float>& h2) {
  if (h1.size() != h2.size()) return 1.0f;

  float overlap = 0.0f;
  for (size_t i = 0; i < h1.size(); i++) {
    overlap += min(h1[i], h2[i]);
  }

  return 1.0f - overlap;
}

// makes histogram add up to 1
void normalizeHist(vector<float>& h) {
  float sum = 0.0f;
  for (float v : h) sum += v;
  if (sum <= 0.0f) return;
  for (float &v : h) v /= sum;
}

// reads the CSV with DNN features
map<string, vector<float>> readResNetCSV(const char* csvPath) {
  map<string, vector<float>> db;

  FILE* f = fopen(csvPath, "r");
  if (!f) {
    printf("uh oh can't open %s\n", csvPath);
    return db;
  }

  char line[40000];
  int count = 0;

  while (fgets(line, sizeof(line), f)) {
    char* fname = strtok(line, ",");
    if (!fname) continue;

    vector<float> feat;
    feat.reserve(512);

    char* tok = NULL;
    while ((tok = strtok(NULL, ",")) != NULL) {
      feat.push_back((float)atof(tok));
    }

    if (feat.size() == 512) {
      db[string(fname)] = feat;
      count++;
    }
  }

  fclose(f);
  printf("got %d images from CSV\n", count);
  return db;
}

// makes a color histogram using HSV
// HSV is better than RGB cuz it handles lighting better
vector<float> computeHSVHist128(const cv::Mat& bgr) {
  vector<float> h(128, 0.0f);

  cv::Mat hsv;
  cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);

  // go thru every pixel and count colors
  for (int y = 0; y < hsv.rows; y++) {
    const cv::Vec3b* row = hsv.ptr<cv::Vec3b>(y);
    for (int x = 0; x < hsv.cols; x++) {
      int H = row[x][0];
      int S = row[x][1];
      int V = row[x][2];

      // figure out which bin
      int hBin = (H * 8) / 180;
      int sBin = (S * 4) / 256;
      int vBin = (V * 4) / 256;

      hBin = min(hBin, 7);
      sBin = min(sBin, 3);
      vBin = min(vBin, 3);

      int idx = hBin * 16 + sBin * 4 + vBin;
      h[idx] += 1.0f;
    }
  }

  normalizeHist(h);
  return h;
}

// makes histogram of edge directions
// tells us if pic has mostly vertical lines, horizontal, diagonal, etc
vector<float> computeEdgeDirHist8(const cv::Mat& bgr) {
  vector<float> h(8, 0.0f);

  cv::Mat gray;
  cv::cvtColor(bgr, gray, cv::COLOR_BGR2GRAY);

  // use sobel to find edges
  cv::Mat gx, gy;
  cv::Sobel(gray, gx, CV_32F, 1, 0, 3);
  cv::Sobel(gray, gy, CV_32F, 0, 1, 3);

  for (int y = 0; y < gray.rows; y++) {
    const float* gxRow = gx.ptr<float>(y);
    const float* gyRow = gy.ptr<float>(y);

    for (int x = 0; x < gray.cols; x++) {
      float dx = gxRow[x];
      float dy = gyRow[x];

      // skip if edge is too weak
      float mag = fabs(dx) + fabs(dy);
      if (mag < 20.0f) continue;

      // get angle
      float ang = atan2(dy, dx);
      if (ang < 0) ang += 2.0f * (float)M_PI;

      // put in bin
      int bin = (int)(ang / (2.0f * (float)M_PI) * 8.0f);
      if (bin < 0) bin = 0;
      if (bin > 7) bin = 7;

      h[bin] += 1.0f;
    }
  }

  normalizeHist(h);
  return h;
}

int main(int argc, char* argv[]) {
  
  if (argc < 5) {
    printf("\nusage:\n");
    printf("  %s <csv> <folder> <target> <N> [wDNN wHSV wEDGE]\n", argv[0]);
    printf("\nexample:\n");
    printf("  %s embeddings.csv olympus pic.1062.jpg 5\n\n", argv[0]);
    return -1;
  }

  const char* csvFile = argv[1];
  const char* imgFolder = argv[2];
  string targetName = argv[3];
  int N = atoi(argv[4]);

  if (N <= 0) {
    printf("N needs to be positive\n");
    return -1;
  }

  // how much weight to give each feature
  // DNN gets more cuz it's usually the best
  float wDNN  = 0.55f;
  float wHSV  = 0.30f;
  float wEDGE = 0.15f;

  // let user change weights if they want
  if (argc >= 8) {
    wDNN  = (float)atof(argv[5]);
    wHSV  = (float)atof(argv[6]);
    wEDGE = (float)atof(argv[7]);
  }

  printf("\n========================================\n");
  printf("task 7 - custom matcher\n");
  printf("mixing DNN + color + edges\n");
  printf("========================================\n");
  printf("csv: %s\n", csvFile);
  printf("folder: %s\n", imgFolder);
  printf("target: %s\n", targetName.c_str());
  printf("want top %d matches\n", N);
  printf("weights: dnn=%.2f color=%.2f edges=%.2f\n", wDNN, wHSV, wEDGE);
  printf("========================================\n\n");

  // load the DNN stuff
  printf("loading DNN features...\n");
  map<string, vector<float>> dnnDB = readResNetCSV(csvFile);
  if (dnnDB.empty()) {
    printf("no embeddings loaded, something went wrong\n");
    return -1;
  }

  // check target exists
  if (dnnDB.find(targetName) == dnnDB.end()) {
    printf("can't find %s in the CSV\n", targetName.c_str());
    return -1;
  }

  // load target image
  printf("loading target image...\n");
  string targetPath = string(imgFolder) + "/" + targetName;
  cv::Mat targetImg = cv::imread(targetPath);
  if (targetImg.empty()) {
    printf("can't read %s\n", targetPath.c_str());
    return -1;
  }

  // get all features from target
  printf("extracting features from target...\n");
  const vector<float>& targetDNN = dnnDB[targetName];
  vector<float> targetHSV  = computeHSVHist128(targetImg);
  vector<float> targetEdge = computeEdgeDirHist8(targetImg);
  
  printf("  dnn: %lu dims\n", targetDNN.size());
  printf("  color bins: %lu\n", targetHSV.size());
  printf("  edge bins: %lu\n", targetEdge.size());

  // now compare with every other image
  printf("\ncomparing with all images...\n");
  vector<Match> results;
  results.reserve((int)dnnDB.size());

  int compared = 0;
  for (const auto& it : dnnDB) {
    const string& name = it.first;

    // doesn't compare with itself
    if (name == targetName) continue;

    // load image
    string path = string(imgFolder) + "/" + name;
    cv::Mat img = cv::imread(path);
    if (img.empty()) continue;

    // get features
    const vector<float>& dnn = it.second;
    vector<float> hsv  = computeHSVHist128(img);
    vector<float> edge = computeEdgeDirHist8(img);

    // calculate each distance
    float dDNN  = cosineDistance(targetDNN, dnn);
    float dHSV  = histIntersectionDist(targetHSV, hsv);
    float dEDGE = histIntersectionDist(targetEdge, edge);

    // mix them together
    float totalDist = wDNN * dDNN + wHSV * dHSV + wEDGE * dEDGE;

    Match m;
    m.name = name;
    m.dist = totalDist;
    results.push_back(m);
    
    compared++;
  }

  printf("compared %d images\n", compared);

  if (results.empty()) {
    printf("no results, check your paths\n");
    return -1;
  }

  // sort by distance
  printf("sorting...\n");
  sort(results.begin(), results.end(),
       [](const Match& a, const Match& b) { return a.dist < b.dist; });

  // show top matches
  int topK = min(N, (int)results.size());
  printf("\n========================================\n");
  printf("top %d matches:\n", topK);
  printf("========================================\n");
  for (int i = 0; i < topK; i++) {
    printf("%d. %s (dist: %.6f)\n", 
           i + 1, results[i].name.c_str(), results[i].dist);
  }

  // show worst matches
  printf("\n========================================\n");
  printf("bottom %d (least similar):\n", topK);
  printf("========================================\n");
  for (int i = 0; i < topK; i++) {
    int idx = (int)results.size() - 1 - i;
    printf("%d. %s (dist: %.6f)\n", 
           i + 1, results[idx].name.c_str(), results[idx].dist);
  }

  printf("\n========================================\n");
  printf("done!\n");
  printf("combined 3 features for better matching:\n");
  printf("- dnn (knows what stuff is)\n");
  printf("- color (knows what colors)\n");
  printf("- edges (knows what patterns)\n");
  printf("========================================\n\n");

  return 0;
}