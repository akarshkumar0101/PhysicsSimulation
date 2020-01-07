//
// Created by Akarsh Kumar on 1/4/20.
//

#include <vector>
#include <memory>
#include "Node.h"

struct GraphicsCommand {
    virtual void performCommand() = 0;
};
struct PointCommand: public GraphicsCommand {
    float x, y;
    void performCommand() override;
};
struct LineCommand: public GraphicsCommand {
    float x1, y1, x2, y2;
    void performCommand() override;
};

struct FillTriangleCommand: public GraphicsCommand{
    float x1, y1, x2, y2, x3, y3;

    void performCommand() override;
};

class Graphics {
private:
    std::vector<std::shared_ptr<GraphicsCommand>> commands;

public:
    void drawPoint(float x, float y);
    void drawLine(float x1, float y1, float x2, float y2);
    void outlineTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
};

using DrawingFunction = void *(Graphics& graphics);

class DrawingNode: public Node {
private:
    Graphics graphics;

    void render();

public:
    DrawingNode(DrawingFunction drawingFunction);
};


