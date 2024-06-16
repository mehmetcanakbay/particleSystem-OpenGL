#pragma once
#include "Public/Renderer.h"
#include "Public/Vertex.h"
#include "Public/Particle.h"

class ParticleSystem {
private:
	unsigned int m_Count;
	float m_particleSize;
	Vertex* quadVertexes; //lets initialize this later
	uint32_t* quadIndices;

	/*
	const float quadSource[12] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	*/
	const glm::vec3 quadSource[4] = {
		{-0.5f, -0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f},
		{0.5f, 0.5f, 0.0f},
		{-0.5f, 0.5f, 0.0f}
	};

	const uint32_t quadIndicesSource[6] = {
		0, 1, 2, 2, 3, 0
	};

	Particle* particlePool;

	//VertexFactory factory;
	//void ApplyVelocity();

public:
	ParticleSystem(unsigned int count, float particleSize);
	~ParticleSystem();

	inline unsigned int GetCount() const { return m_Count; }
	void UpdateParticlePositions();
	void CreateQuadsFromPositions();
	void CreateIndices();

	inline Vertex* GetQuadVertexes() const { return quadVertexes; }
	inline uint32_t* GetQuadIndices() const { return quadIndices; }
};

class ParticleSystemRenderer {

private:
	unsigned int vertexBuffer_id;
	unsigned int vertexArray_id;
	unsigned int indexBuffer_id;

	ParticleSystem* partSystemRef;
public:
	ParticleSystemRenderer(ParticleSystem* particleSystem);
	~ParticleSystemRenderer();

	void Bind();
	void Unbind();

	void Render();
};