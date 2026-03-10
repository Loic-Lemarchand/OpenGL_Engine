
#include "renderer.h"

Renderer::Renderer() : bIsValid(true)
{
	createBuffers();
}

Renderer::~Renderer()
{

}

void Renderer::createBuffers()
{
	
	glGenVertexArrays(1, &myVAO);

	glBindVertexArray(myVAO);

	glGenBuffers(1, &myVBO);


	glBindBuffer(GL_ARRAY_BUFFER, myVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	//Check compilation success
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		utilities::log("Failed to compile vertex shader : " + std::string(infoLog));
		bIsValid = false;
		return;
	}


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		utilities::log("Failed to compile fragment shader : " + std::string(infoLog));
		bIsValid = false;
		return;
	}
	

	//Link both shaders in the shader program

	myShaderProgram = glCreateProgram();

	glAttachShader(myShaderProgram, vertexShader);
	glAttachShader(myShaderProgram, fragmentShader);
	glLinkProgram(myShaderProgram);

	glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(myShaderProgram, 512, NULL, infoLog);
		utilities::log("Failed to link program : " + std::string(infoLog));
		bIsValid = false;
		return;
	}
	

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//p1 : location is 0 in the vertex shader so it is 0
	//p2 : 3 because of vec3
	//p3 : glsl vec are float values
	//p4 : No normalization of data
	//p5 : Stride - the space beetween consecutive vertex attribute (basically the size of a single vertex)
	//p6 : Offset of the beginning of the position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(myVAO);

	utilities::log("Successfully created renderer");
}

void Renderer::update()
{
	glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaderProgram);

		glBindVertexArray(myVAO);
	
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

	glUseProgram(0);
}