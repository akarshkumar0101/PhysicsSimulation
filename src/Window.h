//
// Created by Akarsh Kumar on 12/11/19.
//

#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Window {
private:
    GLFWwindow* mWindow;

    void init();

public:
    Window(const int width, const int height, const std::string& title);
    Window(const int width, const int height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share);
    ~Window();

    void getFramebufferSize(int& width, int& height);

    void setShouldClose(bool shouldClose);
    bool shouldClose();

    void makeContextCurrent();
    void swapBuffers();

    bool isKeyReleased(int key);
    bool isKeyPressed(int key);
    bool isKeyRepeated(int key);

    void getCursorPosition(double& xPos, double& yPos);

    GLFWwindow* getRawWindowPointer();
};

