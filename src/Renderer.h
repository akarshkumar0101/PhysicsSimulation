//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include "Shader.h"
#include "GraphicsBuffer.h"

extern float axisVertices[18];

extern unsigned int axisIndices[3][2];

extern unsigned int axisBuffer;
extern unsigned int axisIndicesBuffers[3];

void initAxis();

class Renderer{
private:
    void renderAxis(const Shader& shader);
    bool mAxis;
    bool mWireFrame;

public:
    Renderer(bool axis, bool wireFrame);

    void clear(int width, int height, const Shader& shader);

    void renderTriangleWise(const GraphicsData& modelData, const Shader& shader);
    void renderLineWise(const GraphicsData& modelData, const Shader& shader);

    void renderRigidBody(const GraphicsData& modelData, const RigidBody &rigidBody, const GraphicsData &jointModel, const Shader &shader);

    void renderForce(const Force& force, const Point& position, const Shader& shader);
};

