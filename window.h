#pragma once
#include <glad/glad.h>

#include "GLFW/glfw3.h"

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	Window operator=(const Window& other) = delete;

	bool shoudClose() { return glfwWindowShouldClose(myWindow); }
	double getGLFWTime() { return glfwGetTime(); }

	void update();

	//It's okay to be public since not read internally
	bool bIsValid;

private:

	GLFWwindow* myWindow;

	int myErrorCode;


	//callback functions
	static void errorCallback(int code, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	

};

