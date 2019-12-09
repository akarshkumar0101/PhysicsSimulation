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

#include "GLFWWrapper.h"
#include "Shader.h"
#include "ModelData.h"
#include "Camera.h"
#include "Renderer.h"
#include "Simulation.h"

class SimulationDisplay {
private:
    Shader *basicShader;
    Camera camera;
    Renderer *renderer;
    GLFWwindow *window;
    PhysicsSimulation& simulation;
    std::vector<ModelData *> modelDatas;

public:
    SimulationDisplay(PhysicsSimulation& simulation):camera(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)), simulation(simulation){
        myGLFWInit();
        window = myGLFWCreateWindow();

        renderer = new Renderer(true, false);
        basicShader = new Shader("resources/shaders/basic.shader");

        initModels();

        for (RigidModel rm: simulation.models()) {
            modelDatas.push_back(new ModelData(rm));
        }
    }

    void startSimulationDisplay() {
        double lastTime = glfwGetTime();

        while (!glfwWindowShouldClose(window)) {
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
        glfwSwapBuffers(window);
        glfwPollEvents();

        processInputs(window, camera);
    }

    void render(){
        //SET VIEW PORT
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

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
    void processInputs(GLFWwindow *window, Camera &camera) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        float dt = 0.1;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.move(FORWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.move(BACKWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.move(LEFTWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.move(RIGHTWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.move(UPWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.move(DOWNWARD, dt);
        }

        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        static double lastXPos = xPos, lastYPos = yPos;

        double xOffset = xPos - lastXPos;
        double yOffset = yPos - lastYPos;

        camera.look(xOffset, yOffset);

        lastXPos = xPos;
        lastYPos = yPos;
    }

};
