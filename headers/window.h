#pragma once
#include <glad/glad.h>
#include "inputManager.h"
#include "GLFW/glfw3.h"

#define triMaxOffset 0.7f


class Window
{
public:
	Window(int width, int height, const char* title, InputManager& inputManager);
	~Window();

	Window operator=(const Window& other) = delete;

	bool shoudClose() { return glfwWindowShouldClose(myWindow); }
	double getGLFWTime() { return glfwGetTime(); }
	GLFWwindow* getGLFWWindow() { return myWindow; }

	int getFrameBufferWidth() { return myFrameBufferWidth; }
	int getFrameBufferHeight() { return myFrameBufferHeight; }

	void update();

	//It's okay to be public since not read internally
	bool bIsValid;
	static float triOffset;

private:

	GLFWwindow* myWindow;

	int myErrorCode;

	int myFrameBufferWidth, myFrameBufferHeight;
	
	static float triIncrement;

	//Mouse infos
	bool mouseFirstMoved;
	double mouseLastX, mouseLastY;
	


	InputManager& myInputManager;

	//GLFW callback functions
	static void errorCallback(int code, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);



	//Events 
	void quitWindow() 
	{
		//utilities::log("Escape pressed");
		glfwSetWindowShouldClose(myWindow, GLFW_TRUE);
	}

	void rotateLeft()
	{
		triIncrement = -0.5f;
	}

	void rotateRight()
	{
		triIncrement = 0.5f;
	}

	void releaseMovement()
	{
		triIncrement = 0.0f;
	}
};

