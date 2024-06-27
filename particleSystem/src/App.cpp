#include "Public/Renderer.h"
#include "Public/ParticleLayer.h"


int main() {
	std::vector<Layer*> layerPointers;

	Renderer renderer;
	ParticleLayer particleLayer;

	//into layer pointers
	layerPointers.push_back(&particleLayer);

	//initialize windows etc.
	renderer.InitializeRenderer();
	
	//add the layers into the main renderer
	for (Layer* layerPtr : layerPointers) {
		layerPtr->OnInit();
		renderer.AddLayer(layerPtr);
	}

	//start rendering
	//this could be refactored, as you cannot kill the renderer properly in here
	renderer.StartRenderer();
}