#include "Public/Particle.h"
#include <iostream>

Particle::Particle(const Vertex& vertex)
	:vertexInfo(vertex), particleLifetime(3.0f)
{
	vertexInfo.position = glm::vec3(glm::linearRand(-0.1f, 0.1f), glm::linearRand(-0.1f, 0.1f), 0.0f);
	vertexInfo.color = glm::vec4(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), 
		glm::linearRand(0.f, 1.0f), 1.0f);

	velocity = glm::vec3(glm::linearRand(-1.0f, 1.0f), glm::linearRand(-1.0f, 1.0f), 0.0f);
	velocity = velocity * (1/glm::length(velocity));
	velocity = velocity * glm::linearRand(0.001f, 0.5f) * 2.0f;
	prevLoc = glm::vec3(0);

	particleLifetime = glm::linearRand(1.0f, 3.0f);
	maxLifetime = particleLifetime;

	velocityDampValue = glm::linearRand(0.97f, 1.0f);
}

void Particle::UpdateParticle(float deltaTime)
{
	vertexInfo.position = vertexInfo.position + velocity*deltaTime;
	velocity *= velocityDampValue * 0.999f;
	//glm::vec3 diff = vertexInfo.position - prevLoc;
	//
	////lifetime
	particleLifetime = glm::max(0.0f, particleLifetime - deltaTime);

	//vertexInfo.color = glm::vec4(vertexInfo.color.x, vertexInfo.color.y, vertexInfo.color.z, particleLifetime / maxLifetime);
	////std::cout << diff.x <<" "<< diff.y << " " << diff.z << std::endl;
	//prevLoc = vertexInfo.position;
}

