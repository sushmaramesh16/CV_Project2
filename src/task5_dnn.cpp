/*
Project 2 - Task 5: Deep Network Embeddings (ResNet18)
Name(s):Dina Barua and Sushma Ramesh
Date: 02.07.2026

Description: Task 5 uses precomputed ResNet18 feature vectors from a CSV file and 
compares images using cosine distance to find the most visually similar images.
*/

#include <cstdio>    // printf, fopen, fgets
#include <cstdlib>   // atoi, atof, malloc, free
#include <cstring>   // strcmp, strcpy, strtok
#include <cmath>     // sqrt
#include <vector>    // vector
#include <map>       // map
#include <string>    // string
#include <algorithm> // (not required here, but okay to have)

using namespace std;

// A tiny struct to store one match result (filename + distance)
struct ImageMatch {
  char name[256];  // storing filename as C-string (simple)
  float dist;      // distance score (smaller = more similar)
};

// ----------------------------
// Distance Functions
// ----------------------------

// SSD: sum of squared differences
// This is basically: (a1-b1)^2 + (a2-b2)^2 + ...
float ssdDistance(const vector<float>& a, const vector<float>& b) {
  // if they don't match sizes, something is wrong, so return huge distance
    if (a.size() != b.size()) return 999999.0f;

    float sum = 0.0f;

  // loop through all 512 values
    for (size_t i = 0; i < a.size(); i++) {
    float diff = a[i] - b[i];
    sum += diff * diff;
    }

    return sum;
}

// Cosine distance: 1 - cosine similarity
// cosine similarity = (a dot b) / (||a|| * ||b||)
float cosineDistance(const vector<float>& a, const vector<float>& b) {
    if (a.size() != b.size()) return 2.0f;

  float dot = 0.0f;   // a dot b
  float magA = 0.0f;  // ||a||^2 (we'll sqrt later)
  float magB = 0.0f;  // ||b||^2 (we'll sqrt later)

    for (size_t i = 0; i < a.size(); i++) {
    dot += a[i] * b[i];
    magA += a[i] * a[i];
    magB += b[i] * b[i];
    }

  // convert squared magnitude into magnitude
    magA = sqrt(magA);
    magB = sqrt(magB);

  // avoid dividing by zero (just in case)
    if (magA == 0.0f || magB == 0.0f) return 2.0f;

  float cosSim = dot / (magA * magB);

  // sometimes floating point gives like 1.0000001, so clamp
    if (cosSim > 1.0f) cosSim = 1.0f;
    if (cosSim < -1.0f) cosSim = -1.0f;

  // distance should be smaller for more similar vectors
    return 1.0f - cosSim;
}

// ----------------------------
// Sorting helper for qsort
// ----------------------------

// qsort needs a compare function like this
int compareMatches(const void* a, const void* b) {
    const ImageMatch* m1 = (const ImageMatch*)a;
    const ImageMatch* m2 = (const ImageMatch*)b;

    if (m1->dist < m2->dist) return -1;
    if (m1->dist > m2->dist) return 1;
    return 0;
}

// ----------------------------
// CSV Reading
// ----------------------------

// Reads the embeddings CSV into a map:
//   db["pic.0001.jpg"] = vector<float>(512 values)
map<string, vector<float>> readCSV(const char* filepath) {
    map<string, vector<float>> db;

  // open file
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
    printf("Error: cannot open CSV file: %s\n", filepath);
    return db;
    }

  // each line is pretty long (filename + 512 floats)
  // so we make a big buffer
    char line[40000];

    int count = 0;

  // read line by line
    while (fgets(line, sizeof(line), file) != NULL) {
    // 1) first token = filename
    char* fname = strtok(line, ",");
    if (fname == NULL) continue;

    // 2) remaining tokens = float values
    vector<float> feat;
    feat.reserve(512); // makes it a little faster + avoids resizing

    char* token = NULL;
    while ((token = strtok(NULL, ",")) != NULL) {
        feat.push_back((float)atof(token));
    }

    // sometimes bad lines happen, so we only keep correct rows
    if (feat.size() == 512) {
        db[string(fname)] = feat;
        count++;
    } else {
      // if you want to debug weird CSV problems, you can print this:
      // printf("Skipping %s because it has %lu values\n", fname, feat.size());
    }
    }

    fclose(file);

    printf("Loaded %d embeddings from CSV\n", count);
    return db;
}

