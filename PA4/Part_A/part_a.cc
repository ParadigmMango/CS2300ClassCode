//!  CS2300 PA4 Part A: Calculating Distances to Lines and Planes
/*!
  \file part_a.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 04-10-2023
 */

#include <eigen3/Eigen/Dense>

#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ios>
#include <string>
#include <vector>

#include "../float_compare.hpp"


//! A format which is used to display output vectors with 4 sig figs.
//! The number of sig figs printed in outputs.
const unsigned int kSigFig = 4;

//! A format which is used to display output vectors with 4 sig figs.
//! For reference: https://eigen.tuxfamily.org/dox/structEigen_1_1IOFormat.html
const Eigen::IOFormat kSigFigFmt(kSigFig, 0, ", ", "\n", "[", "]");


//! An input structure to package the raw data and the dimension number together.
struct Input {
    int dimension_num_;
    std::vector<std::array<double, 9>> num_mat_;
}; // Input


//! Class representing 3D planes in point-normal form.
class PointNormalPlane {
  public:
    //!Construct a new Point Normal Plane object
    /*!
      \param normal_vec a normal vector to plug in to the plane
      \param normal_vec_tail the tail of the normal vector
     */
    PointNormalPlane(const Eigen::Vector3d &normal_vec,
                     const Eigen::Vector3d &normal_vec_tail) {
        // normalize the input normal vector to ensure point-normal form
        normal_vec_ = normal_vec.normalized();
        normal_vec_tail_ = normal_vec_tail;
    } // constructor

    double FindDistanceToPoint(const Eigen::Vector3d &point) {
        // calculate the distance using A*x1 + B*x2 + C*x3 + D
        double a = normal_vec_[0];
        double b = normal_vec_[1];
        double c = normal_vec_[2];
        double d = -normal_vec_.dot(normal_vec_tail_);

        // for some reason the function was returning negative values of the
        // correct answers, so I am taking the absolute value instead
        return std::fabs(a*point[0] + b*point[1] + c*point[2] + d);
    } // FindDistanceToPoint

    Eigen::Vector3d GetNormalVec() {
        return normal_vec_;
    } // GetNormalVec

  private:
    //! The normal unit vector of the plane.
    Eigen::Vector3d normal_vec_;
    //! The tail point of the normal vector.
    Eigen::Vector3d normal_vec_tail_;
}; // PointNormalPlane


//! Class representing paramatric 2D lines.
class ParametricLine2D {
  public:
    //! Construct a new 2D Parametric Line object
    /*!
      \param point_1 one point on the line
      \param point_2 another point on the line
     */
    ParametricLine2D(const Eigen::Vector2d &point_1,
                     const Eigen::Vector2d &point_2) {
        vec_v_ = point_2 - point_1;
        point_on_line_ = point_1;
    } // ParametricLine2D

    //! Finds the distance from the plane to a point using trigonometry.
    /*!
      \param point the point to find the distance to
      \return The distance to the point
     */
    double FindDistanceToPoint(const Eigen::Vector2d &point) {
        Eigen::Vector2d vec_w = point - point_on_line_;

        // calculate cosine alpha using the equation ||v|| ||w|| cos a = v . w
        double cosine_alpha = (vec_v_.dot(vec_w)) / (vec_v_.norm() * vec_w.norm());
        // calculate sine alpha using the pythagorean theorem
        double sine_alpha = std::sqrt(1.0 - cosine_alpha*cosine_alpha);
        // calculate the distance by using sine alpha and vec_w
        double distance = vec_w.norm() * sine_alpha;

        return distance;
    } // FindDistanceToPoint

    Eigen::Vector2d GetVecV() {
        return vec_v_;
    } // GetVecV

  private:
    //! The vector which defines the direction of the line.
    Eigen::Vector2d vec_v_;
    //! A point on the line which constrains it.
    Eigen::Vector2d point_on_line_;
}; // ParametricLine2D


//! Generate an input object from an input file.
/*!
  \param input_path the path of the input file
  \return An input object which contains the data of the input file
 */
Input GetInput(const std::string &input_path);

//! Finds a 2D orthonormal vector to any given 2D vector.
/*!
  \param vec the vector to find an orthonormal vector for
  \return The orthormal vector to vec
 */
Eigen::Vector2d FindOrthonormal(const Eigen::Vector2d &vec);

//! Generates a 3D bisector plane given two points.
/*!
  \param point_1 the first point
  \param point_2 the second point
  \return A 3D point-normal form plane that bisects the two points
 */
PointNormalPlane GenerateBisectorPlane(const Eigen::Vector3d &point_1,
                                       const Eigen::Vector3d &point_2);

//! Solves a file by reading from an input and writing to the output.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);

//! Solves inputs that are 2D cases.
/*!
  \param num_mat the raw data of an input object
  \param output_file the output file
 */
