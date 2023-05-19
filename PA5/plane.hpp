#ifndef PLANE_H_
#define PLANE_H_

//! A plane class with build in calculation functionality
/*!
  \file plane.hpp
  \author Jacob Hartt (jacobjhartt@gmail.com)
  \version 1.0
  \date 05-08-2023
 */

#include <eigen3/Eigen/Dense>

#include "./float_compare.hpp"

//! Class representing 3D planes in point-normal form.
class PointNormalPlane {
  public:
    //! Default construct a new Point Normal Plane object to allow it to be used
    //! in the Triangle class as a data member
    PointNormalPlane() {
        normal_vec_ = {0,0,0};
        normal_vec_tail_ = {0,0,0};
    }; // Constructor

    //! Construct a new Point Normal Plane object
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

    //! Finds the minimum distance from the point to the plane
    /*!
      \param point the point 
      \return The minimum distance from the point to the plane
     */
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

    //! Finds the closest point to another point on the plane
    /*!
      \param point the input point
      \return The closest point on the plane
     */
    Eigen::Vector3d FindClosestPoint(const Eigen::Vector3d &point) {
        Eigen::Vector3d closest_point = normal_vec_;
        
        // scale closest point
        double numerator = normal_vec_.dot(normal_vec_tail_ - point);
        double denominator = normal_vec_.dot(normal_vec_);
        closest_point *= numerator / denominator;

        // translate closest point
        closest_point += point;

        return closest_point;
    } // FindClosestPoint

    Eigen::Vector3d GetNormalVec() {
        return normal_vec_;
    } // GetNormalVec

    Eigen::Vector3d GetNormalVecTail() {
        return normal_vec_tail_;
    } // GetNormalVecTail

  private:
    //! The normal unit vector of the plane.
    Eigen::Vector3d normal_vec_;
    //! The tail point of the normal vector.
    Eigen::Vector3d normal_vec_tail_;
}; // PointNormalPlane

#endif // PLANE_H_