// ----------------------------
// Main
// ----------------------------

int main(int argc, char* argv[]) {
  // Expect:
  // argv[1] = csv file
  // argv[2] = target filename
  // argv[3] = N
  // argv[4] = metric (optional)

    if (argc < 4) {
    printf("\nUsage: %s <csv_file> <target_image_name> <N> [cosine|ssd]\n", argv[0]);
    printf("Example: %s ResNet18_olym.csv pic.0893.jpg 3 cosine\n\n", argv[0]);
    return -1;
    }

    const char* csvFile = argv[1];
    string targetName = argv[2];
    int N = atoi(argv[3]);
    const char* metric = (argc >= 5) ? argv[4] : "cosine";

  // basic checks
    if (N <= 0) {
    printf("Error: N must be > 0\n");
    return -1;
    }

  // decide metric
    bool useSSD = false;
    if (strcmp(metric, "ssd") == 0) useSSD = true;
    else if (strcmp(metric, "cosine") == 0) useSSD = false;
    else {
    printf("Error: metric must be 'cosine' or 'ssd'\n");
    return -1;
    }

  // print a small header so it's obvious what we ran
    printf("\n----------------------------------------\n");
    printf("Task 5: Deep Network Embeddings Matching\n");
    printf("----------------------------------------\n");
    printf("CSV:    %s\n", csvFile);
    printf("Target: %s\n", targetName.c_str());
    printf("Top N:  %d\n", N);
    printf("Metric: %s\n", metric);
    printf("----------------------------------------\n\n");

  // load the embeddings database from CSV
    map<string, vector<float>> db = readCSV(csvFile);

  // if CSV reading failed, db will be empty
    if (db.empty()) {
    printf("Error: database is empty (CSV load failed?)\n");
    return -1;
    }

  // target MUST exist in the CSV (Task 5 says we get target from file too)
    if (db.find(targetName) == db.end()) {
    printf("Error: target image not found in CSV: %s\n", targetName.c_str());
    return -1;
    }

  // get target embedding vector (512 floats)
    const vector<float>& targetVec = db[targetName];

  // we will compute distances to every OTHER image
  // so at most db.size() - 1 results
    int maxResults = (int)db.size() - 1;
    if (maxResults < 1) {
    printf("Error: not enough images in the database\n");
    return -1;
    }

  // allocate an array for matches
  // (using malloc here just because it's simple and matches C style)
  ImageMatch* matches = (ImageMatch*)malloc(maxResults * sizeof(ImageMatch));
    if (matches == NULL) {
    printf("Error: malloc failed\n");
    return -1;
    }

  // compute distances
    int idx = 0;

    for (const auto& entry : db) {
    const string& name = entry.first;

    // IMPORTANT:
    // We do NOT want the target image to show up as a "match"
    // because it will always have distance 0
    if (name == targetName) {
        continue;
    }

    const vector<float>& vec = entry.second;

    float d = 0.0f;
    if (useSSD) d = ssdDistance(targetVec, vec);
    else d = cosineDistance(targetVec, vec);

    // store filename and distance
    strcpy(matches[idx].name, name.c_str());
    matches[idx].dist = d;
    idx++;
    }

  // sort by smallest distance first
    qsort(matches, idx, sizeof(ImageMatch), compareMatches);

  // if N is bigger than how many we have, just print what we have
    int k = (N < idx) ? N : idx;

  // print the top matches
    printf("Top %d matches for %s:\n", k, targetName.c_str());
    for (int i = 0; i < k; i++) {
    printf("%d) %s   dist=%.6f\n", i + 1, matches[i].name, matches[i].dist);
    }

  // clean up
    free(matches);

    printf("\n(done)\n\n");
    return 0;
}
