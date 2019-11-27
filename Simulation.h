//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <chrono>
#include <thread>
#include <mutex>

#include "math/Vector.h"
#include "RigidModel.h"
#include "Physics.h"


class PhysicsSimulation {

private:
    std::vector<RigidModel3D> mModels;
    std::mutex mut;


    Force3D forceAt(const Point3D& point){
        return Force3D();
    }

public:
    PhysicsSimulation(){}

    std::vector<RigidModel3D>& models() const{
        return (std::vector<RigidModel3D>&) mModels;
    }

    void update(const double dt){
        for(RigidModel3D& model: mModels){
            updateModel(model,dt);
        }
    }

    void updateModel(RigidModel3D& model, const double dt){
        Force3D totalForce(0.0);
        Torque3D totalTorque(0.0);

        for(PointMass& pm: model.pointMasses()){
            AKMath::Vector3D relR = pm.r();
            AKMath::Vector3D absR = relR;

            Force3D absForceAtPM = forceAt(absR);

            totalForce = totalForce + absForceAtPM;

            Torque3D relTorque = cross(relR, absForceAtPM);

            totalTorque = totalTorque + relTorque;
        }
        totalTorque = rotate(totalTorque, model.orientation());

    }

    void simulate(){
        using namespace std::chrono;
        high_resolution_clock::time_point start = high_resolution_clock::now();
        high_resolution_clock::time_point previousFrame = start;
        while(true){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            high_resolution_clock::time_point thisFrame = high_resolution_clock::now();
            duration<double> time_span = duration_cast<duration<double>>(thisFrame - previousFrame);
            double dt = time_span.count();
            std::cout << dt << " seconds\n";

            update(dt);
            previousFrame = thisFrame;
        }
    }

    std::thread* simulateOnThread(){
        return new std::thread(&PhysicsSimulation::simulate, this);
    }
};