//
// Created by Akarsh Kumar on 11/28/19.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


class VertexBuffer{
private:
    unsigned int mBufferID;
    unsigned int mSize;
public:
    VertexBuffer():mBufferID(0), mSize(0){
    }
    VertexBuffer(const void* data, const size_t size): mSize(size){
        glGenBuffers(1, &mBufferID);
        bind();
        glBufferData(GL_ARRAY_BUFFER, mSize, data, GL_STATIC_DRAW);
        unbind();
    }
    ~VertexBuffer(){
        glDeleteBuffers(1, &mBufferID);
    }
    void bind(){
        glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
    }
    void unbind(){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

};

class IndexBuffer{
private:
    unsigned int mBufferID;
    unsigned int mCount;
public:
    IndexBuffer():mBufferID(0), mCount(0){
    }
    IndexBuffer(const unsigned int* data, const unsigned int count): mCount(count){
        glGenBuffers(1, &mBufferID);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount*sizeof(unsigned int), data, GL_STATIC_DRAW);
        unbind();
    }
    ~IndexBuffer(){
        glDeleteBuffers(1, &mBufferID);
    }
    void bind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
    }
    void unbind(){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

};

struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    std::string shaderAttributeName;

    static size_t sizeofType(int type){
        switch(type){
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_INT:
                return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE:
                return sizeof(unsigned char);
        }
        return 0;
    }
};

class VertexBufferLayout{
private:
    std::vector<VertexBufferElement> mElements;
    size_t mStride;
public:
    VertexBufferLayout():mStride(0){
    }
    VertexBufferLayout(const std::vector<VertexBufferElement>& elements ):VertexBufferLayout(){
        for(auto e:elements){
            addElement(e);
        }
    }
    template<int N>
    VertexBufferLayout(const std::array<VertexBufferElement,N>& elements):VertexBufferLayout(){
        for(auto e:elements){
            addElement(e);
        }
    }
    void addElement(VertexBufferElement e){
        mStride += e.count * VertexBufferElement::sizeofType(e.type);
        mElements.push_back(e);
    }
    void addElement(const unsigned int type,const unsigned int count, const unsigned char normalized, const std::string& shaderAttributeName){
        addElement({type, count, normalized, shaderAttributeName});
    }
    template<class T>
    void addElement(const unsigned int count, const std::string shaderAttributeName);

    void bindVertexAttributesToShader(const Shader& shader) const{
        size_t offset = 0;
        for (VertexBufferElement e: mElements) {
            int attribLocation = shader.getAttribLocation(e.shaderAttributeName);
            glVertexAttribPointer(attribLocation, e.count, e.type, e.normalized, mStride, (void *) offset);
            glEnableVertexAttribArray(attribLocation);

            offset += e.count * VertexBufferElement::sizeofType(e.type);
        }
    }
};

template<>
void VertexBufferLayout::addElement<float>(unsigned int count, const std::string shaderAttributeName){
    addElement(GL_FLOAT, count, GL_FALSE, shaderAttributeName);
}
template<>
void VertexBufferLayout::addElement<unsigned int>(unsigned int count, const std::string shaderAttributeName){
    addElement(GL_UNSIGNED_INT, count,GL_FALSE, shaderAttributeName);
}
template<>
void VertexBufferLayout::addElement<unsigned char>(unsigned int count, const std::string shaderAttributeName){
    addElement(GL_UNSIGNED_BYTE, count, GL_FALSE, shaderAttributeName);
}


