//
// Created by Akarsh Kumar on 12/11/19.
//

#include "Renderer.h"

float axisVertices[18]={
        -100.0,0.0,0.0,
        +100.0,0.0,0.0,
        0.0,-100.0,0.0,
        0.0,+100.0,0.0,
        0.0,0.0,-100.0,
        0.0,0.0,+100.0,
};

unsigned int axisIndices[3][2]={
        {0,1},
        {2,3},
        {4,5},
};

VertexBuffer* vb = nullptr;
IndexBuffer* ib[3];
VertexBufferLayout* vbl = nullptr;

void initAxis() {
    vb = new VertexBuffer(axisVertices, sizeof(axisVertices));
    for (int i = 0; i < 3; i++) {
        ib[i] = new IndexBuffer(axisIndices[i], 2);
    }
    vbl = new VertexBufferLayout();
    vbl->addElement<float>(3, "vPos");
}

void Renderer::drawAxis(const Shader &shader) {
    shader.bind();
    vb->bind();
    vbl->bindVertexAttributesToShader(shader);

    for(int i=0;i<3;i++){
        glm::vec4 color(0.0);
        color[i] = 1.0;
        color[3] = 1.0;
        shader.setUniform("solidColor", color);

        ib[i]->bind();

        glDrawElements(GL_LINES, ib[i]->count(), GL_UNSIGNED_INT, nullptr);
    }

}

void Renderer::clear(int width, int height, const Shader &shader){
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if (mAxis) {
        glm::mat4 model(1.0f);
        shader.setUniform("model", model);
        drawAxis(shader);
    }
}

Renderer::Renderer(bool axis, bool wireFrame): mAxis(axis), mWireFrame(wireFrame) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    if (wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (mAxis) {
        initAxis();
    }
}

glm::vec3 perpendicular(glm::vec3 vec){
    glm::vec3 y(0,1,0);
    glm::vec3 x(1,0,0);

    glm::vec3 c1 = glm::cross(vec,y);
    if(glm::length(c1)<.1){
        glm::vec3 c2 = glm::cross(vec,x);
        return c2;
    }
    else{
        return c1;
    }
}

void Renderer::renderForce(const Force &force, const Point &position, const Shader &shader){
    float forceMag = glm::length(force);
    glm::mat4 model(1.0);

//        float yaw = glm::atan(force.y,force.x);
//        float pitch = glm::asin(force.z);
//        float roll = 0;
//        glm::quat q(glm::vec3(yaw,pitch,roll));
//        glm::mat4 rotate = glm::toMat4(q);


    model = glm::translate(model,position);


    float scaleXY = std::min(1.0f,forceMag);
    glm::mat4 scaleZ = glm::scale(glm::mat4(1.0),glm::vec3(scaleXY,scaleXY,forceMag));
    glm::mat4 toZ = glm::rotate(glm::mat4(1.0), (float) M_PI/2, glm::vec3(0.0,1.0,0.0));

    glm::mat4 rotate = glm::lookAt(glm::vec3(0.0),force,perpendicular(force));
    rotate = glm::inverse(rotate);
    model = model*rotate*scaleZ*toZ;

    //model = rotate*model;

    shader.setUniform("model", model);


    renderTriangleWise(*CommonModels::arrowModel, shader);
}


void Renderer::renderTriangleWise(const ModelGraphicsData &modelData, const Shader &shader) {
    shader.bind();

    modelData.vertexBuffer()->bind();
    modelData.indexBuffer()->bind();
    modelData.vertexBufferLayout()->bindVertexAttributesToShader(shader);

    glDrawElements(GL_TRIANGLES, modelData.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::renderLineWise(const ModelGraphicsData &modelData, const Shader &shader) {
    shader.bind();

    modelData.vertexBuffer()->bind();
    modelData.indexBuffer()->bind();
    modelData.vertexBufferLayout()->bindVertexAttributesToShader(shader);

    glDrawElements(GL_LINES, modelData.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::renderRigidBody(const ModelGraphicsData& modelData, const RigidBody &rigidBody, const ModelGraphicsData &jointModel, const Shader &shader) {
    shader.setUniform("solidColor", glm::vec4(0.0, 0.0, 0.0, 1.0));

//    glm::mat4 modelMatrix = rigidBody.pose().transformation();
    glm::mat4 modelMatrix = rigidBody.transformation();

    shader.setUniform("model", modelMatrix);

    renderLineWise(modelData, shader);

    for (PointMass pm: rigidBody.pointMasses()) {
        glm::mat4 pointTransform = pm.pose().transformation();
        pointTransform = glm::scale(pointTransform, glm::vec3(0.1));
        shader.setUniform("model", modelMatrix * pointTransform);
        renderTriangleWise(jointModel, shader);
    }
}





