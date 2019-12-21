#include <iostream>

#include "SimulationDisplay.h"
#include "RigidModel.h"
#include "PhysicsSimulation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
//    Pose pose(Point(1.0,2.0,3.0), Orientation(glm::vec3(8.0,2.0,3.0)));

    RigidModel model;
    model.pointMasses().push_back(PointMass(1.0,Point(1.0,0.0,0.0)));
    model.pointMasses().push_back(PointMass(1.0,Point(-1.0,0.0,0.0)));
    model.pointMasses().push_back(PointMass(1.0,Point(0.0,1.0,0.0)));
    model.pointMasses().push_back(PointMass(1.0,Point(0.0,-1.0,0.0)));
    model.pointMasses().push_back(PointMass(1.0,Point(0.0,3.0,1.0)));
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