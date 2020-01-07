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

void testingOpenGL(){
    Window window(900,900, "hey window");
    float vertices[] = {0,0,0,1,1,0,1,0,0};

    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout vbl;
    vbl.addElement<float>(3,"vPos");


    Shader shader("./resources/shaders/triangle.shader");

    glClearColor(0.0,0.0,0.0,1.0);
    while(!window.shouldClose()){
        glClear(GL_COLOR_BUFFER_BIT);
        shader.bind();
        vb.bind();
        vbl.bindVertexAttributesToShader(shader);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.swapBuffers();
        window.pollEvents();
    }
}
int main() {
//    testingOpenGL();
//    if(true) return 0 ;
    try{
        simulation();
    }catch(std::string exp){
        std::cout<<exp<<std::endl;
    }

//    Window window(900,900, "dog window");
//
//    while(!window.shouldClose()){
//
//        window.swapBuffers();
//        window.pollEvents();
//    }
    return 0;
}