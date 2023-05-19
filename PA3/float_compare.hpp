#ifndef FLOAT_COMPARE_H_
#define FLOAT_COMPARE_H_

//!  Handles floating point errors
/*!
  \file float_compare.hpp
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 03-20-2023
 */

#include <eigen3/Eigen/Dense>

//! The tolerance used.
const double kTolerance = 0.00000000000001;

//! Compares doubles in a manner similar to equals but with a tolerance.
/*!
  \param num_1 the first number to be compared
  \param num_2 the second number to be compared
  \return Whether or not the doubles can be considered "equal"
 */
bool EqualsWithinTolerance(const double &num_1, const double &num_2) {
    double difference;

    // calculate difference and ensure the result is positive
    if (num_1 > num_2) {
        difference = num_1 - num_2;
    } else {
        difference = num_2 - num_1;
    }

    return difference < kTolerance;
}

//! Compares arrays with a tolerance to check for equality.
/*!
  \param mat_1 the first matrix to be compared
  \param mat_2 the second matrix to be compared
  \return Whether or not the matrices can be considered "equal"
 */
bool EqualsWithinTolerance(const Eigen::Matrix2d &mat_1,
                           const Eigen::Matrix2d &mat_2) {
    // compare each element of the array
    return EqualsWithinTolerance(mat_1(0,0), mat_2(0,0)) && 
           EqualsWithinTolerance(mat_1(0,1), mat_2(0,1)) && 
           EqualsWithinTolerance(mat_1(1,0), mat_2(1,0)) && 
           EqualsWithinTolerance(mat_1(1,1), mat_2(1,1));
}

#endif
