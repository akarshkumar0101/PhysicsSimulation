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

class PhysicsSimulation;


class PointMass {
private:
    float mMass;
    // mR relative to rigid body
    Point mR;
public:
    PointMass(const float mass):mMass(mass){
    }
    PointMass(const float mass, const Point& r) :mMass(mass), mR(r){
    }
    float& mass() const {
        return (float&) mMass;
    }
    Point& r() const{
        return (Point&) mR;
    }
    //returns a transformation to take you from this object's coordinate system to the parent's
    glm::mat4 transformation(){
        glm::mat4 trans(1.0);
        trans = glm::translate(trans, mR);
        //trans = trans * glm::toMat4(mPose.orientation());
        return trans;
    }
};


class RigidBody {
private:
    std::vector<PointMass> mPointMasses;

    /* Constant quantities */
    float mMass;
    glm::mat3 mIBody;
    glm::mat3 mIBodyInv;

    /* State variables */
    glm::vec3 mX;
//    glm::mat3 mR;
    glm::quat mQ;
    glm::vec3 mP;
    glm::vec3 mL;

    /* Derived quantities */
    glm::mat3 mR;

    glm::mat3 mI;
    glm::mat3 mIInv;
    glm::vec3 mV;
    glm::vec3 mW;

    /* Computed quantities */
    glm::vec3 mForce;
    glm::vec3 mTorque;


    bool establishedModel = false;

    void computeDerivedQuantities();


public:

    RigidBody();

    template <size_t M>
    RigidBody(const std::array<PointMass, M> &stdArray);

    void establishModel();

    void update(float dt, const PhysicsSimulation& simulation);
    void update(float dt, Force force, Torque torque);

    std::vector<PointMass>& pointMasses() const;

    glm::mat4 transformation() const {
        glm::mat4 trans(1.0);
        //first rotate then translate
        trans = glm::translate(trans, mX) * glm::mat4(mR);
        return trans;
    }

    float& mass() const{
        return (float&)mMass;
    }
};



