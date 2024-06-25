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

	particleLifetime = glm::linearRand(3.0f, 10.0f);
	//particleLifetime = 0.1f;
	maxLifetime = particleLifetime;

	velocityDampValue = glm::linearRand(0.97f, 1.0f)*0.999f;
}

void Particle::UpdateParticle(float deltaTime)
{
	if (particleLifetime == 0.0f) return;
	vertexInfo.position = vertexInfo.position + velocity*deltaTime*3.0f;
	velocity *= velocityDampValue;
	//glm::vec3 diff = vertexInfo.position - prevLoc;
	//
	////lifetime
	particleLifetime = particleLifetime - deltaTime;

	//vertexInfo.color = glm::vec4(vertexInfo.color.x, vertexInfo.color.y, vertexInfo.color.z, particleLifetime / maxLifetime);
	////std::cout << diff.x <<" "<< diff.y << " " << diff.z << std::endl;
	//prevLoc = vertexInfo.position;
}

