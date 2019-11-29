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
    std::vector<RigidModel> mModels;
    std::mutex mut;


    Force forceAt(const Point& point){
        return Force();
    }

public:
    PhysicsSimulation(){}

    std::vector<RigidModel>& models() const{
        return (std::vector<RigidModel>&) mModels;
    }

    void update(const double dt){
        for(RigidModel& model: mModels){
            updateModel(model,dt);
        }
    }

    void updateModel(RigidModel& model, const double dt){
        Force totalForce(0.0);
        Torque totalTorque(0.0);

        for(PointMass& pm: model.pointMasses()){
            Point relR = pm.r();
            Point absR = relR;

            Force absForceAtPM = forceAt(absR);

            totalForce = totalForce + absForceAtPM;

            Torque relTorque = glm::cross(relR, absForceAtPM);

            totalTorque = totalTorque + relTorque;
        }
        totalTorque = glm::rotate(model.orientation(),totalTorque);

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