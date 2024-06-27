#include "Public/ParticleSystem.h"
#include <iostream>

void ParticleSystem::UpdateParticles(float deltaTime) {
	for (int i = 0; i < m_Count; i++) {
		particlePool[i].UpdateParticle(deltaTime);

		glm::vec3 pos = particlePool[i].ReturnPosition();
		posLifetimeArray[i].x = pos.x;
		posLifetimeArray[i].y = pos.y;
		posLifetimeArray[i].z = pos.z;
		posLifetimeArray[i].w = particlePool[i].ReturnAlpha();
	}
}


void ParticleSystem::ThreadJob(int start, int end, float deltaTime, float* mappedData) {
	for (int i = start; i < end; i++) {
		Particle* particle = &particlePool[i];
		if (particle->ReturnLifetime() <= 0.0f) continue;
		particle->UpdateParticle(deltaTime);

		glm::vec3 pos = particle->ReturnPosition();
		mappedData[i * 4] = pos.x;
		mappedData[i * 4 + 1] = pos.y;
		mappedData[i * 4 + 2] = pos.z;
		mappedData[i * 4 + 3] = particle->ReturnAlpha();
		//if (i % 10000 == 0)
		//	std::cout << "Thread id: " << std::this_thread::get_id() << " i = " << i <<" delta time: "<<*deltaTime<< std::endl;
	}

	finishedThreadCount += 1;
	//std::cout << "end!! :" << std::this_thread::get_id() << std::endl;
}

ParticleSystem::ParticleSystem(unsigned int count, float particleSize)
:m_Count(count), m_particleSize(particleSize) {
	
	particlePool = new Particle[m_Count];
}

void ParticleSystem::Initialize(unsigned int count, float particleSize) {
	m_Count = count;
	m_particleSize = particleSize;
	particlePool = new Particle[m_Count];
}

ParticleSystem::~ParticleSystem()
{
	delete[] particlePool;
	delete[] posLifetimeArray;
}


//////////////////////////////////////RENDERER
ParticleSystemRenderer::ParticleSystemRenderer(ParticleSystem* particleSystem) :
	vertexBuffer_id(0), vertexArray_id(0), indexBuffer_id(0), partSystemRef(particleSystem), numThreads(4), pool(numThreads)
{
	CreateBuffers();
	threadChunks = partSystemRef->GetCount() / numThreads;
	partSystemRef->finishedThreadCount = 0;
}

void ParticleSystemRenderer::Initialize(ParticleSystem* particleSystem, int numberOfThreads) {

	vertexBuffer_id = 0;
	vertexArray_id = 0;
	indexBuffer_id = 0;
	partSystemRef = particleSystem;
	numThreads = numberOfThreads;
	pool.InitializePool(numThreads);

	CreateBuffers();

	threadChunks = partSystemRef->GetCount() / numThreads;
	partSystemRef->finishedThreadCount = 0;
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
	Unbind();
	glDeleteBuffers(1, &indexBuffer_id);
	glDeleteBuffers(1, &vertexBuffer_id);
	glDeleteVertexArrays(1, &vertexArray_id);
	//pool.stop();
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

void ParticleSystemRenderer::SendOrder(int start, int end, float deltaTimeRef, float* mappedData) {
	partSystemRef->ThreadJob(start, end, deltaTimeRef, mappedData);
}

void ParticleSystemRenderer::CreateBuffers()
{
	glGenVertexArrays(1, &vertexArray_id);
	glBindVertexArray(vertexArray_id);
	//VERTEX BUFFER
	glGenBuffers(1, &vertexBuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &quadSource[0], GL_STATIC_DRAW);

	//VERTEX ARRAY
	//position first
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
	Unbind();
}

bool ParticleSystemRenderer::Render(float deltaTime)
{
	frameCount += 1;

	glBindBuffer(GL_ARRAY_BUFFER, instancedVertexBuffer_id);
	float* mappedData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < numThreads; ++i) {
		int start = i * threadChunks;
		int end = (i == numThreads - 1) ? partSystemRef->GetCount() : (i + 1) * threadChunks;
		pool.enqueue([this, start, end, deltaTime, mappedData] {
			SendOrder(start, end, deltaTime, mappedData);
		});	
	}
	while (partSystemRef->finishedThreadCount < numThreads);
	partSystemRef->finishedThreadCount = 0;
	//std::cout << "UNMAPPING BUFFER" << std::endl;
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL, partSystemRef->GetCount());
	return true;

}
