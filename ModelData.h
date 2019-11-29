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

class ModelData {
private:
    std::vector<float> mVertices;
    std::vector<unsigned int> mIndices;

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    VertexBufferLayout vertexBufferLayout;

    void loadFromFile(const std::string& fileName){
        std::ifstream fileStream(fileName);
        std::string word;

        while(fileStream >> word){
            if(word == "v"){
                float x,y,z;
                fileStream >> x >> y >> z;
                mVertices.push_back(x);
                mVertices.push_back(y);
                mVertices.push_back(z);
            }
            else if(word=="f"){
                unsigned int i1,i2,i3;
                fileStream >> i1 >> i2 >> i3;
                mIndices.push_back(i1 - 1);
                mIndices.push_back(i2 - 1);
                mIndices.push_back(i3 - 1);
            }
        }
    }
    void initBuffers(){
        vertexBuffer = new VertexBuffer(&mVertices.front(), mVertices.size()*sizeof(float));
        indexBuffer = new IndexBuffer(&mIndices.front(), mIndices.size());

        vertexBufferLayout.addElement<float>(3, "vPos");
    }
    static std::vector<unsigned int> defaultListOfVertices(const std::vector<float>& vertices){
        std::vector<unsigned int> indices;
        for(unsigned int i=0;i<vertices.size();i++){
            indices.push_back(i);
        }
        return indices;
    }
public:
    ModelData(const std::string& fileName) {
        loadFromFile(fileName);
        initBuffers();
    }
    ModelData(const std::vector<float>& vertices): ModelData(vertices, defaultListOfVertices(vertices)){
    }
    ModelData(const std::vector<float>& vertices, const std::vector<unsigned int >& indices): mVertices(vertices), mIndices(indices){
        initBuffers();
    }

    void draw(const Shader& shader){
        shader.bind();
        vertexBuffer->bind();
        indexBuffer->bind();

        vertexBufferLayout.bindVertexAttributesToShader(shader);

        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    }
};