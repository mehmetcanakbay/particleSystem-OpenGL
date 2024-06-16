#include "Public/ParticleSystem.h"
#include <iostream>

void ParticleSystem::CreateQuadsFromPositions() {
	//*12 for positions, but now add colors
	//so thats 4 more. in total it should be 28 7*4

	glm::vec3 scaledQuadSource[4];

	for (int i = 0; i < 4; i++) {
		scaledQuadSource[i] = quadSource[i] * m_particleSize;
	}

	//this saps quite a lot from FPS
	for (int i = 0; i < m_Count; i++) {
		int currCount = i * 4; //for every quad, there are 4 vertexes.

		glm::vec3 currPos = particlePool[i].ReturnPosition();
		glm::vec4 currCol = particlePool[i].ReturnColor();

		for (int j = 0; j < 4; j++) {
			quadVertexes[currCount + j].position = currPos + scaledQuadSource[j];
			quadVertexes[currCount + j].color = currCol;
		}
	}
}

//I think as a result this may become slower than the apply velocity.
//TODO: Benchmark both solutions in a new branch.
void ParticleSystem::UpdateParticlePositions(float deltaTime) {
	for (int i = 0; i < m_Count; i++) {
		particlePool[i].UpdateParticle(deltaTime);
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

void ParticleSystem::Tick(float deltaTime)
{
	glm::vec3 scaledQuadSource[4];

	for (int i = 0; i < 4; i++) {
		scaledQuadSource[i] = quadSource[i] * m_particleSize;
	}

	//this saps quite a lot from FPS
	for (int i = 0; i < m_Count; i++) {
		particlePool[i].UpdateParticle(deltaTime);
		
		int currCount = i * 4; //for every quad, there are 4 vertexes.

		glm::vec3 currPos = particlePool[i].ReturnPosition();
		glm::vec4 currCol = particlePool[i].ReturnColor();

		for (int j = 0; j < 4; j++) {
			quadVertexes[currCount + j].position = currPos + scaledQuadSource[j];
			quadVertexes[currCount + j].color = currCol;
		}
	}
}



ParticleSystem::ParticleSystem(unsigned int count, float particleSize)
:m_Count(count), m_particleSize(particleSize) {
	
	quadVertexes = new Vertex[4 * m_Count];
	quadIndices = new uint32_t[6 * m_Count];
	particlePool = new Particle[m_Count];

	CreateQuadsFromPositions();
	CreateIndices();
}

ParticleSystem::~ParticleSystem()
{
	delete[] quadVertexes;
	delete[] quadIndices;
	delete[] particlePool;
}

void ParticleSystem::ThreadJob(int start, int end) {
	glm::vec3 scaledQuadSource[4];

	for (int i = 0; i < 4; i++) {
		scaledQuadSource[i] = quadSource[i] * m_particleSize;
	}

	//this saps quite a lot from FPS
	for (int i = start; i < end; i++) {
		int currCount = i * 4; //for every quad, there are 4 vertexes.

		glm::vec3 currPos = particlePool[i].ReturnPosition();
		glm::vec4 currCol = particlePool[i].ReturnColor();

		for (int j = 0; j < 4; j++) {
			quadVertexes[currCount + j].position = currPos + scaledQuadSource[j];
			quadVertexes[currCount + j].color = currCol;
		}
	}
}


//////////////////////////////////////RENDERER
ParticleSystemRenderer::ParticleSystemRenderer(ParticleSystem* particleSystem):
	vertexBuffer_id(0), vertexArray_id(0), indexBuffer_id(0), partSystemRef(particleSystem)
{
	particleSystem->CreateQuadsFromPositions();
	particleSystem->CreateIndices();

	glGenVertexArrays(1, &vertexArray_id);
	glBindVertexArray(vertexArray_id);
	//VERTEX BUFFER
	glGenBuffers(1, &vertexBuffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_id);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex) * particleSystem->GetCount(), particleSystem->GetQuadVertexes(), GL_DYNAMIC_DRAW);

	//VERTEX ARRAY
	//position first
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float))); //3 because positions are 3dimensional

	//INDEX BUFFER
	glGenBuffers(1, &indexBuffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * particleSystem->GetCount() * sizeof(unsigned int), particleSystem->GetQuadIndices(), GL_DYNAMIC_DRAW);

	Unbind();

	numThreads = 4;
	threadChunks = partSystemRef->GetCount() / numThreads;
	/*
	for (int i = 0; i < numThreads; ++i) {
		int start = i * threadChunks;
		int end = (i == numThreads - 1) ? partSystemRef->GetCount() : (i + 1) * threadChunks;

		threads[i] = std::thread(&ParticleSystem::ThreadJob, partSystemRef, start, end);
	}

	for (std::thread& thread : threads) {
		thread.join();
	}
	*/
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


void ParticleSystemRenderer::Render()
{

	//for (std::thread& thread : threads) {
	//	thread.join();
	//}

	//partSystemRef->CreateQuadsFromPositions();
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex) * partSystemRef->GetCount(), partSystemRef->GetQuadVertexes());
	glDrawElements(GL_TRIANGLES, partSystemRef->GetCount() * 6, GL_UNSIGNED_INT, NULL);
}
