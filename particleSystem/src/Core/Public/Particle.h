#pragma once

#include "Public/Renderer.h"
#include "Public/Vertex.h"
#include "glm/gtc/random.hpp"

class Particle {

private:
    Vertex vertexInfo;
    glm::vec3 velocity = glm::vec3(0.0f, 1.0f, 0.0f);
    float particleLifetime; 
    glm::vec4 particleColor;

    unsigned int particle_id;

    bool isActive;

    glm::vec3 prevLoc;
public:
    Particle(const Vertex& vertex = Vertex());

    void UpdateParticle();
    
    inline glm::vec3 ReturnPosition() const { return vertexInfo.position; }
    inline glm::vec4 ReturnColor() const { return vertexInfo.color; }

    inline const glm::vec3 ReturnVelocity() { return velocity; }
};