//!  CS2300 PA3 Part B: Triangular Areas and Transformation
/*!
  \file part_b.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 03-20-2023
 */

#include <eigen3/Eigen/Dense>

#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>

#include "../float_compare.hpp"
#include "../get_input.hpp"


//! A format which is used to display output vectors with 4 sig figs.
//! The number of sig figs printed in outputs.
const unsigned int kSigFig = 4;

//! A format which is used to display output vectors with 4 sig figs.
//! For reference: https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
const Eigen::IOFormat kSigFigFmt(kSigFig, 0, ", ", "\n", "[", "]");


//! Solves an input file and writes the solutions to an output file.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);

//! Solves part A of part B by writing the areas of triangles to a file.
/*!
  \param v_mat the matrix of the original triangle
  \param v_prime_mat the matrix of the transformed triangle
  \param output_file the output file to write the areas to
 */
void SolvePartA(const Eigen::Matrix2d &v_mat,
                const Eigen::Matrix2d &v_prime_mat, std::ofstream &output_file);

//! Solves part B of part B by computing the map that transforms V into V'
/*!
  \param v_mat the matrix of the original triangle
  \param v_prime_mat the matrix of the transformed triangle
  \param output_file the output file to write the map to
 */
void SolvePartB(const Eigen::Matrix2d &v_mat,
                const Eigen::Matrix2d &v_prime_mat, std::ofstream &output_file);

//! Calculates the area of a triangle given its edge matrix.
/*!
  \param triangle_mat the edge matrix of the triangle
  \return The area of the triangle
 */
double TriangleArea(const Eigen::Matrix2d &triangle_mat);


int main() {
    SolveFile("class_input_1.txt", "class_output_B_1.txt");
    SolveFile("class_input_2.txt", "class_output_B_2.txt");
    SolveFile("class_input_3.txt", "class_output_B_3.txt");
    SolveFile("hartt_jacob_input_i.txt", "hartt_jacob_output_B_i.txt");
    SolveFile("hartt_jacob_input_ii.txt", "hartt_jacob_output_B_ii.txt");

    return 0;
}


void SolveFile(const std::string &input_path, const std::string &output_path) {
    // declare & initialize IO 
    InputMatrix input_matrix = GetInputAsMatrix(input_path);
    std::ofstream output_file(output_path);
    output_file << std::setprecision(kSigFig);

    // set up the V matrix
    Eigen::Vector2d v_2 = input_matrix.col(1) - input_matrix.col(0);
    Eigen::Vector2d v_3 = input_matrix.col(2) - input_matrix.col(0);
    Eigen::Matrix2d v_mat;
    v_mat << v_2, v_3;

    // set up the V matrix
    Eigen::Vector2d v_prime_2 = input_matrix.col(4) - input_matrix.col(3);
    Eigen::Vector2d v_prime_3 = input_matrix.col(5) - input_matrix.col(3);
    Eigen::Matrix2d v_prime_mat;
    v_prime_mat << v_prime_2, v_prime_3;

    // solve each part of part of the program
    SolvePartA(v_mat, v_prime_mat, output_file);
    SolvePartB(v_mat, v_prime_mat, output_file);

    output_file.close();
}

void SolvePartA(const Eigen::Matrix2d &v_mat,
                const Eigen::Matrix2d &v_prime_mat, std::ofstream &output_file) {
    // write areas into output_file
    output_file << TriangleArea(v_mat) << ' ' << TriangleArea(v_prime_mat)
            << std::endl;
}

void SolvePartB(const Eigen::Matrix2d &v_mat,
                const Eigen::Matrix2d &v_prime_mat, std::ofstream &output_file) {
    if (EqualsWithinTolerance(v_mat.determinant(), 0.0)) {
        // handle triangles that cannot be transformed
        output_file << "Cannot compute" << std::endl;
    } else {
        // handle triangles that can be transformed
        Eigen::Matrix2d map = v_prime_mat * v_mat.inverse();

        output_file << map.format(kSigFigFmt) << std::endl;
    }
}

double TriangleArea(const Eigen::Matrix2d &triangle_mat) {
    // utilize the determinant to find the area of a parallelogram, then half it
    return 0.5 * std::fabs(triangle_mat.determinant());
}
