/*
  Bruce A. Maxwell
  Utility functions for reading and writing CSV files with a specific format
*/

#ifndef CSV_UTIL_H
#define CSV_UTIL_H

#include <vector>

/*
  Given a filename, and image filename, and the image features, by
  default the function will append a line of data to the CSV format
  file.  If reset_file is true, then it will open the file in 'write'
  mode and clear the existing contents.
*/
int append_image_data_csv(char *filename, char *image_filename, std::vector<float> &image_data, int reset_file = 0);

/*
  Given a file with the format of a string as the first column and
  floating point numbers as the remaining columns, this function
  returns the filenames as a std::vector of character arrays, and the
  remaining data as a 2D std::vector<float>.
*/
int read_image_data_csv(char *filename, std::vector<char *> &filenames, std::vector<std::vector<float>> &data, int echo_file = 0);

#endif