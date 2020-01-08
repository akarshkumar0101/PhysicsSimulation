//
// Created by Akarsh Kumar on 1/3/20.
//

#include "Node.h"

Viewport::Viewport(): Viewport(0,0,0,0) {
}
Viewport::Viewport(const int x, const int y, const int width, const int height): mX(x), mY(y),mWidth(width), mHeight(height) {
}


Node::Node(std::shared_ptr<Window>window): mWindow(window){
}

void Node::addChild(std::shared_ptr<Node> node) {
    mChildren.insert(node);
}
void Node::removeChild(std::shared_ptr<Node> node) {
    mChildren.erase(node);
}

Node::~Node() {
}




