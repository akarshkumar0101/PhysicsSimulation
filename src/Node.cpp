//
// Created by Akarsh Kumar on 1/3/20.
//

#include "Node.h"

Viewport::Viewport(): Viewport(0,0,0,0) {
}
Viewport::Viewport(const int x, const int y, const int width, const int height): mX(x), mY(y),mWidth(width), mHeight(height) {
}


Node::Node(Window &window): mWindow(window){
}

void Node::setViewport(const Viewport &viewport) {
    mViewport = viewport;
}

void Node::addChild(Node& node) {
    mChildren.emplace(&node);
}
void Node::removeChild(Node& node) {
    mChildren.erase(std::shared_ptr<Node>(&node));
}

void Node::renderNode() {
    //use window.setViewport
    glViewport(mViewport.x(),mViewport.y(),mViewport.width(), mViewport.height());
    render();
}

Node::~Node() {
}




