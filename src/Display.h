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
#include "ModelData.h"
#include "Camera.h"
#include "Renderer.h"
#include "Simulation.h"
#include "Window.h"


class SimulationDisplay {
private:
    Shader *basicShader;
    Camera camera;
    Renderer *renderer;
    Window* window;
    PhysicsSimulation& simulation;
    std::vector<ModelData *> modelDatas;

public:
    SimulationDisplay(PhysicsSimulation& simulation):camera(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)), simulation(simulation){

        window = new Window(500,500,"new window dog");

        renderer = new Renderer(true, false);
        basicShader = new Shader("resources/shaders/basic.shader");

        initModels();

        for (RigidModel rm: simulation.models()) {
            modelDatas.push_back(new ModelData(rm));
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
        destroyModels();


        glfwTerminate();
    }
private:
    void windowEvents(){
        window->swapBuffers();
        glfwPollEvents();
        window->makeContextCurrent();
        processInputs();
    }

    void render(){
        //SET VIEW PORT
        int width, height;
        window->getFramebufferSize(width, height);

        basicShader->setUniform("projectionView", camera.computePerspectiveProjectionMatrix((float) width / height) *
                                                 camera.computeViewMatrix());

        renderer->clear(width, height, *basicShader);

        glm::mat4 model(1.0f);
        basicShader->setUniform("model", model);

        basicShader->bind();

        basicShader->setUniform("model", model);
        basicShader->setUniform("solidColor", glm::vec4(1.0, 0.0, 0.0, 1.0));

        //teapotModel->draw(*basicShader);

        for (int i = 0; i < modelDatas.size(); i++) {
            const RigidModel& model = simulation.models()[i];
            modelDatas[i]->drawModel(simulation.models()[i], *cubeModel, *basicShader);
            for(PointMass pm: model.pointMasses()){
                Point r = model.pose().transformation() * glm::vec4(pm.pose().r(),1.0);
                renderer->renderForce(simulation.forceAt(r), r, *basicShader);
            }
        }

        //Force force(10.0,0.0,0.0);
        //renderer.renderForce(force,glm::vec3(0),basicShader);
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

        camera.look(xOffset, yOffset);

        lastXPos = xPos;
        lastYPos = yPos;
    }

};
