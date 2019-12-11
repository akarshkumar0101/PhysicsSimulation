//
// Created by Akarsh Kumar on 11/27/19.
//

#pragma once

#include <iostream>


#include <glad/glad.h>
#include <GLFW/glfw3.h>


void errorCallback(int error, const char* description){
    std::cerr<<"GLFW Error "<<error<<": "<<description<<std::endl;
}

void myGLFWInit(){
    glfwSetErrorCallback(errorCallback);
    glfwInit();
}

GLFWwindow* myGLFWCreateWindow(){
    GLFWwindow* window = glfwCreateWindow(500,500,"window title",nullptr,nullptr);
    if(window==nullptr){
        std::cout<<"failed to create window"<<std::endl;
        glfwTerminate();
        throw "Window not created";
    }
    glfwMakeContextCurrent(window);

    int gladLoadSucess = gladLoadGL();

    if(gladLoadSucess==0){
        std::cout<<"failed to load with glad"<<std::endl;
        glfwTerminate();
        throw "Could not initialize glad";
    }

    std::cout<< "GL Version: "<<glGetString(GL_VERSION)<<std::endl;
    std::cout<< "GL Shading Language Version: " <<glGetString ( GL_SHADING_LANGUAGE_VERSION )<<std::endl;

    return window;
}


