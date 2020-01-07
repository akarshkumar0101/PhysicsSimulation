//
// Created by Akarsh Kumar on 1/3/20.
//

#pragma once

#include <unordered_set>
#include <memory>

#include "Window.h"

class Viewport {
private:
    int mX, mY;
    int mWidth, mHeight;

public:
    Viewport();
    Viewport(const int x, const int y, const int width, const int height);

    int x(){ return mX; }
    int x2(){ return mX+mWidth; }
    int y(){ return mY; }
    int y2(){ return mY+mHeight; }
    int width(){ return mWidth; }
    int height(){ return mHeight; }
};

class Window;

class Node {
private:
    Window& mWindow;
    std::unordered_set<std::shared_ptr<Node> > mChildren;

protected:
    Viewport mViewport;
    void addChild(Node& node);
    void removeChild(Node& node);

public:
    Node(Window& window);
    virtual ~Node();

    virtual void setViewport(const Viewport& viewport);



    void renderNode();

    virtual void render() = 0;
};
