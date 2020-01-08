//
// Created by Akarsh Kumar on 1/3/20.
//

#pragma once

#include <unordered_map>
#include <utility>

#include "Node.h"
#include "GraphicsBuffer.h"


struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

class GridNode: public Node {
private:
    // numRows, numCols
    std::pair<unsigned int, unsigned int> mGridDimensions;

    std::unordered_map<std::pair<unsigned int, unsigned int>, std::shared_ptr<Node>, pair_hash> mGridChildren;

    Viewport viewportForChild(std::pair<unsigned int, unsigned int> gridLocation, const Viewport& viewport);

public:
    GridNode(std::shared_ptr<Window> window, std::pair<unsigned int, unsigned int> gridDimensions);

    void setChild(std::shared_ptr<Node> node, std::pair<unsigned int, unsigned int> gridLocation);
    void removeChild(std::pair<unsigned int, unsigned int> gridLocation);

    void render(const Viewport& viewport) override;

};


