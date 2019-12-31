//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <chrono>
#include <thread>
#include <mutex>

#include "RigidBody.h"
#include "Physics.h"

#include <glfw/glfw3.h>


class PhysicsSimulation {

private:
    std::vector<RigidBody> mModels;
    std::mutex mut;


public:
    PhysicsSimulation(){}

    std::vector<RigidBody>& models() const{
        return (std::vector<RigidBody>&) mModels;
    }
    Force forceAt(const Point& point) const {
        //return 0.1f*glm::cross(point, glm::vec3(0,0,1));
//        return Force(point.y, -point.x, 10*sin(glfwGetTime()));
        //return 0.1f*Force(point.y, -point.x, 0);
//        return Force(sin(glfwGetTime()), 0, 0);
        return Force(-point.x,0,0);
//        return -glm::normalize(point)*glm::dot(point,point);
    }

    void step(const double dt){
        mut.lock();
        for(RigidBody& model: mModels){
            updateModel(model,dt);
        }
        mut.unlock();
    }

    void updateModel(RigidBody& model, const double dt){
        Force totalForce(0.0);
        Torque totalTorque(0.0);

        model.update(dt, *this);
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