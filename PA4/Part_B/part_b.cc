//!  CS2300 PA4 Part B: Calculating Lambertian Lighting
/*!
  \file part_b.cc
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 04-10-2023
 */

#include <eigen3/Eigen/Dense>

#include <array>
#include <fstream>
#include <iomanip>
#include <ios>
#include <string>
#include <vector>

#include "../float_compare.hpp"


//! A format which is used to display output vectors with 4 sig figs.
//! The number of sig figs printed in outputs.
const unsigned int kSigFig = 4;


//! An input structure to package the raw data and the dimension number together.
struct Input {
    int dimension_num_;
    std::vector<std::array<double, 9>> num_mat_;
}; // Input


//! A triangle class for lighting calculations.
class Triangle {
  public:
    //! Construct a new triange object using its vertices
    /*!
      \param vertex_1 the first vertex
      \param vertex_2 the second vertex
      \param vertex_3 the third vertex
     */
    Triangle(const Eigen::Vector3d &vertex_1, const Eigen::Vector3d &vertex_2,
             const Eigen::Vector3d &vertex_3) {
        // calculate the centroid
        centroid_ = (vertex_1 + vertex_2 + vertex_3) / 3.0;

        // calculate the edge vectors
        Eigen::Vector3d edge_1 = vertex_2 - vertex_1;
        Eigen::Vector3d edge_2 = vertex_3 - vertex_1;

        // find the normal vector and normalize it 
        normal_vec_ = edge_1.cross(edge_2);
        normal_vec_.normalize();
    } // constructor

    //! Finds the intesity of lighting of the triangle given the location of a 
    //! light source.
    /*!
      \param light the location of the light source
      \return The intesity of the triangle's lambertian lighting
     */
    double FindIntensity(const Eigen::Vector3d &light) {
        // find the direction vector of the light and normalize it
        Eigen::Vector3d light_dir = light - centroid_;
        light_dir.normalize();

        // use the dot product to find the light intensity
        double intensity = normal_vec_.dot(light_dir);

        // if the intensity is < 0 than it can be set to 0
        if (intensity < 0.0) {
            intensity = 0.0;
        }

        return intensity;
    } // FindIntensity

    //! Determines if a triangle should be culled given a light and eye location.
    /*!
      \param light the location of the light source
      \param eye the location of the eye
      \return Whether or not the triangle should be culled
     */
    bool ShouldCull(const Eigen::Vector3d &light, const Eigen::Vector3d &eye) {
        // generate and normalize the direction from the centroid to the ye
        Eigen::Vector3d eye_dir = eye - centroid_;
        eye_dir.normalize();

        // if the angle between the normal and the eye_dir is > 90 degerees, cull
        return normal_vec_.dot(eye_dir) < 0;
    } // ShouldCull

    //! Combines FindIntesity and ShouldCull to calculate the intensity only when
    //! necessary.
    /*!
      \param light 
      \param eye 
      \return double 
     */
    double FindCulledIntensity(const Eigen::Vector3d &light,
                               const Eigen::Vector3d &eye) {
        if (ShouldCull(light, eye)) {
            return 0;
        } else {
            return FindIntensity(light);
        }
    } // FindCulledIntensity

    //! Get the centroid object.
    /*!
      \return The centroid
     */
    Eigen::Vector3d GetCentroid() {
        return centroid_;
    } // GetCentroid

  private:
    //! The centroid of the triangle.
    Eigen::Vector3d centroid_;
    //! The normal vector of the triangle.
    Eigen::Vector3d normal_vec_;
}; // Triangle


//! Generate an input object from an input file.
/*!
  \param input_path the path of the input file
  \return An input object which contains the data of the input file
 */
Input GetInput(const std::string &input_path);

//! Solves a file by reading from an input and writing to the output.
/*!
  \param input_path the path of the input file
  \param output_path the path of the output file
 */
void SolveFile(const std::string &input_path, const std::string &output_path);


int main() {
    SolveFile("class_input_1.txt", "jhartt_output_B_1.txt");
    SolveFile("class_input_2.txt", "jhartt_output_B_2.txt");
    SolveFile("class_input_3.txt", "jhartt_output_B_3.txt");
    SolveFile("jhartt_input_1.txt", "jhartt_output_B_4.txt");
    SolveFile("jhartt_input_2.txt", "jhartt_output_B_5.txt");

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

void SolveFile(const std::string &input_path, const std::string &output_path) {
    // initialize I/O
    Input input = GetInput(input_path);
    std::ofstream output_file(output_path);
    output_file << std::setprecision(kSigFig);

    // find eye location and light source location
    Eigen::Vector3d eye(input.num_mat_[0][0], input.num_mat_[0][1],
                        input.num_mat_[0][2]);
    Eigen::Vector3d light(input.num_mat_[0][3], input.num_mat_[0][4],
                          input.num_mat_[0][5]);

    // iterate through every data row except the first
    for (size_t row_num = 1; row_num < input.num_mat_.size(); row_num++) {
        // find the triangular vertices
        Eigen::Vector3d vertex_1(input.num_mat_[row_num][0],
                                 input.num_mat_[row_num][1],
                                 input.num_mat_[row_num][2]);
        Eigen::Vector3d vertex_2(input.num_mat_[row_num][3],
                                 input.num_mat_[row_num][4],
                                 input.num_mat_[row_num][5]);
        Eigen::Vector3d vertex_3(input.num_mat_[row_num][6],
                                 input.num_mat_[row_num][7],
                                 input.num_mat_[row_num][8]);

        // check if any two vertices are equal, as that is an invalid input
        if (EqualsWithinTolerance(vertex_1, vertex_2) ||
                EqualsWithinTolerance(vertex_1, vertex_3) ||
                EqualsWithinTolerance(vertex_2, vertex_3)) {
            output_file << "Invalid Computation\n";
        } else {
            // make the triangle
            Triangle triangle(vertex_1, vertex_2, vertex_3);

            // check if the centroid is equal to either the light source or the 
            // eye, as those are also invalid inputs
            if (EqualsWithinTolerance(triangle.GetCentroid(), light) ||
                    EqualsWithinTolerance(triangle.GetCentroid(), eye)) {
                output_file << "Invalid Computation\n";
            } else {
                // print out the return values of all light calculation functions
                output_file << triangle.ShouldCull(light, eye) << '\t'
                            << triangle.FindIntensity(light) << '\t'
                            << triangle.FindCulledIntensity(light, eye) << '\n';
            }
        }
    } // row for loop

    output_file.close();
} // SolveFile
