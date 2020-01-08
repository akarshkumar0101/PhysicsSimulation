//
// Created by Akarsh Kumar on 1/4/20.
//

#include "DrawingNode.h"
#include "GraphicsBuffer.h"


static Shader& drawingShader(){
    static Shader drawingShader("resources/shaders/drawingNodeShader.shader");
    return drawingShader;
}

static VertexArray& pointArray2D() {
    static bool first = true;
    static VertexArray va;
    static auto vb = std::make_shared<VertexBuffer>();

    if (first) {
        vb->createBuffer(nullptr, 3 * sizeof(float));

        VertexBufferLayout vbl;
        vbl.addElement<float>(3);

        va.addBuffer(vb, vbl);

        first = false;
    }

    return va;
}


void Graphics::drawPoint(float x, float y) {
//    (*pointArray2D().referencedBuffers().begin())
}

void Graphics::drawLine(float x1, float y1, float x2, float y2) {

}

void Graphics::fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {

}

DrawingNode::DrawingNode(std::shared_ptr<Window> window, DrawingFunction drawingFunction): Node(window), mDrawingFunction(drawingFunction) {
}

void DrawingNode::render(const Viewport &viewport) {
    viewport.bind();
    mDrawingFunction(mGraphics);
}


