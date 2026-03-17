#pragma once

extern float deltaTime;
extern float lastFrame;
extern bool firstMove;

#include <string>
#include <iostream>


namespace utilities
{
	void log(const std::string& text);
}