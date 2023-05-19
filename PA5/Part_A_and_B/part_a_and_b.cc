//! PA5 Part A and B: Calculating 3D projections
/*!
  \file part_a_and_b.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 05-08-2023
 */

#include <eigen3/Eigen/Dense>

#include <array>
#include <fstream>
#include <string>
#include <vector>

#include "../float_compare.hpp"
#include "../get_points.hpp"
#include "../plane.hpp"


//! A format which is used to display output vectors with 4 sig figs.
//! The number of sig figs printed in outputs.
const unsigned int kSigFig = 4;

//! A format which is used to display output vectors with 4 sig figs.
//! For reference: https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
const Eigen::IOFormat kSigFigFmt(kSigFig, 0, ", ", " ", "[", "]");

//! A zero vector for comparison
const Eigen::Vector3d kZeroVec = {0,0,0};

//! Invalid computation message
const std::string kInvalidComputationMsg = "Invalid Computation";


struct Input {
    PointNormalPlane plane_;
    Eigen::Vector3d proj_dir_;
    std::vector<InputPointsRow> points_;
}; // Input


//! Generate an input object from an input file.
/*!
  \param input_path the path of the input file
  \return An input object which contains the data of the input file
 */
Input GetInput(const std::string &input_path);

//! Determines if a parallel projection exists
/*!
  \param plane the plane being projected onto
  \param proj_dir the direction of the projection
  \return Whether or not the projection exists
 */
bool ParallelProjExists(PointNormalPlane &plane, 
                        const Eigen::Vector3d &proj_dir);

//! Determines if a perspective projection exists
/*!
  \param plane the plane being projected onto
  \param point the point to be projected
  \return Whether or not the projection exists
 */
bool PerspectiveProjExists(PointNormalPlane &plane, 
                           const Eigen::Vector3d &point);

//! Projects a point onto a plane with parallel projection
/*!
  \param plane the plane to project onto
  \param point the point to project
  \param proj_dir the direction of the projection
  \return The projected point
 */
Eigen::Vector3d ParallelProj(PointNormalPlane &plane,
                             const Eigen::Vector3d &point,
                             const Eigen::Vector3d &proj_dir);

//! Projects a point onto a plane with perspective projection
/*!
  \param plane the plane to project onto
  \param point the point to project
  \param proj_dir the direction of the projection
  \return The projected point
 */
Eigen::Vector3d PerspectiveProj(PointNormalPlane &plane,
                                const Eigen::Vector3d &point);

//! Solves a file.
/*!
  \param input_path the path of the input file
  \param output_path_a the path of the output file for part a
  \param output_path_b the path of the output file for part b
 */
void SolveFile(const std::string &input_path, const std::string &output_path_a,
               const std::string &output_path_b);

//! Solves part A
/*!
  \param input the input struct
  \param output_file the file stream to write to
 */
void SolvePartA(Input &input, std::ofstream &output_file);

//! Solves part B
/*!
  \param input the input struct
  \param output_file the file stream to write to
 */
void SolvePartB(Input &input, std::ofstream &output_file);

int main() {
    SolveFile("class_input_1-2.txt", "jhartt_output_A_1.txt",
              "jhartt_output_B_1.txt");
    SolveFile("class_input_2-2.txt", "jhartt_output_A_2.txt",
              "jhartt_output_B_2.txt");
    SolveFile("class_input_3-2.txt", "jhartt_output_A_3.txt",
              "jhartt_output_B_3.txt");
    SolveFile("jhartt_input_4-2.txt", "jhartt_output_A_4.txt",
              "jhartt_output_B_4.txt");
    SolveFile("jhartt_input_5-2.txt", "jhartt_output_A_5.txt",
              "jhartt_output_B_5.txt");

    return 0;
} // main

