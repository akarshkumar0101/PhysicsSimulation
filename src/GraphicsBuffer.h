//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_set>

#include "Shader.h"
#include "RigidBody.h"

class VertexBuffer{
private:
    unsigned int mBufferID;
    size_t mSize;
public:
    VertexBuffer();
    ~VertexBuffer();
    void putData(const void* data, const size_t size);
    void bind() const;
    void unbind() const;
    size_t size() const;
};

class IndexBuffer{
private:
    unsigned int mBufferID;
    unsigned int mCount;
public:
    IndexBuffer();
    ~IndexBuffer();
    void putData(const unsigned int* data, const unsigned int count);
    void bind() const;
    void unbind() const;
    unsigned int count() const;
};

struct VertexBufferElement {
    static size_t sizeofType(unsigned int type);

    unsigned int type; //represents OpenGL type
    unsigned int count;
    unsigned char normalized;
};

class VertexBufferLayout {
private:
    //vertex array object
    std::vector<VertexBufferElement> mElements;
    size_t mStride;
public:
    static VertexBufferLayout* coordinateOnlyLayout;
    static void initCommonLayouts();
    static void destroyCommonLayouts();

    VertexBufferLayout();
    VertexBufferLayout(const std::vector<VertexBufferElement>& elements );

    void addElement(const VertexBufferElement &e);
    void addElement(const unsigned int type, const unsigned int count, const unsigned char normalized);

    template<class T>
    void addElement(const unsigned int count);

    const std::vector<VertexBufferElement>& elements() const;
    size_t stride() const;
};

//
class VertexArray {
private:
    unsigned int mArrayID;
    std::unordered_set<std::shared_ptr<const VertexBuffer>> referencedBuffers;
public:
    VertexArray();
    ~VertexArray();

    // uses default layout
    void addBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout);
    // uses shader layout
    void addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &vertexBufferLayout, const Shader& shader, const std::vector<std::string>& attributeNames);

    void bind() const;
    void unbind() const;
};


class GraphicsData {
private:
    std::shared_ptr<VertexArray> mVertexArray;
    std::shared_ptr<IndexBuffer> mIndexBuffer;


    void loadFromFile(const std::string &fileName);
    void initBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    static std::vector<unsigned int> defaultListOfIndices(const std::vector<float> &vertices) ;

public:
    GraphicsData(const std::string &fileName);

    GraphicsData(const std::vector<float> &vertices);
    GraphicsData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);

    GraphicsData(const RigidBody &model);

    ~GraphicsData();

    std::shared_ptr<IndexBuffer> indexBuffer() const{
        return mIndexBuffer;
    }

    void bind() const;
    void unbind() const;

};
namespace CommonModels{
    extern GraphicsData* teapotModel;
    extern GraphicsData* squareModel;
    extern GraphicsData* cubeModel;
    extern GraphicsData* arrowModel;

    void initCommonModels();
    void destroyCommonModels();
}

