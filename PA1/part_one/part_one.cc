// Jacob Hartt
// CS2300(T/R)
// 02/06/2023

#include <fstream>
#include <string>

#include <eigen3/Eigen/Dense> // Headers are located at /usr/include/eigen3


// Writes the dimensions and contents of an Eigen dynamic doubles matrix into
// a file at the second input's file path. The method is very simple due to
// the built-in operator overloader of "<<" in the Eigen matrix classes which
// automatically formats and inserts the contents of a matrix object into
// a stream.`
void WriteMatFile(const Eigen::MatrixXd &mat,
                  const std::string &write_file_path);

int main() {
    // Note to self: std::string does not count string termination characters
    // in its size() function.
    const std::string kFirstName = "Jacob";
    const std::string kLastName = "Hartt";
    const int kFirstNameLen = static_cast<int>(kFirstName.size());
    const int kLastNameLen = static_cast<int>(kLastName.size());

    Eigen::MatrixXd mat_1(kLastNameLen, kFirstNameLen);
    double counter_1 = 1.0;
    for (int row = 0; row < mat_1.rows(); row++) {
        for (int col = 0; col < mat_1.cols(); col++) {
            mat_1(row, col) = counter_1;
            counter_1++;
        }
    }
    WriteMatFile(mat_1, "jhartt_p1_mat1.txt");

    Eigen::MatrixXd mat_2(kFirstNameLen, kLastNameLen);
    double counter_2 = 3.0;
    for (int col = 0; col < mat_2.cols(); col++) {
        for (int row = 0; row < mat_2.rows(); row++) {
            mat_2(row, col) = counter_2;
            counter_2 += 5.0;
        }
    }
    WriteMatFile(mat_2, "jhartt_p1_mat2.txt");

    Eigen::MatrixXd mat_3(kFirstNameLen, kLastNameLen);
    double counter_3 = 0.33;
    for (int col = 0; col < mat_3.cols(); col++) {
        for (int row = 0; row < mat_3.rows(); row++) {
            mat_3(row, col) = counter_3;
            counter_3 += 0.6;
        }
    }
    WriteMatFile(mat_3, "jhartt_p1_mat3.txt");

    Eigen::MatrixXd mat_4(5, 6);
    double counter_4 = 3.0;
    for (int col = 0; col < mat_4.cols(); col++) {
        for (int row = 0; row < mat_4.rows(); row++) {
            mat_4(row, col) = counter_4;
            counter_4 += 2.0;
        }
    }
    WriteMatFile(mat_4, "jhartt_p1_mat4.txt");

    Eigen::MatrixXd mat_5(6, 5);
    double counter_5 = -10.0;
    for (int row = 0; row < mat_5.rows(); row++) {
        for (int col = 0; col < mat_5.cols(); col++) {
            mat_5(row, col) = counter_5;
            counter_5++;
        }
    }
    WriteMatFile(mat_5, "jhartt_p1_mat5.txt");

    return 0;
} // main

void WriteMatFile(const Eigen::MatrixXd &mat,
                  const std::string &write_file_path) {
    std::ofstream mat_file;
    mat_file.open(write_file_path);

    mat_file << mat.rows() << ' ' << mat.cols() << std::endl;
    mat_file << std::endl;
    mat_file << mat;

    mat_file.close();
} // WriteMatFile
