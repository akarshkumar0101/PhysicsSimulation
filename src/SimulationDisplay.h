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
    std::vector<GraphicsData *> modelDatas;

    GraphicsData* getGDForRB(const RigidBody& rb){
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        for (int i = 0; i < rb.pointMasses().size(); i++) {
            Point r = rb.pointMasses()[i].r();
            vertices.push_back(r.x);
            vertices.push_back(r.y);
            vertices.push_back(r.z);
            for (int j = i + 1; j < rb.pointMasses().size(); j++) {
                Point r2 = rb.pointMasses()[j].r();
                indices.push_back(i);
                indices.push_back(j);
            }
        }
        GraphicsData* gd = new GraphicsData(vertices, indices);
        return gd;
    }

public:
    SimulationDisplay(PhysicsSimulation& simulation):camera(glm::vec3(0.0, 0.0, 15.0), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)), simulation(simulation){
        window = new Window(500,500,"new window dog");

        renderer = new Renderer(true, false);
        basicShader = new Shader("resources/shaders/basic.shader");



        for (RigidBody& rm: simulation.models()) {
            modelDatas.push_back(getGDForRB(rm));
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

        glm::mat4 projectionView = camera.computePerspectiveProjectionMatrix((float) width / height) * camera.computeViewMatrix();
        basicShader->setUniform("projectionView", projectionView);


        renderer->clear(width, height);
        renderer->renderAxis(projectionView);

        glm::mat4 model(1.0f);

        basicShader->bind();
        basicShader->setUniform("model", model);
        basicShader->setUniform("solidColor", glm::vec4(1.0, 0.0, 0.0, 1.0));

//        renderer->renderTriangleWise(*CommonModels::teapotModel,*basicShader);

        for (int i = 0; i < modelDatas.size(); i++) {
            const RigidBody& model = simulation.models()[i];

            renderRigidBody(*modelDatas[i], model, *CommonModels::cubeModel(), *basicShader);
            for(PointMass pm: model.pointMasses()) {
                //Point r = model.transformation() * glm::vec4(pm.r(), 1.0);
//                renderer->renderForce(simulation.forceAt(r), r, *basicShader);
            }
        }

        for(float x=-10;x<=10;x++){
            for(float y=-10;y<=10;y++){
                Point point(x,y,0);
                Force force = 0.1f*simulation.forceAt(point);
                renderForce(force,point,*basicShader);
            }
        }
    }
    void renderRigidBody(const GraphicsData& modelData, const RigidBody &rigidBody, const GraphicsData &jointModel, const Shader &shader) {
    shader.setUniform("solidColor", glm::vec4(0.0, 0.0, 0.0, 1.0));

    glm::mat4 modelMatrix = rigidBody.transformation();
    shader.setUniform("model", modelMatrix);

    renderer->renderLineWise(modelData, shader);

    for (PointMass pm: rigidBody.pointMasses()) {
        glm::mat4 pointTransform = pm.transformation();
        pointTransform = glm::scale(pointTransform, glm::vec3(0.1));
        shader.setUniform("model", modelMatrix * pointTransform);
        renderer->renderTriangleWise(jointModel, shader);
    }
}

    glm::vec3 perpendicular(glm::vec3 vec){
        glm::vec3 y(0,1,0);
        glm::vec3 x(1,0,0);

        glm::vec3 c1 = glm::cross(vec,y);
        if(glm::length(c1)<.1){
            glm::vec3 c2 = glm::cross(vec,x);
            return c2;
        }
        else{
            return c1;
        }
    }
void renderForce(const Force &force, const Point &position, const Shader &shader){
    float forceMag = glm::length(force);
    glm::mat4 model(1.0);

    model = glm::translate(model,position);


    float scaleXY = std::min(1.0f,forceMag);
    glm::mat4 scaleZ = glm::scale(glm::mat4(1.0),glm::vec3(scaleXY,scaleXY,forceMag));
    glm::mat4 toZ = glm::rotate(glm::mat4(1.0), (float) M_PI/2, glm::vec3(0.0,1.0,0.0));

    glm::mat4 rotate = glm::lookAt(glm::vec3(0.0),force,perpendicular(force));
    rotate = glm::inverse(rotate);
    model = model*rotate*scaleZ*toZ;

    shader.setUniform("model", model);


    renderer->renderTriangleWise(*CommonModels::arrowModel(), shader);
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
        window->getCursorPosition(xPos, yPos);

        static double lastXPos = xPos, lastYPos = yPos;

        double xOffset = xPos - lastXPos;
        double yOffset = yPos - lastYPos;

//        if (window->cursorIsInWindow()) {
            camera.look(xOffset, yOffset);
//        }

        lastXPos = xPos;
        lastYPos = yPos;
    }

};
