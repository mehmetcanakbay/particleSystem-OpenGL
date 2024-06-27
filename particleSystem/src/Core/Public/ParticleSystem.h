#pragma once
#include "Public/Renderer.h"
#include "Public/Vertex.h"
#include "Public/Particle.h"
#include <thread>
#include "Public/ThreadPool.hpp"

class ParticleSystem {
private:
	unsigned int m_Count;
	float m_particleSize;

	Particle* particlePool;

public:
	ParticleSystem(unsigned int count, float particleSize);
	ParticleSystem() {};
	void Initialize(unsigned int count, float particleSize);

	~ParticleSystem();

	inline unsigned int GetCount() const { return m_Count; }
	void UpdateParticles(float deltaTime);
	void ThreadJob(int start, int end, float deltaTime, float* mappedData);

public:
	std::atomic<unsigned int> finishedThreadCount = 0;
};

class ParticleSystemRenderer {

private:
	unsigned int vertexBuffer_id;
	unsigned int vertexArray_id;
	unsigned int indexBuffer_id;

	unsigned int instancedVertexBuffer_id;

	unsigned int numThreads;
	unsigned int threadChunks;
	ThreadPool pool;


	ParticleSystem* partSystemRef;

	const glm::vec3 quadSource[4] = {
		{-0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f}
	};

	const uint32_t quadIndicesSource[6] = {
		0, 1, 2, 2, 3, 0
	};

	//debug variables
	unsigned int frameCount = 0;
	unsigned int skippedCount = 0;


private:
	void SendOrder(int start, int end, float deltaTimeRef, float* mappedData);
	void CreateBuffers();
public:
	ParticleSystemRenderer(ParticleSystem* particleSystem);
	ParticleSystemRenderer() {};
	void Initialize(ParticleSystem* particleSystem, int numberOfThreads=4);
	~ParticleSystemRenderer();

	void Bind();
	void Unbind();


	bool Render(float deltaTime);
};