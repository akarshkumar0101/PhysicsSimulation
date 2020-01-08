//
// Created by Akarsh Kumar on 12/11/19.
//

#include <fstream>
#include "GraphicsBuffer.h"

VertexBuffer::VertexBuffer(): mBufferID(0), mSize(0){
    glGenBuffers(1, &mBufferID);
}
VertexBuffer::~VertexBuffer(){
    glDeleteBuffers(1, &mBufferID);
}

void VertexBuffer::bind() const{
    glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
}
void VertexBuffer::unbind() const{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VertexBuffer::putData(const void* data, const size_t size) {
    mSize = size;
    bind();
    glBufferData(GL_ARRAY_BUFFER, mSize, data, GL_STATIC_DRAW);
    unbind();
}
size_t VertexBuffer::size() const{
    return mSize;
}


IndexBuffer::IndexBuffer(): mBufferID(0), mCount(0){
    glGenBuffers(1, &mBufferID);
}
IndexBuffer::~IndexBuffer(){
    glDeleteBuffers(1, &mBufferID);
}
void IndexBuffer::putData(const unsigned int* data, const unsigned int count){
    mCount = count;
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount*sizeof(unsigned int), data, GL_STATIC_DRAW);
    unbind();
}
void IndexBuffer::bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
}
void IndexBuffer::unbind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
unsigned int IndexBuffer::count() const{
    return mCount;
}



size_t VertexBufferElement::sizeofType(unsigned int type){
    switch(type){
        case GL_FLOAT: return sizeof(float);
        case GL_UNSIGNED_INT: return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
    }
    return 0;
}


VertexBufferLayout::VertexBufferLayout():mStride(0){
}
VertexBufferLayout::VertexBufferLayout(const std::vector<VertexBufferElement>& elements ):VertexBufferLayout(){
    for(auto e:elements){
        addElement(e);
    }
}
void VertexBufferLayout::addElement(const VertexBufferElement &e){
    mStride += e.count * VertexBufferElement::sizeofType(e.type);
    mElements.push_back(e);
}
void VertexBufferLayout::addElement(const unsigned int type,const unsigned int count, const unsigned char normalized){
    addElement({type, count, normalized});
}

template<>
void VertexBufferLayout::addElement<float>(unsigned int count){
    addElement(GL_FLOAT, count, GL_FALSE);
}
template<>
void VertexBufferLayout::addElement<unsigned int>(unsigned int count){
    addElement(GL_UNSIGNED_INT, count,GL_FALSE);
}
template<>
void VertexBufferLayout::addElement<unsigned char>(unsigned int count){
    addElement(GL_UNSIGNED_BYTE, count, GL_FALSE);
}

VertexBufferLayout* VertexBufferLayout::coordinateOnlyLayout = nullptr;

void VertexBufferLayout::initCommonLayouts() {
    coordinateOnlyLayout = new VertexBufferLayout({{GL_FLOAT, 3, false}});
}

void VertexBufferLayout::destroyCommonLayouts() {
    delete coordinateOnlyLayout;
}

const std::vector<VertexBufferElement> &VertexBufferLayout::elements() const{
    return mElements;
}
size_t VertexBufferLayout::stride() const{
    return mStride;
}

VertexArray::VertexArray(){
    glGenVertexArrays(1, &mArrayID);
}
VertexArray::~VertexArray(){
    glDeleteVertexArrays(1, &mArrayID);
}

void VertexArray::addBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer, const VertexBufferLayout &vertexBufferLayout){
//    referencedBuffers.insert(std::shared_ptr<VertexBuffer>(&vertexBuffer));
    referencedBuffers.insert(vertexBuffer);

    bind();
    vertexBuffer->bind();

    size_t offset = 0;

    for(unsigned int i=0; i < vertexBufferLayout.elements().size(); i++){
        const VertexBufferElement& vertexBufferElement = vertexBufferLayout.elements().at(i);
        glVertexAttribPointer(i, vertexBufferElement.count, vertexBufferElement.type, vertexBufferElement.normalized,
                              vertexBufferLayout.stride(), (void *) offset);
        glEnableVertexAttribArray(i);

        offset += vertexBufferElement.count * VertexBufferElement::sizeofType(vertexBufferElement.type);
    }
}


