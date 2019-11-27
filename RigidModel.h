//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <iostream>
#include <vector>

#include "math/Vector.h"
#include "math/Quaternion.h"


class PointMass {
private:
    double mMass;
    AKMath::Vector3D mR;
public:
    PointMass(const double mass){
        this->mMass = mass;
    }
    PointMass(const double mass, const AKMath::Vector3D& r) : PointMass(mass){
        this->mR = r;
    }

    inline double& mass() const {
        return (double&) mMass;
    }
    inline AKMath::Vector3D& r() const {
        return (AKMath::Vector3D&) mR;
    }

    friend std::ostream &operator<<(std::ostream &out, const PointMass &pointMass);
};

std::ostream &operator<<(std::ostream &out, const PointMass &pointMass){
    out<<"PointMass: ";
    out << "m = " << pointMass.mMass << ", ";
    out<<"r = "<<pointMass.mR;
    return out;
}

class RigidModel {
private:
    std::vector<PointMass> mPointMasses;
    AKMath::Vector3D mR;
    AKMath::Quaternion mOrientation;

    double mM = 0;
    double mI = 0;

    bool establishedModel = false;
public:

    RigidModel(): mOrientation(AKMath::i, 0.0){
    }

    template <size_t M>
    RigidModel(const std::array<PointMass, M> &stdArray): RigidModel() {
        mPointMasses.insert(mPointMasses.begin(), std::begin(stdArray), std::end(stdArray));
        establishModel();
    }

    void establishModel(){
        establishedModel = true;
        //center of mass;
        AKMath::Vector3D cm(0.0);

        for(const PointMass& pm: mPointMasses){
            AKMath::Vector3D relR = pm.r();

            double m = pm.mass();
            mM += m;

            relR *= m;
            cm+=relR;
        }

        cm/=mM;

        for(PointMass& pm: mPointMasses){
            AKMath::Vector3D& pmR = pm.r();
            double& m = pm.mass();

            pmR -= cm;

            mI += m * magnitudeSqr(pmR);

        }
        mR += cm;
    }


    std::vector<PointMass>& pointMasses() const{
        return (std::vector<PointMass>&) mPointMasses;
    }
    AKMath::Vector3D& r() const{
        return (AKMath::Vector3D&) mR;
    }
    AKMath::Quaternion& orientation() const{
        return (AKMath::Quaternion&) mOrientation;
    }

};


std::ostream & operator<<(std::ostream &out, const RigidModel &rigidModel) {
    out << "RigidModel: ";
    out << '[';
    int N = rigidModel.pointMasses().size();
    for (int i = 0; i < N; i++) {
        out<<rigidModel.pointMasses()[i];
        if(i<N-1){
            out<<", "<<std::endl;
        }
    }
    out << ']';
    return out;
}


using PointMass3D = PointMass;
using RigidModel3D = RigidModel;