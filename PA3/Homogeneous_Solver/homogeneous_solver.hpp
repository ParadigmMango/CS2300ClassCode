#ifndef HOMOGENEOUS_SOLVER_H_
#define HOMOGENEOUS_SOLVER_H_

//!  Contains an algorithm to solve 2x2 homogeneous systems
/*!
  \file homogeneous_solver.hpp
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 03-20-2023
 */

#include <eigen3/Eigen/Dense>

#include "../float_compare.hpp"

//! Solves a 2x2 homogeneous system by getting the normal vector.
/*!
  \param mat The matrix to solve for
  \param solution the reference to the solution vector
  \return Whether or not the solution is trivial
 */
bool SolveHomogeneousSystem(const Eigen::Matrix2d mat,
                            Eigen::Vector2d &solution) {
    // handle trivial solutions
    if (!EqualsWithinTolerance(mat.determinant(), 0.0)) {
        return false;
    }

    if (mat(0,0) == 0.0 && mat(0,1) == 0.0) {
        if (mat(1,0) == 0.0 && mat(1,1) == 0.0) {
            // handle zero matrix
            solution = {0.6, 0.8};
        } else {
            // handle matrix where top row is zeroes
            solution = {-mat(1,1), mat(1,0)};
        }
    } else {
        // handle most non-trivial matrices
        solution = {-mat(0,1), mat(0,0)};
    }

    // normalize the solution vector set earlier
    solution.normalize();

    return true;
}

#endif
