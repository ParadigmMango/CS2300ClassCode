#ifndef GET_INPUT_H_
#define GET_INPUT_H_

//!  Retrieves the input from a file and returns an eigen matrix.
/*!
  \file get_input.hpp
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 03-20-2023
 */

#include <eigen3/Eigen/Dense>

#include <fstream>
#include <string>

//! The number of rows in the input file
const size_t kInputRows = 2;
//! The number of columns in the input file
const size_t kInputCols = 6;

// A double matrix with the dimensions of the data in input files
typedef Eigen::Matrix<double, kInputRows, kInputCols> InputMatrix;


//! Formats the input file's data into an Eigen matrix.
/*!
  \param input_path the path of the input file
  \return An Eigen matrix containing the input file's data
 */
InputMatrix GetInputAsMatrix(const std::string &input_path) {
    InputMatrix input_matrix;

    // open input_file
    std::ifstream input_file;
    input_file.open(input_path);
    
    // fill input_matrix
    for (size_t row = 0; row < kInputRows; row++) {
        for (size_t col = 0; col < kInputCols; col++) {
            input_file >> input_matrix(row, col);
        }
    }

    // close input_file
    input_file.close();

    return input_matrix;
}

#endif
