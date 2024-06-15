#include "Public/Vertex.h"

VertexFactory::VertexFactory(unsigned int amount, void (*positionFunc)(Vertex* pos, int index))
: m_Amount(amount){
    //heap allocate all the vertexes. 
    vertexPositions = std::vector<Vertex>(amount);

    if (positionFunc != nullptr) {
        for (int i = 0; i<amount; i++) {
            positionFunc(&vertexPositions[i], i); 
        }
    }
}
