// Jacob Hartt
// CS2300(T/R)
// 02/06/2023


#include <fstream>
#include <string>

#include <eigen3/Eigen/Dense> // Headers are located at /usr/include/eigen3


// Adds two matrices in a custom implementation and returns the sum.
// Assumes input matrices can be added.
Eigen::MatrixXd MatSumCustom(const Eigen::MatrixXd &input_1,
                             const Eigen::MatrixXd &input_2);

// Adds two matrices using Eigen and returns the sum.
// Assumes input matrices can be added.
Eigen::MatrixXd MatSumEigen(const Eigen::MatrixXd &input_1,
                            const Eigen::MatrixXd &input_2);

// Reads the matrix at file_path’s data, creates a matrix object with that data,
// and returns the matrix object.
Eigen::MatrixXd ReadMatFile(const std::string &read_file_path);

// Writes the contents and dimensions of an Eigen dynamic doubles matrix into a file at the second input's file path.
void WriteMatFile(const Eigen::MatrixXd &mat,
                  const std::string &write_file_path);

// Write the matrix sum of the two input matrices, or an error message,
// to a file at output_path using MatSumCustom.
void WriteMatSumFileCustom(const Eigen::MatrixXd &input_1,
                           const Eigen::MatrixXd &input_2,
                           const std::string &output_path);

// Write the matrix sum of the two input matrices, or an error message,
// to a file at output_path using MatSumEigen.
void WriteMatSumFileEigen(const Eigen::MatrixXd &input_1,
                          const Eigen::MatrixXd &input_2,
                          const std::string &output_path);


int main() {
    const std::string kMat1Path = "../part_one/jhartt_p1_mat1.txt";
    const std::string kMat2Path = "../part_one/jhartt_p1_mat2.txt";
    const std::string kMat3Path = "../part_one/jhartt_p1_mat3.txt";
    const std::string kMat4Path = "../part_one/jhartt_p1_mat4.txt";
    const std::string kMat5Path = "../part_one/jhartt_p1_mat5.txt";
    const Eigen::MatrixXd k1Mat = ReadMatFile(kMat1Path);
    const Eigen::MatrixXd k2Mat = ReadMatFile(kMat2Path);
    const Eigen::MatrixXd k3Mat = ReadMatFile(kMat3Path);
    const Eigen::MatrixXd k4Mat = ReadMatFile(kMat4Path);
    const Eigen::MatrixXd k5Mat = ReadMatFile(kMat5Path);

    WriteMatSumFileCustom(k1Mat, k1Mat, "jhartt_p2a_out11.txt");
    WriteMatSumFileEigen(k1Mat, k2Mat, "jhartt_p2a_out12.txt");
    WriteMatSumFileCustom(k1Mat, k3Mat, "jhartt_p2a_out13.txt");
    WriteMatSumFileEigen(k1Mat, k4Mat, "jhartt_p2a_out14.txt");
    WriteMatSumFileCustom(k1Mat, k5Mat, "jhartt_p2a_out15.txt");
    WriteMatSumFileEigen(k2Mat, k2Mat, "jhartt_p2a_out22.txt");
    WriteMatSumFileCustom(k2Mat, k3Mat, "jhartt_p2a_out23.txt");
    WriteMatSumFileEigen(k2Mat, k4Mat, "jhartt_p2a_out24.txt");
    WriteMatSumFileCustom(k2Mat, k5Mat, "jhartt_p2a_out25.txt");
    WriteMatSumFileEigen(k3Mat, k3Mat, "jhartt_p2a_out33.txt");
    WriteMatSumFileCustom(k3Mat, k4Mat, "jhartt_p2a_out34.txt");
    WriteMatSumFileEigen(k3Mat, k5Mat, "jhartt_p2a_out35.txt");
    WriteMatSumFileCustom(k4Mat, k4Mat, "jhartt_p2a_out44.txt");
    WriteMatSumFileEigen(k4Mat, k5Mat, "jhartt_p2a_out45.txt");
    WriteMatSumFileCustom(k5Mat, k5Mat, "jhartt_p2a_out55.txt");

    return 0;
} // main

Eigen::MatrixXd MatSumCustom(const Eigen::MatrixXd &input_1,
                             const Eigen::MatrixXd &input_2) {
    int rows = input_1.rows();
    int cols = input_2.cols();
    
    Eigen::MatrixXd sum_mat(rows, cols);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            sum_mat(row, col) = input_1(row, col) + input_2(row, col);
        }
    }

    return sum_mat;
} // MatSumCustom

Eigen::MatrixXd MatSumEigen(const Eigen::MatrixXd &input_1,
                            const Eigen::MatrixXd &input_2) {
    return input_1 + input_2;
} // MatSumEigen

Eigen::MatrixXd ReadMatFile(const std::string &read_file_path) {
    std::ifstream read_file;
    read_file.open(read_file_path);

    // Makes sure the read_file actually exists, otherwise ends the program
    assert(read_file.good());

    std::string raw_rows;
    read_file >> raw_rows;
    int rows = std::stod(raw_rows);

    std::string raw_cols;
    read_file >> raw_cols;
    int cols = std::stod(raw_cols);

    Eigen::MatrixXd out_mat(rows, cols);

    std::string raw_element = "";
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            read_file >> raw_element;
            out_mat(row, col) = stod(raw_element);
        }
    }

    return out_mat;
} // ReadMatFile

void WriteMatFile(const Eigen::MatrixXd &mat,
                  const std::string &write_file_path) {
    std::ofstream mat_file;
    mat_file.open(write_file_path);

    mat_file << mat.rows() << ' ' << mat.cols() << std::endl;
    mat_file << std::endl;
    mat_file << mat;

    mat_file.close();
} // WriteMatFile

void WriteMatSumFileCustom(const Eigen::MatrixXd &input_1,
                           const Eigen::MatrixXd &input_2,
                           const std::string &output_path) {
    if (input_1.rows() == input_2.rows() && input_1.cols() == input_2.cols()) {
        Eigen::MatrixXd sum_mat = MatSumCustom(input_1, input_2);
        WriteMatFile(sum_mat, output_path);
    } else {
        std::ofstream mat_file;
        mat_file.open(output_path);

        mat_file << "Error: matrices have different dimensions";

        mat_file.close();
    }
} // WriteMatSumFileCustom

void WriteMatSumFileEigen(const Eigen::MatrixXd &input_1,
                          const Eigen::MatrixXd &input_2,
                          const std::string &output_path) {
    if (input_1.rows() == input_2.rows() && input_1.cols() == input_2.cols()) {
        Eigen::MatrixXd sum_mat = MatSumEigen(input_1, input_2);
        WriteMatFile(sum_mat, output_path);
    } else {
        std::ofstream mat_file;
        mat_file.open(output_path);

        mat_file << "Error: matrices have different dimensions";

        mat_file.close();
    }
} // WriteMatSumFileEigen
