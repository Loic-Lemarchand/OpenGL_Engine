#include "window.h"

#include "utilities.h"

float  Window::triOffset = 0.0f;
float  Window::triIncrement = 0.0f;

Window::Window(int width, int height, const char* title, InputManager& inputManager) :
	bIsValid(true),
	myWindow(nullptr),
	myErrorCode(0),
	myInputManager(inputManager),
	myFrameBufferHeight(0),
	myFrameBufferWidth(0)
{
	triIncrement = 0.0f;
	//glfwInitHint(GLFW_ANGLE_PLATFORM_TYPE, GLFW_ANGLE_PLATFORM_TYPE_OPENGL);
	glfwSetErrorCallback(Window::errorCallback);

	if (!glfwInit())
	{
		const char* description;
		myErrorCode = glfwGetError(&description);
		utilities::log("Error while intitializing the window, the error is" + std::string(description));
		bIsValid = false;
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	myWindow = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!myWindow)
	{
		utilities::log("Error while creating window");
		bIsValid = false;
		return;
	}

	glfwMakeContextCurrent(myWindow);

	//OPENGL
	int version = gladLoadGL();
	if (!version)
	{
		utilities::log("Error while loading OpenGL");
		bIsValid = false;
		myWindow = nullptr;
		return;
	}


	glfwSetKeyCallback(myWindow, Window::key_callback);

	//pass the size of glfw window to opengl (the viewport takes the whole frame buffer)
	
	glfwGetFramebufferSize(myWindow, &myFrameBufferWidth, &myFrameBufferHeight);
	glViewport(0, 0, myFrameBufferWidth, myFrameBufferHeight);

	glfwSetFramebufferSizeCallback(myWindow, framebuffer_size_callback);

	
	glfwSwapInterval(1);

	utilities::log("Successfully created window");

	glfwSetWindowUserPointer(myWindow, this);
	auto& eventBus = myInputManager.getEventBus();
	eventBus.subscribe(EventDispatcher::EventType::KeyPressed,
		[this](const EventDispatcher::Event& e)
		{
			const KeyPressedEvent& key = static_cast<const KeyPressedEvent&>(e);

			if (key.getKey() == GLFW_KEY_ESCAPE)
				quitWindow();
			else if (key.getKey() == GLFW_KEY_LEFT)
				rotateLeft();
			else if (key.getKey() == GLFW_KEY_RIGHT)
				rotateRight();
		});

	eventBus.subscribe(EventDispatcher::EventType::KeyReleased,
		[this](const EventDispatcher::Event& e)
		{
			const KeyReleasedEvent& key = static_cast<const KeyReleasedEvent&>(e);
			triIncrement = 0;
		});
}

void Window::update()
{
	glfwPollEvents();
	glfwSwapBuffers(myWindow);

	//if(!(abs(triOffset + triIncrement) > triMaxOffset))
	{
		triOffset += triIncrement;
	}
}

Window::~Window()
{
	utilities::log("Destroy window");
	if(myWindow)
		glfwDestroyWindow(myWindow);

	glfwTerminate();

}


//Callbacks function

void Window::errorCallback(int code, const char* description)
{

}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (!win)
	{
		utilities::log("Couldn't get window instance");
		return;
	}
		
	win->myInputManager.processKey(key, action);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

