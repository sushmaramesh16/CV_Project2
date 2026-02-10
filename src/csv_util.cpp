/*
  Name: Bruce A. Maxwell (original), Modified by Sushma Ramesh, Dina Barua
  Date: February 9, 2026
  Purpose: Implementation of CSV file utilities for storing and reading feature vectors
*/

#include <cstdio>
#include <cstring>
#include <vector>
#include "csv_util.h"

/*
  reads a string from a CSV file. the 0-terminated string is returned in the char array os.
  The function returns false if it is successfully read. It returns true if it reaches the end of the line or the file.
 */
int getstring(FILE *fp, char os[]) {
  int p = 0;
  int eol = 0;
  
  for(;;) {
    char ch = fgetc(fp);
    if(ch == ',') {
      break;
    }
    else if(ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }
    os[p] = ch;
    p++;
  }
  os[p] = '\0';

  return(eol);
}

int getint(FILE *fp, int *v) {
  char s[256];
  int p = 0;
  int eol = 0;

  for(;;) {
    char ch = fgetc(fp);
    if(ch == ',') {
      break;
    }
    else if(ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }
      
    s[p] = ch;
    p++;
  }
  s[p] = '\0';
  *v = atoi(s);

  return(eol);
}

/*
  Utility function for reading one float value from a CSV file
 */
int getfloat(FILE *fp, float *v) {
  char s[256];
  int p = 0;
  int eol = 0;

  for(;;) {
    char ch = fgetc(fp);
    if(ch == ',') {
      break;
    }
    else if(ch == '\n' || ch == EOF) {
      eol = 1;
      break;
    }
      
    s[p] = ch;
    p++;
  }
  s[p] = '\0';
  *v = atof(s);

  return(eol);
}

/*
  Append a line of data to the CSV file
 */
int append_image_data_csv(char *filename, char *image_filename, std::vector<float> &image_data, int reset_file) {
  char buffer[256];
  char mode[8];
  FILE *fp;

  strcpy(mode, "a");

  if(reset_file) {
    strcpy(mode, "w");
  }
  
  fp = fopen(filename, mode);
  if(!fp) {
    printf("Unable to open output file %s\n", filename);
    exit(-1);
  }

  strcpy(buffer, image_filename);
  std::fwrite(buffer, sizeof(char), strlen(buffer), fp);
  for(int i=0; i<image_data.size(); i++) {
    char tmp[256];
    sprintf(tmp, ",%.4f", image_data[i]);
    std::fwrite(tmp, sizeof(char), strlen(tmp), fp);
  }
      
  std::fwrite("\n", sizeof(char), 1, fp);

  fclose(fp);
  
  return(0);
}

/*
  Read image data from CSV file
 */
int read_image_data_csv(char *filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int echo_file) {
  FILE *fp;
  float fval;
  char img_file[256];

  fp = fopen(filename, "r");
  if(!fp) {
    printf("Unable to open feature file\n");
    return(-1);
  }

  printf("Reading %s\n", filename);
  for(;;) {
    std::vector<float> dvec;
    
    if(getstring(fp, img_file)) {
      break;
    }

    for(;;) {
      float eol = getfloat(fp, &fval);
      dvec.push_back(fval);
      if(eol) break;
    }

    data.push_back(dvec);

    char *fname = new char[strlen(img_file)+1];
    strcpy(fname, img_file);
    filenames.push_back(fname);
  }
  fclose(fp);
  printf("Finished reading CSV file\n");

  if(echo_file) {
    for(int i=0; i<data.size(); i++) {
      for(int j=0; j<data[i].size(); j++) {
        printf("%.4f  ", data[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }

  return(0);
}