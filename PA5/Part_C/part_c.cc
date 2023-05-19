//! PA5 Part C: Finding closest point and it's distance to a plane
/*!
  \file part_c.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 05-08-2023
 */

#include <eigen3/Eigen/Dense>

#include <array>
#include <fstream>
#include <iomanip>
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

//! Invalid computation message
const std::string kInvalidComputationMsg = "Invalid Computation";


//! Generates a 3D bisector plane given two points.
/*!
  \param point_1 the first point
  \param point_2 the second point
  \return A 3D point-normal form plane that bisects the two points
 */
PointNormalPlane GenerateBisectorPlane(const Eigen::Vector3d &point_1,
                                       const Eigen::Vector3d &point_2);

//! Solves a file.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);

//! Solves a row of the input file and prints the solutions
/*!
  \param row the row to solve 
  \param output_file the output file to write the solutions to
 */
void SolveRow(const InputPointsRow &row, std::ofstream &output_file);


int main() {
    SolveFile("class_input_1-2.txt", "jhartt_output_C_1.txt");
    SolveFile("class_input_2-2.txt", "jhartt_output_C_2.txt");
    SolveFile("class_input_3-2.txt", "jhartt_output_C_3.txt");
    SolveFile("jhartt_input_4-2.txt", "jhartt_output_C_4.txt");
    SolveFile("jhartt_input_5-2.txt", "jhartt_output_C_5.txt");

    return 0;
} // main

PointNormalPlane GenerateBisectorPlane(const Eigen::Vector3d &point_1,
                                       const Eigen::Vector3d &point_2) {
    // Generate a point on the plane and the normal
    Eigen::Vector3d midpoint = (point_1 + point_2) / 2.0;
    Eigen::Vector3d normal_vector = point_2 - point_1;

    PointNormalPlane ret_plane(normal_vector, midpoint);
    
    return ret_plane;
} // GenerateBisectorPlane

void SolveFile(const std::string &input_path, const std::string &output_path) {
    // initialize I/O
    InputPoints input = GetInputAsPoints(input_path);

    std::ofstream output_file(output_path);
    output_file << std::setprecision(kSigFig);

    // solve each row in input
    for (InputPointsRow row : input) {
        SolveRow(row, output_file);
    }

    // close I/O
    output_file.close();
} // SolveFile

void SolveRow(const InputPointsRow &row, std::ofstream &output_file) {

    // if the points to bisect are equal
    if (EqualsWithinTolerance(row.at(0), row.at(1))) {
        output_file << kInvalidComputationMsg << '\n';
    }
    // if a bisector plane can be generated
    else {
        // find the plane, distance and closest point
        PointNormalPlane plane = GenerateBisectorPlane(row.at(0), row.at(1));

        double distance = plane.FindDistanceToPoint(row.at(2));
        Eigen::Vector3d closest_point = plane.FindClosestPoint(row.at(2));

        // print closest point and distance
        output_file << closest_point.transpose().format(kSigFigFmt) << ' '
                    << distance << '\n';
    }
} // SolveRow

