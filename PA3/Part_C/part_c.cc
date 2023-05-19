//!  CS2300 PA3 Part C: Eigen Decomposition
/*!
  \file part_c.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 03-20-2023
 */

#include <eigen3/Eigen/Dense>

#include <cmath>
#include <fstream>
#include <string>

#include "../float_compare.hpp"
#include "../get_input.hpp"
#include "../Homogeneous_Solver/homogeneous_solver.hpp"


//! A format which is used to display output vectors with 4 sig figs.
//! The number of sig figs printed in outputs.
const unsigned int kSigFig = 4;

//! A format which is used to display output vectors with 4 sig figs.
//! For reference: https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
const Eigen::IOFormat kSigFigFmt(kSigFig, 0, ", ", "\n", "[", "]");


//! Solves for a given matrix's eigenvalues.
/*!
  \param mat the matrix to find eigenvalues for
  \param eigenvalue_mat the matrix to fill with eigenvalues
  \return Whether or not the eigenvalues exist
 */
bool SolveEigenvalueMat(const Eigen::Matrix2d &mat,
                        Eigen::Matrix2d &eigenvalue_mat);

//! Solves for an eigenvector given a matrix and an eigenvalue.
/*!
  \param mat the matrix to find the eigenvector for
  \param eigenvalue the eigenvalue to solve with
  \param eigenvector the eigenvector to solver for
  \return Whether or not the eigenvector can be computed
 */
bool SolveEigenvector(const Eigen::Matrix2d &mat, const double &eigenvalue,
                      Eigen::Vector2d &eigenvector);

//! Solves for the eigenvectors of a matrix given the eigenvalues.
/*!
  \param mat the matrix to solve with
  \param eigenvalue_mat the eigenvalues to solve with
  \param eigenvector_mat the matrix containing the solution eigenvectors
  \return Whether or not the eigenvectors can be computed
 */
bool SolveEigenvectorMat(const Eigen::Matrix2d &mat,
                         const Eigen::Matrix2d &eigenvalue_mat,
                         Eigen::Matrix2d &eigenvector_mat);

//! Solves an input file and writes the solutions to an output file.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);


int main() {
    SolveFile("class_input_1.txt", "class_output_C_1.txt");
    SolveFile("class_input_2.txt", "class_output_C_2.txt");
    SolveFile("class_input_3.txt", "class_output_C_3.txt");
    SolveFile("hartt_jacob_input_i.txt", "hartt_jacob_output_C_i.txt");
    SolveFile("hartt_jacob_input_ii.txt", "hartt_jacob_output_C_ii.txt");

    return 0;
}


bool SolveEigenvalueMat(const Eigen::Matrix2d &mat,
                        Eigen::Matrix2d &eigenvalue_mat) {
    // declare a, b, and c as they would appear in a quadratic equation after 
    // the charactgeristic polynomial of mat has been solved
    double a = 1.0;
    double b = -mat(0,0) - mat(1,1);
    double c = mat.determinant();

    // calculate the quadratic determinant of the characteristic polynomial
    double quadratic_det = b*b - 4*a*c;

    // handle solutions with one eigenvalue
    if (EqualsWithinTolerance(quadratic_det, 0.0)) {
        double eigenvalue = (-b) / (2*a);

        if (EqualsWithinTolerance(eigenvalue, 0.0)) {\
            // handle solutions where the eigenvalue is zero
            return false;
        } else {
            // handle solutions with one valid eigenvalue
            eigenvalue_mat << eigenvalue,        0.0,
                                    0.0, eigenvalue;
            
            return true;
        }
    } else if (quadratic_det < 0.0) {
        // handle solution with no real eigenvalues
        return false;
    } else {
        // handle solutions with two eigenvalues
        double eigenvalue_1 = (-b + std::sqrt(quadratic_det)) / (2*a);
        double eigenvalue_2 = (-b - std::sqrt(quadratic_det)) / (2*a);
        
        // set the eigenvalue matrix with the eigenvalues in order of dominance
        if (std::fabs(eigenvalue_1) > std::fabs(eigenvalue_2)) { 
            eigenvalue_mat << eigenvalue_1,          0.0,
                                       0.0, eigenvalue_2;
        } else {
            eigenvalue_mat << eigenvalue_2,          0.0,
                                       0.0, eigenvalue_1;
        }

        return true;
    }
}

bool SolveEigenvector(const Eigen::Matrix2d &mat, const double &eigenvalue,
                      Eigen::Vector2d &eigenvector) {
    // equivalent of solving for (A-lI)r = 0
    return SolveHomogeneousSystem(mat - eigenvalue * Eigen::Matrix2d::Identity(),
                                  eigenvector);
}

bool SolveEigenvectorMat(const Eigen::Matrix2d &mat,
                         const Eigen::Matrix2d &eigenvalue_mat,
                         Eigen::Matrix2d &eigenvector_mat) {
    // solve for eigenvector 1
    Eigen::Vector2d eigenvector_1;
    if (!SolveEigenvector(mat, eigenvalue_mat(0,0), eigenvector_1)) {
        return false;
    }

    // solve for eigenvector 2
    Eigen::Vector2d eigenvector_2;
    if (!SolveEigenvector(mat, eigenvalue_mat(1,1), eigenvector_2)) {
        return false;
    }

    // assemble eigenvectors into a matrix
    eigenvector_mat << eigenvector_1, eigenvector_2;
    return true;
}

void SolveFile(const std::string &input_path, const std::string &output_path) {
    // declare & initialize IO 
    InputMatrix input_matrix = GetInputAsMatrix(input_path);
    std::ofstream output_file(output_path);

    // set up the matrix
    Eigen::Matrix2d mat;
    mat << input_matrix.col(0), input_matrix.col(1);

    // set up decomposition matrices
    Eigen::Matrix2d eigenvalue_mat;
    Eigen::Matrix2d eigenvector_mat;

    if (!SolveEigenvalueMat(mat, eigenvalue_mat)) {
        // handles situations where no real eigenvalues exist or eigenvalues
        // are zero
        output_file << "No real eigenvalues" << std::endl;
    } else if (!SolveEigenvectorMat(mat, eigenvalue_mat, eigenvector_mat)) {
        // handle situations where the eigenvectors cannot be computed
        output_file << "Cannot compute" << std::endl;
    } else {
        // find the evaluation of the eigen decomposition
        Eigen::Matrix2d product = eigenvector_mat * eigenvalue_mat *
                                  eigenvector_mat.inverse();

        // print out the decomposition matrices
        output_file << eigenvector_mat.format(kSigFigFmt) << std::endl;
        output_file << eigenvalue_mat.format(kSigFigFmt) << std::endl;
        output_file << product.format(kSigFigFmt) << std::endl;

        // test the validity of the decomposition
        if (EqualsWithinTolerance(mat, product)) {
            output_file << "1" << std::endl;
        } else {
            output_file << "0" << std::endl;
        }
    }

    output_file.close();
}
