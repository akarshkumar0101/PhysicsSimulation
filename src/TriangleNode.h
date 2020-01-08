//
// Created by Akarsh Kumar on 1/8/20.
//

#include <vector>

#include "Node.h"
#include "GraphicsBuffer.h"

class TriangleNode: public Node {
private:
    GraphicsData triangleData;
    Shader triangleShader;

    static std::vector<float> verticies;
    static std::vector<unsigned int> indices;
public:
    TriangleNode(std::shared_ptr<Window> window):Node(window), triangleData(verticies, indices), triangleShader("resources/shaders/triangle.shader"){
    }

    void render(const Viewport& viewport) override{
        viewport.bind();
        triangleShader.bind();
        triangleData.bind();
        glDrawElements(GL_TRIANGLES, triangleData.indexBuffer()->count(), GL_UNSIGNED_INT, nullptr);
    }
};
