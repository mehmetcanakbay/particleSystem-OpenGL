#pragma once 

#include "Public/Renderer.h"
#include <vector>

//since struct makes mem adresses sequential, this will work
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;

    Vertex(const glm::vec3& pos=glm::vec3(0.0f), const glm::vec4 col=glm::vec4(1.0f)) {
        position = pos;
        color = col;
    }
};

class VertexFactory {
private:
    std::vector<Vertex> vertexPositions;
    unsigned int m_Amount;
public:
    VertexFactory(unsigned int amount, void (*positionFunc)(Vertex* pos, int index));
    //ARGS:
    //positionFunc: For every position, run this function at the start
    inline std::vector<Vertex> ReturnVertex() {return vertexPositions;}
};