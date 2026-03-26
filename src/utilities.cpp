#include "utilities.h"


float lastFrame = 0.0f;
float deltaTime = 0.0f;
bool firstMove = true;

namespace utilities
{
	void log(const std::string& text)
	{
		std::cout << text << std::endl;
	}
}

