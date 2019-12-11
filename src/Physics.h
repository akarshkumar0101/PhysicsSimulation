//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

//using Point = AKMath::Vector3D;
//using Velocity = AKMath::Vector3D;
//using Acceleration = AKMath::Vector3D;
//
//using Force = AKMath::Vector3D;
//using Torque = AKMath::Vector3D;


using Point = glm::vec3;
using Velocity = glm::vec3;
using Acceleration = glm::vec3;

using Orientation = glm::quat;
using AngularVelocity = glm::vec3;
using AngularAcceleration = glm::vec3;

using Force = glm::vec3;
using Torque = glm::vec3;

//pose variables are relative to parent
class Pose {
private:
    //location of object with respect to parent
    Point mR;
    Velocity mV;
    Acceleration mA;

    //orientation of object with respect to parent
    Orientation mOrientation;
    AngularVelocity mAngV;
    AngularAcceleration mAngA;
public:
    Pose(const Point& r, const Orientation& orientation): mR(r), mOrientation(orientation){
    }
    Pose(const Point& r): Pose(r, Orientation(1.0, glm::vec3(0.0))){
    }
    Pose(): Pose(Point(0.0)){
    }
    Point& r() const{
        return (Point&) mR;
    }
    Orientation& orientation() const{
        return (Orientation&) mOrientation;
    }

    //returns a transformation to take you from this object's coordinate system to the parent's
    glm::mat4 transformation(){
        glm::mat4 trans(1.0);
        trans = glm::translate(trans, mR);
        trans = trans * glm::toMat4(mOrientation);
        return trans;
    }
    //returns a transformation to take you from the parent's coordinate system to this object's
    glm::mat4 inverseTransformation(){
        glm::mat4 trans(1.0);
        trans = trans * glm::toMat4(glm::inverse(mOrientation));
        trans = glm::translate(trans, -mR);
        return trans;
    }
};


