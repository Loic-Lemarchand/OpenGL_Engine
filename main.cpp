#include "window.h"
#include "utilities.h"
#include "renderer.h"
#include <memory>


int main(int argc, char* argv[])
{
	std::unique_ptr<Window> window = std::make_unique<Window>(640, 720, "My Window");
	std::unique_ptr<Renderer> renderer = nullptr;


	if (!window->bIsValid)
	{
		utilities::log("Rindow is not valid");
	}

	renderer = std::make_unique<Renderer>();

	if (!renderer->bIsValid)
	{
		utilities::log("Renderer is not valid");
	}

	while (!window->shoudClose() && window->bIsValid)
	{
		window->update();

		//RENDERER WORK HERE

		renderer->update();


		//PHYSICS COMPUTATION HERE

	
	}
}