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

#include "Shader.h"



using namespace std;

float triangleVertices[] = {
        //location then color
        -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

float boxVerticesColors[36*6];

glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
};

void createVertex(glm::vec3 v, int vertex){
    float* ptr = &boxVerticesColors[vertex*6];
    //write to this
    ptr[0] = v.x/3;
    ptr[1] = v.y/3;
    ptr[2] = v.z/3;

    for(int i=3;i<=6;i++){
        ptr[i] = ((double) rand() / (RAND_MAX));
    }
}
void createRandomColoredBox(){
    int vertex=0;

    for(int i=0;i<3;i++) {
        glm::vec3 center(0.0);
        for (int j = -1; j <= 1; j+=2) {
            //per face
            center[i] = (float) j;
            float p1= -1, p2= -1;
            for(int k=0;k<3;k++){
                if(i==k){
                    continue;
                }
                if(p1== -1){
                    p1 = k;
                }
                else{
                    p2 =k;
                }
            }
            //now vary p1, p2 from -1 to 1
            glm::vec3 vertices[4];
            int vi=0;
            for(int v1=-1;v1<=1;v1+=2){
                for(int v2=-1;v2<=1;v2+=2){
                    glm::vec3 v = center;
                    v[p1] = (float) v1;
                    v[p2] = (float) v2;
                    vertices[vi++] = v;
                }
            }
            createVertex(vertices[0], vertex++);
            createVertex(vertices[1], vertex++);
            createVertex(vertices[2], vertex++);
            createVertex(vertices[1], vertex++);
            createVertex(vertices[2], vertex++);
            createVertex(vertices[3], vertex++);

        }
    }
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}
void errorCallback(int error, const char* description){
    cerr<<"GLFW Error "<<error<<": "<<description<<endl;
}


void processInputs(GLFWwindow* window){
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window,GLFW_KEY_SPACE) == GLFW_PRESS){
        cout<<"space"<<endl;
    }
}

void testGL(){
    createRandomColoredBox();

    glfwSetErrorCallback(errorCallback);
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(500,500,"window title",nullptr,nullptr);
    if(window==nullptr){
        cout<<"failed to create window"<<endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    int gladLoadSucess = gladLoadGL();

    if(gladLoadSucess==0){
        cout<<"failed to load with glad"<<endl;
        glfwTerminate();
        return;
    }
    frameBufferSizeCallback(window, 500,500);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    cout<< glGetString(GL_VERSION)<<endl;
    cout<< glGetString ( GL_SHADING_LANGUAGE_VERSION )<<endl;

    ShaderProgram sp("basic.shader");
    ShaderProgram sp2("lol.shader");

    //vertex buffer object
    unsigned int VBOid;
    glGenBuffers(1, &VBOid);

    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);


    unsigned int boxVBO;
    glGenBuffers(1, &boxVBO);

    glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVerticesColors), boxVerticesColors, GL_STATIC_DRAW);


    int vPosLocation = glGetAttribLocation(sp.shaderProgram(),"vPos");
    int vColLocation = glGetAttribLocation(sp.shaderProgram(),"vCol");
    int sinValLocation = glGetUniformLocation(sp.shaderProgram(), "sinVal");
    int rotMatLocation = glGetUniformLocation(sp.shaderProgram(), "rotMat");



    vPosLocation = glGetAttribLocation(sp2.shaderProgram(),"vPos");
    vColLocation = glGetAttribLocation(sp2.shaderProgram(),"vCol");



    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        float time = glfwGetTime();
        float greenVal = (sin(time)+1.0f)/2.0f;

        glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
        //model = glm::translate(model,glm::vec3(0.0,0.0,0.0));
        glm::vec4 eye (10.0,0.0,0.0,1.0);
        glm::mat4 eyeMat = glm::rotate(glm::mat4(1.0),(float)time*glm::radians(50.0f),glm::vec3(0.0f,1.0f,0.0f));
        eye = eyeMat*eye;
        glm::vec3 eye3 = glm::vec3(eye.x,eye.y,eye.z);
        view = glm::lookAt(eye3,glm::vec3(0.0f),glm::vec3(0.0f,1.0,0.0));
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

        projection = glm::perspective(glm::radians(45.0f), (float) width / height, 0.1f, 100.0f);

        sp.glUseThisProgram();

//        int vertexColorLocation = glGetUniformLocation(sp.shaderProgram(), "ourColor");
//        glUniform4f(vertexColorLocation,0.0f, greenVal, 0.0f, 1.0f);

//        glBindBuffer(GL_ARRAY_BUFFER,VBOid);
//        glVertexAttribPointer(vPosLocation,3,GL_FLOAT,GL_FALSE,6*sizeof(float), (void*) 0);
//        glEnableVertexAttribArray(vPosLocation);
//        glVertexAttribPointer(vColLocation,3,GL_FLOAT,GL_FALSE,6*sizeof(float), (void*) (3*sizeof(float)));
//        glEnableVertexAttribArray(vColLocation);
//
//        glUniform1f(sinValLocation,greenVal);


//        mat4x4 rotMat;
//        mat4x4_identity(rotMat);
//        mat4x4_rotate_Z(rotMat, rotMat, time);
//
//        glUniformMatrix4fv(rotMatLocation, 1, GL_FALSE, (const GLfloat*) rotMat);


        sp2.glUseThisProgram();

        glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
        glVertexAttribPointer(vPosLocation,3,GL_FLOAT,GL_FALSE,6* sizeof(float), (void*)0);
        glEnableVertexAttribArray(vPosLocation);
        glVertexAttribPointer(vColLocation,3,GL_FLOAT,GL_FALSE,6* sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(vColLocation);


        model = glm::rotate(model,(float)time * glm::radians(50.0f),glm::vec3(1.0,1.0,1.0));

        int modelLoc = glGetUniformLocation(sp2.shaderProgram(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(sp2.shaderProgram(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(sp2.shaderProgram(), "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //glBindVertexArray(VAOid);




        //render triangles, start at 0 index, 3 vertices to render
//        glDrawArrays(GL_TRIANGLES,0,3);
//        glDrawArrays(GL_TRIANGLES,0,36);

        for(int i=0;i<sizeof(cubePositions)/sizeof(glm::vec3);i++){
            model = glm::mat4(1.0);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model,(float)i*time * glm::radians(50.0f),glm::vec3(1.0,1.0,1.0));

            int modelLoc = glGetUniformLocation(sp2.shaderProgram(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            int viewLoc = glGetUniformLocation(sp2.shaderProgram(), "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            int projLoc = glGetUniformLocation(sp2.shaderProgram(), "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            //glDrawArrays(GL_LINES,0,36);
        }



        glfwSwapBuffers(window);
        glfwPollEvents();

        processInputs(window);
    }

    glfwTerminate();
}
