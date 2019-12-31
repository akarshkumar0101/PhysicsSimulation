//
// Created by Akarsh Kumar on 11/20/19.
//

#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_glfw.h>

#include "Shader.h"
#include "ModelGraphicsData.h"
#include "Camera.h"
#include "Renderer.h"
#include "PhysicsSimulation.h"
#include "Window.h"


class SimulationDisplay {
private:
    Window* window;
    Shader *basicShader;
    Camera camera;
    Renderer *renderer;
    PhysicsSimulation& simulation;
    std::vector<ModelGraphicsData *> modelDatas;

public:
    SimulationDisplay(PhysicsSimulation& simulation):camera(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)), simulation(simulation){
        window = new Window(500,500,"new window dog");

        renderer = new Renderer(true, false);
        basicShader = new Shader("resources/shaders/basic.shader");

        CommonModels::initCommonModels();

        for (RigidBody rm: simulation.models()) {
            modelDatas.push_back(new ModelGraphicsData(rm));
        }
    }

    void startSimulationDisplay() {
        double lastTime = glfwGetTime();

        while (!window->shouldClose()) {
            double currentTime = glfwGetTime();
            double dt = currentTime - lastTime;
            lastTime = currentTime;

            render();
            simulation.step(dt);

            windowEvents();
        }

        glfwTerminate();
    }
private:
    void windowEvents(){
        window->swapBuffers();
        glfwPollEvents();
        processInputs();
    }

    void render(){
        //SET VIEW PORT
        int width, height;
        window->getFramebufferSize(width, height);
        renderer->clear(width, height, *basicShader);

        basicShader->setUniform("projectionView", camera.computePerspectiveProjectionMatrix((float) width / height) *
                                                 camera.computeViewMatrix());

        glm::mat4 model(1.0f);

        basicShader->bind();
        basicShader->setUniform("model", model);
        basicShader->setUniform("solidColor", glm::vec4(1.0, 0.0, 0.0, 1.0));

//        renderer->renderTriangleWise(*CommonModels::teapotModel,*basicShader);

        for (int i = 0; i < modelDatas.size(); i++) {
            const RigidBody& model = simulation.models()[i];

            renderer->renderRigidBody(*modelDatas[i], model, *CommonModels::cubeModel, *basicShader);
            for(PointMass pm: model.pointMasses()) {
                //Point r = model.transformation() * glm::vec4(pm.r(), 1.0);
//                renderer->renderForce(simulation.forceAt(r), r, *basicShader);
            }
        }

        for(float x=-10;x<=10;x++){
            for(float y=-10;y<=10;y++){
                Point point(x,y,0);
                Force force = 0.1f*simulation.forceAt(point);
                renderer->renderForce(force,point,*basicShader);
            }
        }
    }
    void processInputs() {
        if (window->isKeyPressed(GLFW_KEY_ESCAPE)) {
            window->setShouldClose(true);
        }

        float dt = 0.1;
        if (window->isKeyPressed(GLFW_KEY_W)) {
            camera.move(FORWARD, dt);
        }
        if (window->isKeyPressed(GLFW_KEY_S)) {
            camera.move(BACKWARD, dt);
        }
        if (window->isKeyPressed(GLFW_KEY_A)) {
            camera.move(LEFTWARD, dt);
        }
        if (window->isKeyPressed(GLFW_KEY_D)) {
            camera.move(RIGHTWARD, dt);
        }
        if (window->isKeyPressed(GLFW_KEY_SPACE)) {
            camera.move(UPWARD, dt);
        }
        if (window->isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            camera.move(DOWNWARD, dt);
        }

        double xPos, yPos;
        window->getCursorPosition(xPos,yPos);

        static double lastXPos = xPos, lastYPos = yPos;

        double xOffset = xPos - lastXPos;
        double yOffset = yPos - lastYPos;

//        camera.look(xOffset, yOffset);

        lastXPos = xPos;
        lastYPos = yPos;
    }

};
