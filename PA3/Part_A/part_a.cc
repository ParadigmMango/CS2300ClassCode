//!  CS2300 PA3 Part A: Solving Linear Systems
/*!
  \file part_a.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 03-20-2023
 */

#include <eigen3/Eigen/Dense>

#include <fstream>
#include <string>

#include "../float_compare.hpp"
#include "../get_input.hpp"
#include "../Homogeneous_Solver/homogeneous_solver.hpp"


//! The number of sig figs printed in outputs.
const unsigned int kSigFig = 4;

//! A format which is used to display output vectors with 4 sig figs.
//! For reference: https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
const Eigen::IOFormat kSigFigFmt(kSigFig, 0, ", ", "\n", "[", "]");


//! Determines where or not two row vectors are colinear.
/*!
  \param vec_1 the first vector to compare
  \param vec_2 the second vector to compare
  \return Whether or not the vectors are colinear
 */
bool AreColinear(const Eigen::RowVector2d &vec_1,
                 const Eigen::RowVector2d &vec_2);

//! Solves an input file and writes the solutions to an output file.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);

//! Solves part A of part A by solving a linear system.
/*!
  \param mat the matrix of the linear system
  \param result the result vector of the linear system
  \param output_file the output file to write the result to
 */
void SolvePartA(const Eigen::Matrix2d &mat, const Eigen::Vector2d &result,
                std::ofstream &output_file);

//! Solves part B of part A by solving a homogeneous system
/*!
  \param mat the matrix of the homogeneous system
  \param output_file the output file to write the result to.
 */
void SolvePartB(const Eigen::Matrix2d &mat, std::ofstream &output_file);


int main() {
    SolveFile("class_input_1.txt", "class_output_A_1.txt");
    SolveFile("class_input_2.txt", "class_output_A_2.txt");
    SolveFile("class_input_3.txt", "class_output_A_3.txt");
    SolveFile("hartt_jacob_input_i.txt", "hartt_jacob_output_A_i.txt");
    SolveFile("hartt_jacob_input_ii.txt", "hartt_jacob_output_A_ii.txt");

    return 0;
}


bool AreColinear(const Eigen::RowVector2d &vec_1,
                 const Eigen::RowVector2d &vec_2) {
    // below the vector dot product theorm with theta = 0 or 180 degrees is used
    double dot_product = vec_1.dot(vec_2);
    double magnitude_product = vec_1.norm() * vec_2.norm();

    return EqualsWithinTolerance(dot_product, magnitude_product) ||
           EqualsWithinTolerance(-dot_product, magnitude_product);
}

void SolveFile(const std::string &input_path, const std::string &output_path) {
    // declare & initialize IO 
    InputMatrix input_matrix = GetInputAsMatrix(input_path);
    std::ofstream output_file(output_path);

    // set up the system's matrix
    Eigen::Matrix2d mat;
    mat << input_matrix.col(0), input_matrix.col(1);

    // set up the system's result vector
    Eigen::Vector2d result = input_matrix.col(2);

    // solve the parts by calling their methods
    SolvePartA(mat, result, output_file);
    SolvePartB(mat, output_file);

    output_file.close();
}

void SolvePartA(const Eigen::Matrix2d &mat, const Eigen::Vector2d &result,
                std::ofstream &output_file) {
    if (AreColinear(mat.row(0), mat.row(1))) {
        if (AreColinear(mat.row(0), result.transpose())) {
            // handle underdetermined systems
            output_file << "System underdetermined" << std::endl;
        } else {
            // handle inconsistent systems
            output_file << "System inconsistent" << std::endl;
        }
    } else {
        // handle valid systems
        Eigen::Vector2d solution = mat.inverse() * result;
        output_file << solution.transpose().format(kSigFigFmt) << std::endl;
    }
}

void SolvePartB(const Eigen::Matrix2d &mat, std::ofstream &output_file) {
    // declare solution ahead of SolveHomogeneousSystem
    Eigen::Vector2d solution;

    // set is trivial to the oppositie of whether or not SolveHomogeneousSystem
    // succeeds
    bool is_trivial = !SolveHomogeneousSystem(mat, solution);

    if (is_trivial) {
        // handle trivial soltutions
        output_file << "Only trivial solution exists" << std::endl;
    } else {
        // handle non-trivial solutions
        output_file << solution.transpose().format(kSigFigFmt);
    }
}
