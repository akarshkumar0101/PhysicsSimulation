//
// Created by Akarsh Kumar on 11/27/19.
//

#pragma once


#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include "Shader.h"
#include "Camera.h"
#include "GraphicsBuffer.h"
#include "RigidModel.h"

class ModelData {

private:
    std::vector<float> mVertices;
    std::vector<unsigned int> mIndices;

    VertexBuffer *vertexBuffer = nullptr;
    IndexBuffer *indexBuffer = nullptr;
    VertexBufferLayout vertexBufferLayout;

    void loadFromFile(const std::string &fileName);

    void initBuffers() ;

    static std::vector<unsigned int> defaultListOfVertices(const std::vector<float> &vertices) ;

public:
    ModelData(const std::string &fileName);

    ModelData(const std::vector<float> &vertices) ;

    ModelData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);

    ModelData(const RigidModel &model);

    ~ModelData();

    void draw(const Shader &shader) const;

    void drawModelOutline(const Shader &shader) const;

    //will organize this lol
    void drawModel(const RigidModel &rigidModel, const ModelData &jointModel, const Shader &shader) ;
};



