//! PA5 Part D: Calculating Line-Triangle Intersections
/*!
  \file part_d.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 05-08-2023
 */

#include <eigen3/Eigen/Dense>

#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
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


//! Finds the minimum angle between two vectors using the cosine-dot product
//! formula in radians.
/*!
  \param vec_1 the first vector
  \param vec_2 the second vector
  \return The angle between the vectors in radians
 */
double AngleBetweenVecs(const Eigen::Vector3d &vec_1,
                        const Eigen::Vector3d &vec_2);

//! Compares two vectors to see if they are parallel.
/*!
  \param vec_1 the first vector
  \param vec_2 the second vector
  \return Whether or not the vectors are parallel
 */
bool AreParallel(const Eigen::Vector3d &vec_1, const Eigen::Vector3d &vec_2);

//! Solves a file.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);


//! A class which contains the information for a parametric line in 3D.
class ParametricLine {
  public:
    //! Construct a new Parametric Line object.
    /*!
      \param point_1 one point on the line
      \param point_2 another point on the line
     */
    ParametricLine(const Eigen::Vector3d &point_1,
                   const Eigen::Vector3d &point_2) {
        vec_v_ = point_2 - point_1;
        point_on_line_ = point_1;
    } // Constructor

    //! Get the Vec V object
    /*!
      \return vec_v_
     */
    Eigen::Vector3d GetVecV() {
        return vec_v_;
    } // GetVecV

    //! Get the Point On Line object
    /*!
      \return point_on_line_
     */
    Eigen::Vector3d GetPointOnLine() {
        return point_on_line_;
    } // GetPointOnLine

  private:
    //! The direction of the line.
    Eigen::Vector3d vec_v_;
    //! A point on the line.
    Eigen::Vector3d point_on_line_;
}; // ParametricLine


//! A class which tracks and contains various functionality for 3D triangles.
class Triangle {
  public:
    //! Construct a new Triangle object
    /*!
      \param point_1 the first corner of the triangle
      \param point_2 the second corner of the triangle
      \param point_3 the third corner of the triangle
     */
    Triangle(Eigen::Vector3d point_1, Eigen::Vector3d point_2,
             Eigen::Vector3d point_3) {
        // assign points
        point_1_ = point_1;
        point_2_ = point_2;
        point_3_ = point_3;

        // create edge vectors
        Eigen::Vector3d v_0 = point_2_ - point_1_;
        Eigen::Vector3d v_1 = point_3_ - point_1_;

        // if the vectors are parallel, the triangle has no area
        if (AreParallel(v_0, v_1)) {
            has_area_ = false;
        } else {
            // create a plane for the triangle and set has area to true
            plane_ = PointNormalPlane(v_0.cross(v_1), point_1_);

            has_area_ = true;
        }
    } // Constructor

    //! Determines if a point is inside of the triangle.
    /*!
      \param point the point
      \return Whether or not the point is inside the triangle
     */
    bool IsPointInside(const Eigen::Vector3d &point) {
        // If the point is a corner return true
        if (EqualsWithinTolerance(point, point_1_) ||
                EqualsWithinTolerance(point, point_2_) ||
                EqualsWithinTolerance(point, point_3_)) {
            return true;
        } else {
            // create vectors from points to the corners
            Eigen::Vector3d v_1 = point_1_ - point;
            Eigen::Vector3d v_2 = point_2_ - point;
            Eigen::Vector3d v_3 = point_3_ - point;

            // sum up the angles between the vectors
            double sum = AngleBetweenVecs(v_1, v_2) + AngleBetweenVecs(v_1, v_3)
                         + AngleBetweenVecs(v_2, v_3);
            
            // if the sum = 2 * pi the the point is within the triangle
            return EqualsWithinTolerance(sum, 2 * M_PI);
        }
    } // IsPointInside

    //! Returns whether or not the triangle has non-zero area
    /*!
      \return whether or not the triangle has area
     */
    bool HasArea() {
        return has_area_;
    } // HasArea

    //! Get the Plane object
    /*!
      \return the plane of the triangle 
     */
    PointNormalPlane &GetPlane() {
        return plane_;
    } // GetPlane

  private:
    //! The plane of the triangle.
    PointNormalPlane plane_;

    //! The vertices of the triangle.
    Eigen::Vector3d point_1_;
    Eigen::Vector3d point_2_;
    Eigen::Vector3d point_3_;

    // Whether or not the triangle has area.
    bool has_area_;
}; // Triangle


//! An input data structure to package input information
struct Input {
    ParametricLine line_;
    std::vector<Triangle> triangles_;
}; // Input


