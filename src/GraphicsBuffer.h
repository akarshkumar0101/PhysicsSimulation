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

enum BufferUsage{
    STATIC, DYNAMIC
};

class GraphicsBuffer {
private:
    const unsigned int mBufferID;
    unsigned int mCount; //num elements (indices or vertices_
    size_t mSize; //size of buffer
    virtual GLenum getTarget() const = 0;
public:
    GraphicsBuffer();
    ~GraphicsBuffer();
    void allocateBuffer(const size_t size, const void* data, BufferUsage usage = STATIC);
    void changeBufferSubData(const size_t offset, const size_t size, const void* data);
    void bind() const;
    void unbind() const;
    unsigned int count() const;
    size_t size() const;
};

class VertexBufferF: public GraphicsBuffer {
private:
    virtual GLenum getTarget() const override {return GL_ARRAY_BUFFER;}
};
class IndexBufferF: public GraphicsBuffer {
private:
    virtual GLenum getTarget() const override {return GL_ELEMENT_ARRAY_BUFFER;}
};

class VertexBuffer{
private:
    unsigned int mBufferID;
    size_t mSize;
public:
    VertexBuffer();
    ~VertexBuffer();
    void createBuffer(const void* data, const size_t size);
    void createBuffer(const void* data, const size_t size, BufferUsage usage);
    void changeData();
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
    std::unordered_set<std::shared_ptr<const VertexBuffer>> mReferencedBuffers;
public:
    VertexArray();
    ~VertexArray();

    // uses default layout
    void addBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout);
    // uses shader layout
    void addBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout, const Shader& shader, const std::vector<std::string>& attributeNames);

    std::unordered_set<std::shared_ptr<const VertexBuffer>>& referencedBuffers(){
        return mReferencedBuffers;
    }

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
    GraphicsData(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<IndexBuffer> indexBuffer);

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

