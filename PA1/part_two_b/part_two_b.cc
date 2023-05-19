// Jacob Hartt
// CS2300(T/R)
// 02/06/2023


#include <fstream>
#include <string>
#include <vector>

#include <eigen3/Eigen/Dense> // Headers are located at /usr/include/eigen3


// Multiplies two matrices in a custom implementation and returns the product.
// Assumes input matrices can be multiplied.
Eigen::MatrixXd MatProductCustom(const Eigen::MatrixXd &input_1,
                                 const Eigen::MatrixXd &input_2);

// Multiplies two matrices using Eigen and returns the product.
// Assumes input matrices can be multiplied.
Eigen::MatrixXd MatProductEigen(const Eigen::MatrixXd &input_1,
                                const Eigen::MatrixXd &input_2);

// Reads the matrix at file_path’s data, creates a matrix object with that data,
// and returns the matrix object.
Eigen::MatrixXd ReadMatFile(const std::string &read_file_path);

// Writes the contents and dimensions of an Eigen dynamic doubles matrix into a file at the second input's file path.
void WriteMatFile(const Eigen::MatrixXd &mat,
                  const std::string &write_file_path);

// Write the matrix product of the two input matrices, or an error message,
// to a file at output_path using MatProductCustom.
void WriteMatProductFileCustom(const Eigen::MatrixXd &input_1,
                               const Eigen::MatrixXd &input_2,
                               const std::string &output_path);

// Write the matrix prduct of the two input matrices, or an error message,
// to a file at output_path using MatProductEigen.
void WriteMatProductFileEigen(const Eigen::MatrixXd &input_1,
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
    const std::vector<Eigen::MatrixXd> kMatArr{k1Mat, k2Mat, k3Mat,
                                               k4Mat, k5Mat};

    // The permutations of a set of 5 and a subset of 2 with repetition
    // is implemented somewhat simply here.
    for (int first_mat_num = 0; first_mat_num < 5; first_mat_num++) {
        for (int second_mat_num = 0; second_mat_num < 5; second_mat_num++) {
            std::string output_path = "jhartt_p2b_out" +
                                      std::to_string(first_mat_num + 1) +
                                      std::to_string(second_mat_num + 1) +
                                      ".txt";
            
            // Alternate MatProduct implementations for demonstration purposes.
            if ((first_mat_num + second_mat_num) % 2 == 0) {
                WriteMatProductFileCustom(kMatArr[first_mat_num],
                                          kMatArr[second_mat_num], output_path);
            } else {
                WriteMatProductFileEigen(kMatArr[first_mat_num],
                                         kMatArr[second_mat_num], output_path);
            }
        }
    }

    return 0;
} // main

Eigen::MatrixXd MatProductCustom(const Eigen::MatrixXd &input_1,
                                 const Eigen::MatrixXd &input_2) {
    Eigen::MatrixXd product_mat(input_1.rows(), input_2.cols());

    for (int row = 0; row < product_mat.rows(); row++) {
        for (int col = 0; col < product_mat.cols(); col++) {
            int element_sum = 0;

            for (int inner_index = 0; inner_index < input_1.cols();
                    inner_index++) {
                element_sum += input_1(row, inner_index) *
                               input_2(inner_index, col);
            }

            product_mat(row, col) = element_sum;
        }
    }

    return product_mat;
} // MatProductCustom

Eigen::MatrixXd MatProductEigen(const Eigen::MatrixXd &input_1,
                                const Eigen::MatrixXd &input_2) {
    return input_1 * input_2;
} // MatProductEigen

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

void WriteMatProductFileCustom(const Eigen::MatrixXd &input_1,
                               const Eigen::MatrixXd &input_2,
                               const std::string &output_path) {
    if (input_1.cols() == input_2.rows()) {
        Eigen::MatrixXd product_mat = MatProductCustom(input_1, input_2);
        WriteMatFile(product_mat, output_path);
    } else {
        std::ofstream mat_file;
        mat_file.open(output_path);

        mat_file << "Error: matrices have incompatible dimensions for multiplication";

        mat_file.close();
    }
} // WriteMatProductFileCustom

void WriteMatProductFileEigen(const Eigen::MatrixXd &input_1,
                              const Eigen::MatrixXd &input_2,
                              const std::string &output_path) {
    if (input_1.cols() == input_2.rows()) {
        Eigen::MatrixXd product_mat = MatProductEigen(input_1, input_2);
        WriteMatFile(product_mat, output_path);
    } else {
        std::ofstream mat_file;
        mat_file.open(output_path);

        mat_file << "Error: matrices have incompatible dimensions for multiplication";

        mat_file.close();
    }
} // WriteMatProductFileEigen