void Solve2D(const std::vector<std::array<double, 9>> &num_mat, 
             std::ofstream &output_file);

//! Solves inputs that are 3D cases.
/*!
  \param num_mat the raw data of an input object
  \param output_file the output file
 */
void Solve3D(const std::vector<std::array<double, 9>> &num_mat, 
             std::ofstream &output_file);


int main() {
    SolveFile("class_input_1.txt", "jhartt_output_A_1.txt");
    SolveFile("class_input_2.txt", "jhartt_output_A_2.txt");
    SolveFile("class_input_3.txt", "jhartt_output_A_3.txt");
    SolveFile("jhartt_input_1.txt", "jhartt_output_A_4.txt");
    SolveFile("jhartt_input_2.txt", "jhartt_output_A_5.txt");

    return 0;
} // main


Input GetInput(const std::string &input_path) {
    Input input;

    // open input_file
    std::ifstream input_file;
    input_file.open(input_path);
    
    // fill input's matrix
    double num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9;

    while (input_file >> num_1 >> num_2 >> num_3 >> num_4 >> num_5 >> num_6 >>
           num_7 >> num_8 >> num_9) {
        std::array<double, 9> row = {num_1, num_2, num_3, num_4, num_5, num_6,
                                     num_7, num_8, num_9};
        input.num_mat_.push_back(row);
    }
    
    // grab the int at the end of the file
    input.dimension_num_ = (int) num_1;
    
    // close input_file
    input_file.close();

    return input;
} // GetInput

Eigen::Vector2d FindOrthonormal(const Eigen::Vector2d &vec) {
    Eigen::Vector2d ret_vec(-vec[1], vec[0]);
    ret_vec.normalize();

    return ret_vec;
} // FindOrthonormal

PointNormalPlane GenerateBisectorPlane(const Eigen::Vector3d &point_1,
                                       const Eigen::Vector3d &point_2) {
    Eigen::Vector3d midpoint = (point_1 + point_2) / 2.0;
    Eigen::Vector3d normal_vector = point_2 - point_1;

    PointNormalPlane ret_plane(normal_vector, midpoint);
    
    return ret_plane;
} // GenerateBisectorPlane

void SolveFile(const std::string &input_path, const std::string &output_path) {
    // initialize I/O
    Input input = GetInput(input_path);
    std::ofstream output_file(output_path);

    // solve for 2D/3D or print error
    if (input.dimension_num_ == 2) {
        Solve2D(input.num_mat_, output_file);
    } else if (input.dimension_num_ == 3) {
        Solve3D(input.num_mat_, output_file);
    } else {
        output_file << "Invalid Computation\n";
    }

    output_file.close();
} // SolveFile

void Solve2D(const std::vector<std::array<double, 9>> &num_mat, 
             std::ofstream &output_file) {
    // set output 
    output_file << std::setprecision(kSigFig);

    // iterate through the data row by row
    for (std::array<double, 9> row : num_mat) {
        // find the first two points
        Eigen::Vector2d point_1(row[0], row[1]);
        Eigen::Vector2d point_2(row[2], row[3]);

        // if first two points are equal, print error, as case is invalid
        if (EqualsWithinTolerance(point_1, point_2)) {
            output_file << "Invalid Computation\n";
        } else {
            // find the orthonormal and distance from point_3 to the line
            ParametricLine2D line(point_1, point_2);

            Eigen::Vector2d orthonormal = FindOrthonormal(line.GetVecV());
        
            Eigen::Vector2d point_3(row[4], row[5]);
            double distance = line.FindDistanceToPoint(point_3);

            // print the orthonormal and distance
            output_file << orthonormal.transpose().format(kSigFigFmt) << '\t'
                        << distance << std::endl;
        }
    } // row for each loop
} // Solve2D

void Solve3D(const std::vector<std::array<double, 9>> &num_mat, 
             std::ofstream &output_file) {
    // set output 
    output_file << std::setprecision(kSigFig);

    for (std::array<double, 9> row : num_mat) {
        // find the first two points
        Eigen::Vector3d point_1(row[0], row[1], row[2]);
        Eigen::Vector3d point_2(row[3], row[4], row[5]);

        // if first two points are equal, print error, as case is invalid
        if (EqualsWithinTolerance(point_1, point_2)) {
            output_file << "Invalid Computation\n";
        } else {
            // find distance from point_3 to the line
            PointNormalPlane bisector = GenerateBisectorPlane(point_1, point_2);
        
            Eigen::Vector3d point_3(row[6], row[7], row[8]);
            double distance = bisector.FindDistanceToPoint(point_3);
            
            // print the normal and distance
            output_file << bisector.GetNormalVec().transpose().format(kSigFigFmt)
                        << '\t' << distance << std::endl;
        }
    } // row for each loop
} // Solve3D
