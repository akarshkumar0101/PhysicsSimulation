//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Physics.h"


//pose is relative to parent
class Pose {
private:
    //location of object with respect to parent
    Point mR;
    //orientation of object with respect to parent
    Orientation mOrientation;
public:
    Pose(Point r, Orientation orientation): mR(r), mOrientation(orientation){
    }
    Pose(Point r): mR(r), mOrientation(1.0, glm::vec3(0.0)){
    }
    Pose(): mR(0.0), mOrientation(1.0, glm::vec3(0.0)){
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

class PointMass {
private:
    float mMass;
    //pose relative to rigid body
    Pose mPose;
public:
    PointMass(const float mass):mMass(mass){
    }
    PointMass(const float mass, const Point& r) :mMass(mass), mPose(r){
    }
    float& mass() const {
        return (float&) mMass;
    }
    Pose& pose() const {
        return (Pose&) mPose;
    }
};


class RigidModel {
private:
    std::vector<PointMass> mPointMasses;
    //pose relative to world
    Pose mPose;

    //total mass and moment of inertia
    float mM = 0;
    float mI = 0;

    bool establishedModel = false;
public:

    RigidModel(){}

    template <size_t M>
    RigidModel(const std::array<PointMass, M> &stdArray): RigidModel() {
        mPointMasses.insert(mPointMasses.begin(), std::begin(stdArray), std::end(stdArray));
        establishModel();
    }

    void establishModel(){
        establishedModel = true;
        //center of mass;
        Point cm(0.0);

        for(const PointMass& pm: mPointMasses){
            mM += pm.mass();
            cm += (pm.mass()*pm.pose().r());
        }
        cm/=mM;
        //found center of mass now

        for(PointMass& pm: mPointMasses){
            float d = glm::length(pm.pose().r());
            mI += pm.mass() * d*d;
        }
    }

    std::vector<PointMass>& pointMasses() const{
        return (std::vector<PointMass>&) mPointMasses;
    }

    Pose& pose() const{
        return (Pose&) mPose;
    }
    float& mass() const{
        return (float&)mM;
    }
};

