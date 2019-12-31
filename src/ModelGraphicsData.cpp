//
// Created by Akarsh Kumar on 12/11/19.
//

#include "ModelGraphicsData.h"

void ModelGraphicsData::loadFromFile(const std::string &fileName) {
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

void ModelGraphicsData::initBuffers() {
    mVertexBuffer = new VertexBuffer(mVertices.data(), mVertices.size() * sizeof(float));
    mIndexBuffer = new IndexBuffer(mIndices.data(), mIndices.size());

    mVertexBufferLayout.addElement<float>(3, "vPos");
}

std::vector<unsigned int> ModelGraphicsData::defaultListOfVertices(const std::vector<float> &vertices) {
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }
    return indices;
}

ModelGraphicsData::ModelGraphicsData(const std::string &fileName) {
    loadFromFile(fileName);
    initBuffers();
}

ModelGraphicsData::ModelGraphicsData(const std::vector<float> &vertices) : ModelGraphicsData(vertices, defaultListOfVertices(vertices)) {
}

ModelGraphicsData::ModelGraphicsData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices) : mVertices(
        vertices), mIndices(indices) {
    initBuffers();
}

ModelGraphicsData::ModelGraphicsData(const RigidBody &model) {
    for (int i = 0; i < model.pointMasses().size(); i++) {
        Point r = model.pointMasses()[i].r();
        mVertices.push_back(r.x);
        mVertices.push_back(r.y);
        mVertices.push_back(r.z);
        for (int j = i + 1; j < model.pointMasses().size(); j++) {
            Point r2 = model.pointMasses()[j].r();
            mIndices.push_back(i);
            mIndices.push_back(j);
        }
    }
    initBuffers();
}

ModelGraphicsData::~ModelGraphicsData() {
    if (mVertexBuffer != nullptr) {
        delete mVertexBuffer;
    }
    if (mIndexBuffer != nullptr) {
        delete mIndexBuffer;
    }
}


namespace CommonModels{
    ModelGraphicsData* teapotModel = nullptr;
    ModelGraphicsData* squareModel = nullptr;
    ModelGraphicsData* cubeModel = nullptr;
    ModelGraphicsData* arrowModel = nullptr;

    void initCommonModels(){
        teapotModel = new ModelGraphicsData("resources/models/teapot.obj");
        squareModel = new ModelGraphicsData("resources/models/square.obj");
        cubeModel = new ModelGraphicsData("resources/models/cube.obj");
        arrowModel = new ModelGraphicsData("resources/models/arrow.obj");
    }
}


