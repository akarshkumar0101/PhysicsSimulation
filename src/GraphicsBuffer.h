//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "Shader.h"

class VertexBuffer{
private:
    unsigned int mBufferID;
    unsigned int mSize;
public:
    VertexBuffer();
    VertexBuffer(const void* data, const size_t size);
    ~VertexBuffer();
    void bind();
    void unbind();
};

class IndexBuffer{
private:
    unsigned int mBufferID;
    unsigned int mCount;
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, const unsigned int count);
    ~IndexBuffer();
    void bind();
    void unbind();
};

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    std::string shaderAttributeName;

    static size_t sizeofType(int type);
};

class VertexBufferLayout{
private:
    std::vector<VertexBufferElement> mElements;
    size_t mStride;
public:
    VertexBufferLayout();
    VertexBufferLayout(const std::vector<VertexBufferElement>& elements );
    template<int N>
    VertexBufferLayout(const std::array<VertexBufferElement,N>& elements);

    void addElement(VertexBufferElement e);
    void addElement(const unsigned int type,const unsigned int count, const unsigned char normalized, const std::string& shaderAttributeName);

    template<class T>
    void addElement(const unsigned int count, const std::string shaderAttributeName);

    void bindVertexAttributesToShader(const Shader& shader) const;
};