//! Generate an input object from an input file.
/*!
  \param input_path the path of the input file
  \return An input object which contains the data of the input file
 */
Input GetInput(const std::string &input_path);

//! Finds an intersection between the plane and the line.
/*!
  \param plane the plane
  \param line the line
  \return The intersection point
 */
Eigen::Vector3d GetIntersection(PointNormalPlane &plane, ParametricLine &line);

//! Check if an intersection exists between the plane and the line.
/*!
  \param plane the plane
  \param line the line
  \return Whether or not the intersection exists
 */
bool IntersectionExists(PointNormalPlane &plane, ParametricLine &line);


int main() {
    SolveFile("class_input_1-2.txt", "jhartt_output_D_1.txt");
    SolveFile("class_input_2-2.txt", "jhartt_output_D_2.txt");
    SolveFile("class_input_3-2.txt", "jhartt_output_D_3.txt");
    SolveFile("jhartt_input_4-2.txt", "jhartt_output_D_4.txt");
    SolveFile("jhartt_input_5-2.txt", "jhartt_output_D_5.txt");

    return 0;
} // main


double AngleBetweenVecs(const Eigen::Vector3d &vec_1,
                        const Eigen::Vector3d &vec_2) {
    // calculate the products
    double dot_product = vec_1.dot(vec_2);
    double magnitude_product = vec_1.norm() * vec_2.norm();

    // calculate the inverse cosine of the ratio of the products
    return std::acos(dot_product / magnitude_product);
} // AngleBetweenVecs

bool AreParallel(const Eigen::Vector3d &vec_1, const Eigen::Vector3d &vec_2) {
    // calculate the products
    double dot_product = vec_1.dot(vec_2);
    double magnitude_product = vec_1.norm() * vec_2.norm();

    // check if the products are equal to +/- each other
    return dot_product == magnitude_product || 
           dot_product == -magnitude_product; 
} // AreParallel

Input GetInput(const std::string &input_path) {
    // get the raw input points
    InputPoints input_points = GetInputAsPoints(input_path);

    // create a line from the first two points
    ParametricLine line(input_points.at(0).at(0), input_points.at(0).at(1));

    // create a triangles vector
    std::vector<Triangle> triangles;

    // fill the triangles vector with a triangle made from each row of points
    for (size_t i = 1; i < input_points.size(); i++) {
        Triangle triangle(input_points.at(i).at(0), input_points.at(i).at(1), 
                          input_points.at(i).at(2));

        triangles.push_back(triangle);
    }

    // create the input return
    Input ret_input = {line, triangles};

    return ret_input;
} // GetInput

Eigen::Vector3d GetIntersection(PointNormalPlane &plane, ParametricLine &line) {
    // initialize the intersection value
    Eigen::Vector3d intersection = line.GetVecV();

    // calculate the numerator and denominator components of the intersection
    // multiplier
    double numerator = plane.GetNormalVec()
                        .dot(plane.GetNormalVecTail() - line.GetPointOnLine());
    double denominator = plane.GetNormalVec().dot(line.GetVecV());

    // scale the intersection
    intersection *= numerator / denominator;

    // translate the intersection
    intersection += line.GetPointOnLine();

    return intersection;
} // GetIntersection

bool IntersectionExists(PointNormalPlane &plane, ParametricLine &line) {
    // check that the denominator component of the intersection multipier is not
    // zero
    return !EqualsWithinTolerance(plane.GetNormalVec().dot(line.GetVecV()), 0);
}

void SolveFile(const std::string &input_path, const std::string &output_path) {
    // initialize I/O
    Input input = GetInput(input_path);
    std::ofstream output_file(output_path);

    // iterate through triangles
    for (Triangle triangle : input.triangles_) {
        // if the triangle has area and the intersection of the plane with the 
        // triangle exists
        if (triangle.HasArea() && IntersectionExists(triangle.GetPlane(),
                                                     input.line_)) {
            // calculate the intersection of the line with the plane of the
            // triangle
            Eigen::Vector3d intersection = GetIntersection(triangle.GetPlane(),
                                                           input.line_);
            
            // print whether or not the point is inside the triangle
            if (triangle.IsPointInside(intersection)) {
                output_file << "1 ";
            } else {
                output_file << "0 ";
            }

            // print out the intersection
            output_file << intersection.transpose().format(kSigFigFmt) << '\n';
        } else {
            // print out the invalid computation message
            output_file << kInvalidComputationMsg << '\n';
        }
    } // triangles loop

    // close the output file
    output_file.close();
} // SolveFile