void VertexArray::addBuffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &vertexBufferLayout, const Shader& shader, const std::vector<std::string>& attributeNames){
    referencedBuffers.emplace(&vertexBuffer);
    bind();
    vertexBuffer.bind();

    size_t offset = 0;

    for(unsigned int i=0; i < vertexBufferLayout.elements().size(); i++){
        const VertexBufferElement& vertexBufferElement = vertexBufferLayout.elements().at(i);
        int attribLocation = shader.getAttribLocation(attributeNames.at(i));
        glVertexAttribPointer(attribLocation, vertexBufferElement.count, vertexBufferElement.type, vertexBufferElement.normalized,
                              vertexBufferLayout.stride(), (void *) offset);
        glEnableVertexAttribArray(attribLocation);

        offset += vertexBufferElement.count * VertexBufferElement::sizeofType(vertexBufferElement.type);
    }
}

void VertexArray::bind() const{
    glBindVertexArray(mArrayID);
}
void VertexArray::unbind() const{
    glBindVertexArray(0);
}

void GraphicsData::loadFromFile(const std::string &fileName) {
    std::ifstream fileStream(fileName);
    std::string word;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    while (fileStream >> word) {
        if (word == "v") {
            float x, y, z;
            fileStream >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        } else if (word == "f") {
            unsigned int i1, i2, i3;
            fileStream >> i1 >> i2 >> i3;
            indices.push_back(i1 - 1);
            indices.push_back(i2 - 1);
            indices.push_back(i3 - 1);
        }
    }
    initBuffers(vertices, indices);
}

void GraphicsData::initBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
//    mVertexBuffer = new VertexBuffer(mVertices.data(), mVertices.size() * sizeof(float));
    mIndexBuffer = std::make_shared<IndexBuffer>();
    mIndexBuffer->putData(indices.data(), indices.size());

    std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->putData(vertices.data(), vertices.size() * sizeof(float));

    mVertexArray = std::make_shared<VertexArray>();
    mVertexArray->addBuffer(vertexBuffer, *VertexBufferLayout::coordinateOnlyLayout);
}

std::vector<unsigned int> GraphicsData::defaultListOfIndices(const std::vector<float> &vertices) {
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        indices.push_back(i);
    }
    return indices;
}

GraphicsData::GraphicsData(const std::string &fileName) {
    loadFromFile(fileName);
}

GraphicsData::GraphicsData(const std::vector<float> &vertices) {
    initBuffers(vertices, defaultListOfIndices(vertices));
}

GraphicsData::GraphicsData(const std::vector<float> &vertices, const std::vector<unsigned int> &indices){
    initBuffers(vertices, indices);
}

GraphicsData::GraphicsData(const RigidBody &model) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    for (int i = 0; i < model.pointMasses().size(); i++) {
        Point r = model.pointMasses()[i].r();
        vertices.push_back(r.x);
        vertices.push_back(r.y);
        vertices.push_back(r.z);
        for (int j = i + 1; j < model.pointMasses().size(); j++) {
            Point r2 = model.pointMasses()[j].r();
            indices.push_back(i);
            indices.push_back(j);
        }
    }
    initBuffers(vertices, indices);
}

GraphicsData::~GraphicsData() {
}
void GraphicsData::bind() const{
    mVertexArray->bind();
    mIndexBuffer->bind();
}
void GraphicsData::unbind() const{
    mVertexArray->unbind();
    mIndexBuffer->unbind();
}


namespace CommonModels{
    GraphicsData* teapotModel = nullptr;
    GraphicsData* squareModel = nullptr;
    GraphicsData* cubeModel = nullptr;
    GraphicsData* arrowModel = nullptr;

    void initCommonModels(){
        teapotModel = new GraphicsData("resources/models/teapot.obj");
        squareModel = new GraphicsData("resources/models/square.obj");
        cubeModel = new GraphicsData("resources/models/cube.obj");
        arrowModel = new GraphicsData("resources/models/arrow.obj");
    }
    void destroyCommonModels(){
        delete teapotModel;
        delete squareModel;
        delete cubeModel;
        delete arrowModel;
    }
}