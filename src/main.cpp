#include <iostream>

#include "SimulationDisplay.h"
#include "RigidBody.h"
#include "PhysicsSimulation.h"
#include "TrianglePane.h"
#include "GridPane.h"
#include "DrawingPane.h"

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

void renderFunction(Graphics& graphics){
    graphics.setColor(Color::GRAY);
    graphics.clear();

    graphics.setColor(Color(1.0,0.0,0.0,1.0));
    graphics.drawPoint(0,0);
    graphics.setColor(Color(1.0,1.0,0.0,1.0));
    graphics.drawLine(-0.5,0,0.5,0.0);
    graphics.setColor(Color(1.0,0.0,1.0,1.0));
    graphics.drawLine(0,-0.5,0.0,0.5);
    graphics.setColor(Color(0.0,0.0,1.0,1.0));
    graphics.fillTriangle(0.5,0,0,-0.5,1,-0.5);
    graphics.setColor(Color(0.0,1.0,0.0,1.0));
    graphics.fillCircle(0.5,0.5,0.5);


}

void testWindowAndNode(){
    auto window = std::make_shared<Window>(900,900, "hey window");

    auto gridNode = std::make_shared<GridPane> (window, std::pair<unsigned int, unsigned int>({3, 3}));
    auto gridNode2 = std::make_shared<GridPane> (window, std::pair<unsigned int, unsigned int>({3, 3}));

    auto drawingNode = std::make_shared<DrawingPane> (window, renderFunction);
    auto drawingNode2 = std::make_shared<DrawingPane> (window, renderFunction);

    window->setRoot(gridNode);

    for(int x=0;x<3; x++){
        for(int y=0; y<3; y++){
            auto triangleNode = std::make_shared<TrianglePane>(window);
            gridNode->setChild(triangleNode, {x,y});
        }
    }
    gridNode->setChild(gridNode2, {1,1});
    gridNode->setChild(drawingNode, {0,0});
    for(int x=0;x<3; x++){
        for(int y=0; y<3; y++){
            auto triangleNode = std::make_shared<TrianglePane>(window);
            gridNode2->setChild(triangleNode, {x,y});
        }
    }
    gridNode2->setChild(drawingNode2, {1,0});

    glClearColor(0.0,0.0,0.0,1.0);
    while(!window->shouldClose()){
        glClear(GL_COLOR_BUFFER_BIT);

//        triangleNode.render();
        window->render();

        window->swapBuffers();
        window->pollEvents();
    }



}
int main() {
    testWindowAndNode();
//    testingOpenGL();
//    try{
//        simulation();
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