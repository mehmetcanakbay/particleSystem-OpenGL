#include "Public/Particle.h"
#include <iostream>

Particle::Particle(const Vertex& vertex)
	:vertexInfo(vertex)
{
	vertexInfo.color = glm::vec4(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), 
		glm::linearRand(0.f, 1.0f), 1.0f);

	velocity = glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), 0.0f);
	velocity = velocity * (1/glm::length(velocity));
	velocity = velocity * glm::linearRand(0.0f, 5.0f);
	prevLoc = glm::vec3(0);
}

void Particle::UpdateParticle()
{
	vertexInfo.position = vertexInfo.position + velocity * 0.0005f;
	velocity *= 0.999f;
	glm::vec3 diff = vertexInfo.position - prevLoc;
	//std::cout << diff.x <<" "<< diff.y << " " << diff.z << std::endl;
	prevLoc = vertexInfo.position;
}

