// Jacob Hartt
// CS2300(T/R)
// 02/06/2023


#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <eigen3/Eigen/Dense>


enum class Operation {
	kAddition = 0,
	kSubtraction = 1,
	kScaling = 2,
	kDotProduct = 3,
	kCosineAngle = 4,
	kProjection = 5
};

struct Calculation {
	Operation operation;
	Eigen::Vector2d vector_1;
	Eigen::Vector2d vector_2;
};

// Converts a raw calculation (array of strings) to a calculation.  Will fail
// if the operation is invalid, the number of vector components are invalid,
// or the vector components are not doubles.
Calculation ConvertToCalculation(const std::vector<std::string>
                                 &raw_calculation);

// Generates an input file.
void GenInputFile(const std::string &file_path);

// Read the  at file_path’s data, create a jagged 2D string array with
// that data, and return the 2d array object.  Each row will contain each word
// in the input file’s given line.
std::vector<std::vector<std::string>> ReadInputFile(const std::string
                                                    &input_file_path);

// Splits a string at the spaces.
std::vector<std::string> SplitString(const std::string &str);

// Gets the dot product of two vectors in a custom implementation and returns
// the product.  
double VectorDotProductCustom(const Eigen::Vector2d &input_1,
                              const Eigen::Vector2d &input_2);

// Calculates the angle in radians between input_1 and input_2 using the theorem
// a dot b = ||a|| ||b|| cos(theta). 
double VectorCosineAngle(const Eigen::Vector2d &input_1,
                         const Eigen::Vector2d &input_2);

// Calculates the orthogonal projection of input_2 onto input_1. 
Eigen::Vector2d VectorProjection(const Eigen::Vector2d &input_1,
                                 const Eigen::Vector2d &input_2);

// Calculates input_1 scaled by the magnitude of input_2. 
Eigen::Vector2d VectorScaling(const Eigen::Vector2d &input_1,
                              const Eigen::Vector2d &input_2);

// Runs calculations on the file at input_file_path and outputs the results and
// errors to a file at output_file_path
void WriteVectorCalculationsFile(const std::string &input_file_path, 
                                 const std::string &output_file_path);


int main() {
    GenInputFile("jhartt_p3_input.txt");

    WriteVectorCalculationsFile("jhartt_p3_input.txt", "jhartt_p3_output.txt");
    WriteVectorCalculationsFile("class_p3_input.txt", "class_p3_output.txt");

    return 0;
} // main


Calculation ConvertToCalculation(const std::vector<std::string>
                                 &raw_calculation) {
    if (raw_calculation.size() != 5) {
        throw std::runtime_error("invalid number of arguments");
    }

    std::string raw_operation = raw_calculation[0];

    Calculation calculation;

    if (raw_operation == "AD") {
        calculation.operation = Operation::kAddition;
    } else if (raw_operation == "SU") {
        calculation.operation = Operation::kSubtraction;
    } else if (raw_operation == "SC") {
        calculation.operation = Operation::kScaling;
    } else if (raw_operation == "DO") {
        calculation.operation = Operation::kDotProduct;
    } else if (raw_operation == "CO") {
        calculation.operation = Operation::kCosineAngle;
    } else if (raw_operation == "PR") {
        calculation.operation = Operation::kProjection;
    } else {
        throw std::runtime_error("invalid operation");
    }

    // If a stod below fails, one of the raw components isn't a double
    calculation.vector_1 = Eigen::Vector2d(std::stod(raw_calculation[1]),
                                           std::stod(raw_calculation[2]));
    calculation.vector_2 = Eigen::Vector2d(std::stod(raw_calculation[3]),
                                           std::stod(raw_calculation[4]));
    
    return calculation;
} // ConvertToCalculation

