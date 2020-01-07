//
// Created by Akarsh Kumar on 11/27/19.
//

#pragma once


#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "GraphicsBuffer.h"
#include "RigidBody.h"

class ModelGraphicsData {

private:
    std::vector<float> mVertices;
    std::vector<unsigned int> mIndices;

    VertexBuffer *mVertexBuffer = nullptr;
    IndexBuffer *mIndexBuffer = nullptr;
    VertexBufferLayout mVertexBufferLayout;

    void loadFromFile(const std::string &fileName);

    void initBuffers();

    static std::vector<unsigned int> defaultListOfVertices(const std::vector<float> &vertices) ;

public:
    ModelGraphicsData(const std::string &fileName);

    ModelGraphicsData(const std::vector<float> &vertices) ;

    ModelGraphicsData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);

    ModelGraphicsData(const RigidBody &model);

    ~ModelGraphicsData();

    VertexBuffer* vertexBuffer() const{
        return mVertexBuffer;
    }
    IndexBuffer* indexBuffer() const{
        return mIndexBuffer;
    }
    const VertexBufferLayout* vertexBufferLayout() const{
        return &mVertexBufferLayout;
    }

};

namespace CommonModels{
    extern ModelGraphicsData* teapotModel;
    extern ModelGraphicsData* squareModel;
    extern ModelGraphicsData* cubeModel;
    extern ModelGraphicsData* arrowModel;

    void initCommonModels();
    void destroyCommonModels();
}


