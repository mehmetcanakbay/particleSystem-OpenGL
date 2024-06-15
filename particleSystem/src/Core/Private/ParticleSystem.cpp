#include "Public/ParticleSystem.h"

void ParticleSystem::CreateQuadsFromPositions() {
	int vertexIndex = -1;
	for (int i = 0; i < m_Count * 12; i++) {
		if (i % 12 == 0) vertexIndex += 1;
		//for every small position (like -0.5f)
		//get the correct vertex position to add from vertexes and then add the quad pos there
		quadPositions[i] = particlePool[vertexIndex].ReturnPosition()[i % 3] +
			(quadSource[i % 12] * m_particleSize);
	}
}

//I think as a result this may become slower than the apply velocity.
//TODO: Benchmark both solutions in a new branch.
void ParticleSystem::UpdateParticlePositions() {
	for (int i = 0; i < m_Count; i++) {
		particlePool[i].UpdateParticle();
	}
}

//this would work but I dont think its smart to do it this way
//position should change in the particle, and I should just set the positions in here.
/*
void ParticleSystem::ApplyVelocity()
{
	int vertexIndex = -1;
	for (int i = 0; i < m_Count * 12; i++) {
		if (i % 12 == 0) vertexIndex += 1;
		quadPositions[i] += particlePool[vertexIndex].ReturnVelocity()[i % 3];
	}
}
*/

void ParticleSystem::CreateIndices()
{
	unsigned int fullCycle = 0;
	for (unsigned int i = 5; i < 6 * m_Count; i += 6) {
		quadIndices[i - 5] = quadIndicesSource[0] + fullCycle * 4;
		quadIndices[i - 4] = quadIndicesSource[1] + fullCycle * 4;
		quadIndices[i - 3] = quadIndicesSource[2] + fullCycle * 4;
		quadIndices[i - 2] = quadIndicesSource[3] + fullCycle * 4;
		quadIndices[i - 1] = quadIndicesSource[4] + fullCycle * 4;
		quadIndices[i] = quadIndicesSource[5] + fullCycle * 4;

		fullCycle += 1;
	}
}



ParticleSystem::ParticleSystem(unsigned int count, float particleSize)
:m_Count(count), m_particleSize(particleSize) {
	
	quadPositions = new float[12 * m_Count];
	quadIndices = new uint32_t[6 * m_Count];
	particlePool = new Particle[m_Count];

	CreateQuadsFromPositions();
	CreateIndices();
}

ParticleSystem::~ParticleSystem()
{
	delete[] quadPositions;
	delete[] quadIndices;
	delete[] particlePool;
}


//////////////////////////////////////RENDERER
ParticleSystemRenderer::ParticleSystemRenderer(ParticleSystem* particleSystem):
	vertexBuffer_id(0), vertexArray_id(0), indexBuffer_id(0), partSystemRef(particleSystem)
{
	//VERTEX BUFFER
	glGenBuffers(1, &vertexBuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float) * particleSystem->GetCount(), particleSystem->GetQuadPositions(), GL_DYNAMIC_DRAW);

	//VERTEX ARRAY
	glGenVertexArrays(1, &vertexArray_id);
	glBindVertexArray(vertexArray_id);
	glEnableVertexAttribArray(0);
	//position first
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//color
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(3*sizeof(float)));

	//INDEX BUFFER
	glGenBuffers(1, &indexBuffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * particleSystem->GetCount() * sizeof(unsigned int), particleSystem->GetQuadIndices(), GL_DYNAMIC_DRAW);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_id);
	glBindVertexArray(vertexArray_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
}

void ParticleSystemRenderer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ParticleSystemRenderer::Render()
{
	partSystemRef->CreateQuadsFromPositions();
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float) * partSystemRef->GetCount(), partSystemRef->GetQuadPositions());
	glDrawElements(GL_TRIANGLES, partSystemRef->GetCount() * 6, GL_UNSIGNED_INT, nullptr);
}
