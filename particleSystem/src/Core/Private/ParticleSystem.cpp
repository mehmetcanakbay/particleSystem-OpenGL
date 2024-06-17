#include "Public/ParticleSystem.h"
#include <iostream>

void ParticleSystem::UpdateParticles(float deltaTime) {
	for (int i = 0; i < m_Count; i++) {
		particlePool[i].UpdateParticle(deltaTime);

		glm::vec3 pos = particlePool[i].ReturnPosition();
		posLifetimeArray[i].x = pos.x;
		posLifetimeArray[i].y = pos.y;
		posLifetimeArray[i].z = pos.z;
		posLifetimeArray[i].w = particlePool[i].ReturnLifetime();
	}
}

void ParticleSystem::ThreadJob(int start, int end, float deltaTime) {
	for (int i = start; i < end; i++) {
		particlePool[i].UpdateParticle(deltaTime);

		glm::vec3 pos = particlePool[i].ReturnPosition();
		posLifetimeArray[i].x = pos.x;
		posLifetimeArray[i].y = pos.y;
		posLifetimeArray[i].z = pos.z;
		posLifetimeArray[i].w = particlePool[i].ReturnLifetime();
	}
}

ParticleSystem::ParticleSystem(unsigned int count, float particleSize)
:m_Count(count), m_particleSize(particleSize) {
	
	particlePool = new Particle[m_Count];
	posLifetimeArray = new glm::vec4[m_Count];
}

ParticleSystem::~ParticleSystem()
{
	delete[] particlePool;
	delete[] posLifetimeArray;
}


//////////////////////////////////////RENDERER
ParticleSystemRenderer::ParticleSystemRenderer(ParticleSystem* particleSystem) :
	vertexBuffer_id(0), vertexArray_id(0), indexBuffer_id(0), partSystemRef(particleSystem), pool(4)
{
	partSystemRef->UpdateParticles(0.0005f);
	glGenVertexArrays(1, &vertexArray_id);
	glBindVertexArray(vertexArray_id);
	//VERTEX BUFFER
	glGenBuffers(1, &vertexBuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &quadSource[0], GL_STATIC_DRAW);

	//VERTEX ARRAY
	//position first
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	
	//instanced array creation and supplying the vertex array with it
	glEnableVertexAttribArray(1);
	//created vb
	glGenBuffers(1, &instancedVertexBuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVertexBuffer_id);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * partSystemRef->GetCount(), &(partSystemRef->posLifetimeArray[0]), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //0 offset because new vertex buffer
	glVertexAttribDivisor(1, 1);

	//INDEX BUFFER
	glGenBuffers(1, &indexBuffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), &quadIndicesSource[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Unbind();
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
	Unbind();
	glDeleteBuffers(1, &indexBuffer_id);
	glDeleteBuffers(1, &vertexBuffer_id);
	glDeleteVertexArrays(1, &vertexArray_id);
}

void ParticleSystemRenderer::Bind()
{
	glBindVertexArray(vertexArray_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_id);
}

void ParticleSystemRenderer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void ParticleSystemRenderer::Render(float deltaTime)
{
	pool.addTask(&ParticleSystem::ThreadJob, partSystemRef);
	//partSystemRef->UpdateParticles(deltaTime);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVertexBuffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float) * partSystemRef->GetCount(), &(partSystemRef->posLifetimeArray[0]));
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL, partSystemRef->GetCount());
}
