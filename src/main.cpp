#include <iostream>

#include "SimulationDisplay.h"
#include "RigidBody.h"
#include "PhysicsSimulation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {

    RigidBody model;
    model.pointMasses().emplace_back(1.0,Point(0.0,0.0,1.0));
    model.pointMasses().emplace_back(1.0,Point(0.0,0.0,-1.0));
    model.pointMasses().emplace_back(1.0,Point(0.0,1.0,0.0));
    model.pointMasses().emplace_back(1.0,Point(0.0,-1.0,0.0));
//    model.pointMasses().emplace_back(1.0,Point(1.0,0.0,0.0));
    model.establishModel();

    PhysicsSimulation simulation;
    simulation.models().push_back(model);

    //std::thread* thread = simulation.simulateOnThread();

    SimulationDisplay display(simulation);
    display.startSimulationDisplay();

    //delete thread;




    std::cout<<"Exiting program successfully"<<std::endl;

    return 0;
}