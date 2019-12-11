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

    void loadFromFile(const std::string &fileName) {
        std::ifstream fileStream(fileName);
        std::string word;

        while (fileStream >> word) {
            if (word == "v") {
                float x, y, z;
                fileStream >> x >> y >> z;
                mVertices.push_back(x);
                mVertices.push_back(y);
                mVertices.push_back(z);
            } else if (word == "f") {
                unsigned int i1, i2, i3;
                fileStream >> i1 >> i2 >> i3;
                mIndices.push_back(i1 - 1);
                mIndices.push_back(i2 - 1);
                mIndices.push_back(i3 - 1);
            }
        }
    }

    void initBuffers() {
        vertexBuffer = new VertexBuffer(&mVertices.front(), mVertices.size() * sizeof(float));
        indexBuffer = new IndexBuffer(&mIndices.front(), mIndices.size());

        vertexBufferLayout.addElement<float>(3, "vPos");
    }

    static std::vector<unsigned int> defaultListOfVertices(const std::vector<float> &vertices) {
        std::vector<unsigned int> indices;
        for (unsigned int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
        }
        return indices;
    }

public:
    ModelData(const std::string &fileName) {
        loadFromFile(fileName);
        initBuffers();
    }

    ModelData(const std::vector<float> &vertices) : ModelData(vertices, defaultListOfVertices(vertices)) {
    }

    ModelData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) : mVertices(vertices),
                                                                                              mIndices(indices) {
        initBuffers();
    }

    ModelData(const RigidModel &model) {
        for (int i = 0; i < model.pointMasses().size(); i++) {
            Point r = model.pointMasses()[i].pose().r();
            mVertices.push_back(r.x);
            mVertices.push_back(r.y);
            mVertices.push_back(r.z);
            for (int j = i + 1; j < model.pointMasses().size(); j++) {
                Point r2 = model.pointMasses()[j].pose().r();
                mIndices.push_back(i);
                mIndices.push_back(j);
            }
        }
        initBuffers();
    }

    ~ModelData() {
        if (vertexBuffer != nullptr) {
            delete vertexBuffer;
        }
        if (indexBuffer != nullptr) {
            delete indexBuffer;
        }
    }

    void draw(const Shader &shader) const {
        shader.bind();
        vertexBuffer->bind();
        indexBuffer->bind();

        vertexBufferLayout.bindVertexAttributesToShader(shader);

        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    }

    void drawModelOutline(const Shader &shader) const {
        shader.bind();
        vertexBuffer->bind();
        indexBuffer->bind();

        vertexBufferLayout.bindVertexAttributesToShader(shader);

        glDrawElements(GL_LINES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    }

    //will organize this lol
    void drawModel(const RigidModel &rigidModel, const ModelData &jointModel, const Shader &shader) {
        shader.setUniform("solidColor", glm::vec4(0.0, 0.0, 0.0, 1.0));
        glm::mat4 modelMatrix = rigidModel.pose().transformation();

        shader.setUniform("model", modelMatrix);
        drawModelOutline(shader);

        for (PointMass pm: rigidModel.pointMasses()) {
            glm::mat4 pointTransform = pm.pose().transformation();
            pointTransform = glm::scale(pointTransform, glm::vec3(0.1));
            shader.setUniform("model", modelMatrix * pointTransform);
            jointModel.draw(shader);
        }
    }
};


ModelData *teapotModel;
ModelData *squareModel;
ModelData *cubeModel;
ModelData *arrowModel;

void initModels() {
    teapotModel = new ModelData("resources/models/teapot.obj");
    squareModel = new ModelData("resources/models/square.obj");
    cubeModel = new ModelData("resources/models/cube.obj");
    arrowModel = new ModelData("resources/models/arrow.obj");
}
void destroyModels(){
    delete teapotModel;
    delete squareModel;
    delete cubeModel;
    delete arrowModel;
}