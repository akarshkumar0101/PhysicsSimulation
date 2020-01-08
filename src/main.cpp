#include <iostream>

#include "SimulationDisplay.h"
#include "RigidBody.h"
#include "PhysicsSimulation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int simulation(){
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

void testLines(){
    Window window(900,900, "hey window");
    VertexBufferLayout::initCommonLayouts();
    CommonModels::initCommonModels();

    Shader shader("./resources/shaders/triangle.shader");

    std::vector<float> vertices({0,0,0});
    std::vector<unsigned int> indices;
    for(float deg=0;deg<2*M_PI;deg+=0.2){
        vertices.insert(vertices.end(),{(float) cos(deg),(float) sin(deg),0.0f});
    }
    for(unsigned int i=1; i <vertices.size();i++){
        indices.insert(indices.end(), {0, i});
    }


    GraphicsData gd(vertices, indices);

    glClearColor(0.0,0.0,0.0,1.0);
    while(!window.shouldClose()){
        glClear(GL_COLOR_BUFFER_BIT);
        shader.bind();

        gd.bind();

        glDrawElements(GL_LINES, gd.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);

        window.swapBuffers();
        window.pollEvents();
    }
}
int main() {
//    testingOpenGL();
//    try{
        simulation();
//    }catch(std::string exp){
//        std::cout<<exp<<std::endl;
//    }

//    Window window(900,900, "dog window");
//
//    while(!window.shouldClose()){
//
//        window.swapBuffers();
//        window.pollEvents();
//    }
    return 0;
}