#ifndef GET_POINTS_H_
#define GET_POINTS_H_

//!  Retrieves the input from a file and returns an eigen matrix of points
/*!
  \file get_points.hpp
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 05-08-2023
 */

#include <eigen3/Eigen/Dense>

#include <array>
#include <fstream>
#include <string>
#include <vector>

//! The number of columns in the input file
const size_t kInputCols = 3;

// A points matrix with the dimensions of the data in input files
typedef std::array<Eigen::Vector3d, kInputCols> InputPointsRow;
typedef std::vector<InputPointsRow> InputPoints;


//! Formats the input file's data into a matrix of points.
/*!
  \param input_path the path of the input file
  \return A matrix containing the input file's data
 */
InputPoints GetInputAsPoints(const std::string &input_path) {
    InputPoints input_points;

    // open input_file
    std::ifstream input_file;
    input_file.open(input_path);
    
    // fill input_points
    double num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9;

    while (input_file >> num_1 >> num_2 >> num_3 >> num_4 >> num_5 >> num_6 >>
           num_7 >> num_8 >> num_9) {
        // make points from numbers
        Eigen::Vector3d point_1 = {num_1, num_2, num_3};
        Eigen::Vector3d point_2 = {num_4, num_5, num_6};
        Eigen::Vector3d point_3 = {num_7, num_8, num_9};

        // make row from points
        InputPointsRow row = {point_1, point_2, point_3};

        input_points.push_back(row);
    }

    // close input_file
    input_file.close();

    return input_points;
}

#endif // GET_POINTS_H_
