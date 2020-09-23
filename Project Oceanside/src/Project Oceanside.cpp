#include <time.h>
#include <chrono>
#include <iostream>

#include "Constants.h"
#include "Heap.h"
#include "Scene.h"

int main()
{
	// Set the filename of the scene file you wish to use.
	//std::string sceneFile = "scene_observatory_jp.json";
	//std::string sceneFile = "scene_woods_of_mystery_day1.json";
	std::string sceneFile = "scene_graveyard_day1.json";

	// Uncomment one line depending on your version.
	//char version = MM_JP0;
	char version = MM_JP1;
	//char version = MM_US;

	// Initialize version-specific information
	int heapStart = MM_JP0_START;
	int linkSize = MM_JP_LINK_SIZE;

	// Adjust version-specific information based on the version.
	if (version == MM_JP1)
	{
		heapStart = MM_JP1_START;
	}
	else if (version == MM_US)
	{
		heapStart = MM_US_START;
		linkSize = MM_US_LINK_SIZE;
	}

	// Create the scene and the heap.
	Scene* scene = new Scene(version, sceneFile);
	Heap* heap = new Heap(scene, heapStart, linkSize);

	// Solve the heap.
	heap->SolveGraveyardGrotto();

	// Delete the scene and the heap.
	delete(scene);
	delete(heap);

	// Return.
	return 0;
}
