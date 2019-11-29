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

#include "GLFWWrapper.h"
#include "Shader.h"
#include "ModelData.h"
#include "Camera.h"
#include "Renderer.h"



void processInputs(GLFWwindow* window, Camera& camera){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    float dt = 0.1;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.move(FORWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.move(BACKWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.move(LEFTWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.move(RIGHTWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera.move(UPWARD, dt);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.move(DOWNWARD, dt);
    }

    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    static double lastXPos = xPos, lastYPos = yPos;

    double xOffset = xPos- lastXPos;
    double yOffset = yPos- lastYPos;

    camera.look(xOffset, yOffset);

    lastXPos = xPos;
    lastYPos = yPos;
}



void testGL(){
    myGLFWInit();
    GLFWwindow* window = myGLFWCreateWindow();

    Shader basicShader("resources/shaders/basic.shader");
    ModelData teapotModel("resources/models/teapot.obj");
    ModelData squareModel("resources/models/square.obj");
    ModelData cubeModel("resources/models/cube.obj");
    ModelData arrowModel("resources/models/arrow.obj");

    Camera camera(glm::vec3(0.0,0.0,15.0),glm::vec3(0.0,0.0,-1.0),glm::vec3(0.0,1.0,0.0));

    Renderer renderer(true, false);


    double lastTime = glfwGetTime();
    basicShader.bind();
    while(!glfwWindowShouldClose(window)) {
        //SET VIEW PORT
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        renderer.clear(width, height, basicShader);

        //TIME
        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime;
        lastTime = currentTime;

        //set uniforms
        basicShader.setUniform("view", camera.computeViewMatrix());
        basicShader.setUniform("projection", camera.computePerspectiveProjectionMatrix((float)width/height));

        glm::mat4 model(1.0f);
        //model = glm::rotate(model, 100*glm::radians((float)currentTime), glm::vec3(0.0,1.0,0.0));

        basicShader.setUniform("model", model);
        basicShader.setUniform("solidColor", glm::vec4(1.0,0.0,0.0,1.0));

        //teapotModel.draw(basicShader);
        //cubeModel.draw(basicShader);
        arrowModel.draw(basicShader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        processInputs(window, camera);
    }

    glfwTerminate();
}


