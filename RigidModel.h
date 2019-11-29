//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


class PointMass {
private:
    float mMass;
    glm::vec3 mR;
public:
    PointMass(const float mass){
        this->mMass = mass;
    }
    PointMass(const float mass, const glm::vec3& r) : PointMass(mass){
        this->mR = r;
    }
    inline float& mass() const {
        return (float&) mMass;
    }
    inline glm::vec3& r() const {
        return (glm::vec3&) mR;
    }
};


class RigidModel {
private:
    std::vector<PointMass> mPointMasses;
    glm::vec3 mR;
    glm::quat mOrientation;

    float mM = 0;
    float mI = 0;

    bool establishedModel = false;
public:

    RigidModel(): mOrientation(1.0,0.0,0.0,0.0){
    }

    template <size_t M>
    RigidModel(const std::array<PointMass, M> &stdArray): RigidModel() {
        mPointMasses.insert(mPointMasses.begin(), std::begin(stdArray), std::end(stdArray));
        establishModel();
    }

    void establishModel(){
        establishedModel = true;
        //center of mass;
        glm::vec3 cm(0.0);

        for(const PointMass& pm: mPointMasses){
            glm::vec3 relR = pm.r();

            float m = pm.mass();
            mM += m;

            relR *= m;
            cm+=relR;
        }

        cm/=mM;

        for(PointMass& pm: mPointMasses){
            glm::vec3& pmR = pm.r();
            float& m = pm.mass();

            pmR -= cm;

            float mag = glm::length(pmR);
            mI += m * mag*mag;

        }
        mR += cm;
    }


    std::vector<PointMass>& pointMasses() const{
        return (std::vector<PointMass>&) mPointMasses;
    }
    glm::vec3& r() const{
        return (glm::vec3&) mR;
    }
    glm::quat& orientation() const{
        return (glm::quat&) mOrientation;
    }

};

