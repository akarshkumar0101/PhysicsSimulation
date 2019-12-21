//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <chrono>
#include <thread>
#include <mutex>

#include "RigidModel.h"
#include "Physics.h"


class PhysicsSimulation {

private:
    std::vector<RigidModel> mModels;
    std::mutex mut;


public:
    PhysicsSimulation(){}

    std::vector<RigidModel>& models() const{
        return (std::vector<RigidModel>&) mModels;
    }
    Force forceAt(const Point& point){
        //return 0.1f*glm::cross(point, glm::vec3(0,0,1));
        return Force(point.y, -point.x,0);
    }

    void step(const double dt){
        mut.lock();
        for(RigidModel& model: mModels){
            updateModel(model,dt);
        }
        mut.unlock();
    }

    void updateModel(RigidModel& model, const double dt){
        Force totalForce(0.0);
        Torque totalTorque(0.0);

        for(PointMass& pm: model.pointMasses()){
            Point relR = pm.pose().r();
            Point absR = model.pose().transformation() * glm::vec4(pm.pose().r(),1.0);

            Force absForceAtPM = forceAt(absR);

            totalForce = totalForce + absForceAtPM;

            Torque relTorque = glm::cross(relR, absForceAtPM);

            totalTorque = totalTorque + relTorque;
        }
        //totalTorque = glm::rotate(model.pose().orientation(),totalTorque);
//        model.pose().r() += (float)dt *(totalForce/model.mass());
        //std::cout<<model.pose().r().x<<std::endl;
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

            step(dt);
            previousFrame = thisFrame;
        }
    }

    std::thread* simulateOnThread(){
        return new std::thread(&PhysicsSimulation::simulate, this);
    }
};