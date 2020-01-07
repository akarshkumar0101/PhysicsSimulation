//
// Created by Akarsh Kumar on 1/3/20.
//

#include "GridNode.h"


GridNode::GridNode(Window& window, std::pair<unsigned int, unsigned int> gridDimensions): Node(window), mGridDimensions(std::move(gridDimensions)) {
}


void GridNode::setViewportForChild(std::pair<unsigned int, unsigned int> gridLocation) {
    int width = mViewport.width() / mGridDimensions.first;
    int height = mViewport.height() / mGridDimensions.second;
    int x = mViewport.x()+ (((float) gridLocation.first/mGridDimensions.first) * mViewport.width());
    int y = mViewport.y()+ (((float) gridLocation.second/mGridDimensions.second) * mViewport.height());
    Viewport viewport(x, y, width, height);
    mGridChildren[gridLocation]->setViewport(viewport);
}

void GridNode::setViewport(const Viewport &viewport) {
    Node::setViewport(viewport);
    for(auto it: mGridChildren){
        setViewportForChild(it.first);
    }
}

void GridNode::setChild(Node& node, std::pair<unsigned int, unsigned int> gridLocation){
    Node::addChild(node);

    auto it = mGridChildren.find(gridLocation);
    if(it != mGridChildren.end()){ // if child already exists
        Node::removeChild(*(it->second));
        it->second = std::shared_ptr<Node>(&node);
    }
    else{
        mGridChildren.insert(std::pair(gridLocation, std::shared_ptr<Node>(&node)));
    }
    setViewportForChild(gridLocation);
}
void GridNode::removeChild(std::pair<unsigned int, unsigned int> gridLocation) {
    Node::removeChild(*(mGridChildren[gridLocation]));
    mGridChildren.erase(gridLocation);
}

void GridNode::render() {
    for(auto it: mGridChildren){
        it.second->renderNode();
    }
}






