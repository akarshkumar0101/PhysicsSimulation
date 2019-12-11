//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include "ModelData.h"
#include "Shader.h"


float axisVertices[]={
        -100.0,0.0,0.0,
        +100.0,0.0,0.0,
        0.0,-100.0,0.0,
        0.0,+100.0,0.0,
        0.0,0.0,-100.0,
        0.0,0.0,+100.0,
};
unsigned int axisIndices[3][2]={
        {0,1},
        {2,3},
        {4,5},
};

unsigned int axisBuffer;
unsigned int* axisIndicesBuffers = new unsigned int[3];

void initAxis(){
    glGenBuffers(1, &axisBuffer);
    glGenBuffers(3, axisIndicesBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices,GL_STATIC_DRAW);

    for(int i=0;i<3;i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisIndicesBuffers[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axisIndices[i]), (void*) axisIndices[i],GL_STATIC_DRAW);
    }
}

class Renderer{
private:
    void drawAxis(const Shader& shaderProgram){
        glBindBuffer(GL_ARRAY_BUFFER, axisBuffer);

        int vPosLocation = shaderProgram.getAttribLocation("vPos");
        glVertexAttribPointer(vPosLocation,3,GL_FLOAT,GL_FALSE,3* sizeof(float), (void*)0);
        glEnableVertexAttribArray(vPosLocation);

        for(int i=0;i<3;i++){
            glm::vec4 color(0.0);
            color[i] = 1.0;
            color[3] = 1.0;
            shaderProgram.setUniform("solidColor", color);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisIndicesBuffers[i]);
            glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, nullptr);
        }
    }
    bool mAxis;
    bool mWireFrame;

public:
    Renderer(bool axis, bool wireFrame): mAxis(axis), mWireFrame(wireFrame) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        if (wireFrame) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (mAxis) {
            initAxis();
        }
    }

    void clear(int width, int height, const Shader& shader){
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        if (mAxis) {
            glm::mat4 model(1.0f);
            shader.setUniform("model", model);
            drawAxis(shader);
        }
    }


    void render(const ModelData& modelData, const Shader& shader){

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
    void renderForce(const Force& force, const Point& position, const Shader& shader){
        float forceMag = glm::length(force);
        glm::mat4 model(1.0);

//        float yaw = glm::atan(force.y,force.x);
//        float pitch = glm::asin(force.z);
//        float roll = 0;
//        glm::quat q(glm::vec3(yaw,pitch,roll));
//        glm::mat4 rotate = glm::toMat4(q);


        model = glm::translate(model,position);


        float scaleXY = std::min(1.0f,forceMag);
        glm::mat4 scaleZ = glm::scale(glm::mat4(1.0),glm::vec3(scaleXY,scaleXY,forceMag));
        glm::mat4 toZ = glm::rotate(glm::mat4(1.0), (float) M_PI/2, glm::vec3(0.0,1.0,0.0));

        glm::mat4 rotate = glm::lookAt(glm::vec3(0.0),force,perpendicular(force));
        rotate = glm::inverse(rotate);
        model = model*rotate*scaleZ*toZ;

        //model = rotate*model;






        shader.setUniform("model", model);

        arrowModel->draw(shader);
    }

};

