//
// Created by Akarsh Kumar on 12/11/19.
//

#include "Renderer.h"

float axisVertices[18]={
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

unsigned int axisBuffer = 0;
unsigned int axisIndicesBuffers[3];

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

void Rendererf::setViewPort(int width, int height) {
    glViewport(0, 0, width, height);
}

void Rendererf::clear() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