Input GetInput(const std::string &input_path) {
    InputPoints input_points = GetInputAsPoints(input_path);

    PointNormalPlane plane(input_points.at(0).at(1), input_points.at(0).at(0));

    Eigen::Vector3d proj_dir = input_points.at(0).at(2);

    std::vector<InputPointsRow> points;
    for (int i = 1; i < input_points.size(); i++) {
        points.push_back(input_points.at(i));
    }
    
    Input input = {plane, proj_dir, points};

    return input;
} // GetInput

bool ParallelProjExists(PointNormalPlane &plane, 
                        const Eigen::Vector3d &proj_dir) {
    return !EqualsWithinTolerance(proj_dir.dot(plane.GetNormalVec()), 0) &&
           !EqualsWithinTolerance(plane.GetNormalVec(), kZeroVec);
} // ParallelProjExists

bool PerspectiveProjExists(PointNormalPlane &plane, 
                           const Eigen::Vector3d &point) {
    return !EqualsWithinTolerance(point.dot(plane.GetNormalVec()), 0) &&
           !EqualsWithinTolerance(plane.GetNormalVec(), kZeroVec);
} // PerspectiveProjExists
    
Eigen::Vector3d ParallelProj(PointNormalPlane &plane,
                             const Eigen::Vector3d &point,
                             const Eigen::Vector3d &proj_dir) {
    Eigen::Vector3d x_prime = proj_dir;

    // calculate multiplier components
    double numerator = plane.GetNormalVec().dot(plane.GetNormalVecTail() - point);
    double denominator = proj_dir.dot(plane.GetNormalVec());

    // multiply x_prime
    x_prime *= numerator / denominator;

    // offset x_prime
    x_prime += point;

    return x_prime;
} // ParallelProj

Eigen::Vector3d PerspectiveProj(PointNormalPlane &plane,
                                const Eigen::Vector3d &point) {
    Eigen::Vector3d x_prime = point;

    // calculate multiplier components
    double numerator = plane.GetNormalVec().dot(plane.GetNormalVecTail());
    double denominator = point.dot(plane.GetNormalVec());

    // multiply x_prime
    x_prime *= numerator / denominator;

    return x_prime;
} // PerspectiveProj

void SolveFile(const std::string &input_path, const std::string &output_path_a,
               const std::string &output_path_b) {
    // initialize I/O
    Input input = GetInput(input_path);

    std::ofstream output_file_a(output_path_a);
    std::ofstream output_file_b(output_path_b);

    // solve each part
    SolvePartA(input, output_file_a);
    SolvePartB(input, output_file_b);

    // close I/O
    output_file_a.close();
    output_file_b.close();
} // SolveFile

void SolvePartA(Input &input, std::ofstream &output_file) {
    // if parallel projections exist
    if (ParallelProjExists(input.plane_, input.proj_dir_)) {
        // iterate through each point
        for (InputPointsRow row : input.points_) {
            for (Eigen::Vector3d point : row) {
                // calculate parallel projection and print it
                Eigen::Vector3d x_prime = ParallelProj(input.plane_, point,
                                                       input.proj_dir_);
                
                output_file << x_prime.transpose().format(kSigFigFmt);
            }

            output_file << '\n'; 
        }
    }
    // if parallel projections do not exist
    else {
        // iterate through each point
        for (InputPointsRow row : input.points_) {
            for (Eigen::Vector3d point : row) {
                output_file << kInvalidComputationMsg << ' '; 
            }

            output_file << '\n'; 
        }
    }
} // SolvePartA

void SolvePartB(Input &input, std::ofstream &output_file) {
    // iterate through each point
    for (InputPointsRow row : input.points_) {
        for (Eigen::Vector3d point : row) {
            // if a perspective projection exists, calculate and print it
            if (PerspectiveProjExists(input.plane_, point)) {
                Eigen::Vector3d x_prime = PerspectiveProj(input.plane_, point);
                    
                output_file << x_prime.transpose().format(kSigFigFmt);
            } 
            // print error
            else {
                output_file << kInvalidComputationMsg << ' ';
            }
        }

        output_file << '\n'; 
    }
} // SolvePartB