void GenInputFile(const std::string &file_path) {
    std::ofstream gen_file;
    gen_file.open(file_path);

    // The below random number generation code is inspired by
    // https://stackoverflow.com/a/7560564
    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_int_distribution<> distr_rand(-10, 10);

    gen_file << "AD " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "SU " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "SC " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "DO " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "CO " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "PR " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "XD " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "SU " << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << '\n';
    gen_file << "PR " << "C" << ' ' << distr_rand(rand_gen) <<
            ' ' << distr_rand(rand_gen) << ' ' << distr_rand(rand_gen);

    gen_file.close();
} // GenInputFile

std::vector<std::vector<std::string>> ReadInputFile(const std::string
                                                    &input_file_path) {
    std::vector<std::vector<std::string>> out;

    std::ifstream input_file;
    input_file.open(input_file_path);

    while (input_file.good()) {
        std::string line;
        std::getline(input_file, line);
        std::vector<std::string> raw_calculations = SplitString(line);
        out.push_back(raw_calculations);
    }

    return out;
} // ReadInputFile

std::vector<std::string> SplitString(const std::string &str) {
    std::stringstream string_stream(str);
    std::vector<std::string> out;

    while (string_stream.good()) {
        std::string temp_str;
        string_stream >> temp_str;

        if (!temp_str.empty()) {
            out.push_back(temp_str);
        }
    }

    return out;
} // SplitString

double VectorDotProductCustom(const Eigen::Vector2d &input_1,
                              const Eigen::Vector2d &input_2) {
    return (input_1[0] * input_2[0]) + (input_1[1] * input_2[1]);
} // VectorDotProductCustom

double VectorCosineAngle(const Eigen::Vector2d &input_1,
                         const Eigen::Vector2d &input_2) {
    return std::acos((input_1.dot(input_2)) /
                     (input_1.norm() * input_2.norm()));
} // VectorDotProductCustom

Eigen::Vector2d VectorProjection(const Eigen::Vector2d &input_1,
                                 const Eigen::Vector2d &input_2) {
    double length = input_1.dot(input_2) / input_1.squaredNorm();
    return length * input_1;
} // VectorProjection

Eigen::Vector2d VectorScaling(const Eigen::Vector2d &input_1,
                              const Eigen::Vector2d &input_2) {
    return input_1 * input_2.norm();
} // VectorScaling

void WriteVectorCalculationsFile(const std::string &input_file_path, 
                                 const std::string &output_file_path) {
    std::vector<std::vector<std::string>> raw_calculations =
            ReadInputFile(input_file_path);
    
    std::ofstream output_file;
    output_file.open(output_file_path);

    for (std::vector<std::string> raw_calculation : raw_calculations) {
        try {
            Calculation calculation = ConvertToCalculation(raw_calculation);

            Eigen::Vector2d vec;
            double num;

            switch (calculation.operation) {
                case Operation::kAddition:
                    vec = calculation.vector_1 + calculation.vector_2;
                    output_file << "[" << vec[0] << ", " << vec[1] << "]\n";
                    break;

                case Operation::kSubtraction:
                    vec = calculation.vector_1 - calculation.vector_2;
                    output_file << "[" << vec[0] << ", " << vec[1] << "]\n";
                    break;

                case Operation::kScaling:
                    vec = VectorScaling(calculation.vector_1,
                                                           calculation.vector_2);
                    output_file << "[" << vec[0] << ", " << vec[1] << "]\n";
                    break;

                case Operation::kDotProduct:
                    num = VectorDotProductCustom(calculation.vector_1,
                                                 calculation.vector_2);
                    output_file << num << "\n";
                    break;

                case Operation::kCosineAngle:
                    output_file << VectorCosineAngle(calculation.vector_1,
                            calculation.vector_2) << " radians\n";
                    break;

                case Operation::kProjection:
                    vec = VectorProjection(calculation.vector_1,
                                           calculation.vector_2);
                    output_file << "[" << vec[0] << ", " << vec[1] << "]\n";
                    break;

                default:
                    output_file << "Congratulations, you found a unicorn!\n";
            }
        } catch (...) {
            output_file << "Error: invalid input data\n";
        }
    }
} // WriteVectorCalculationsFile
