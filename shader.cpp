#include "shader.h"
#include "utilities.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) : bIsValid(true)
{
	std::ifstream vertexFile(vertexPath);
	std::ifstream fragmentFile(fragmentPath);

	if (!vertexFile.is_open()) {
		utilities::log("Failed to open Vertex Shader file. Provided path was : " + std::string(vertexPath));
		bIsValid = false;
		return;
	}

	if (!fragmentFile.is_open()) {
		utilities::log("Failed to open Fragment Shader file. Provided path was : " + std::string(fragmentPath));
		bIsValid = false;
		return;
	}

	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vertexFile.rdbuf();
	fShaderStream << fragmentFile.rdbuf();

	vertexFile.close();
	fragmentFile.close();

	myProgramID = glCreateProgram();

	GLuint vertexShader = AddShader(vShaderStream.str().c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = AddShader(fShaderStream.str().c_str(), GL_FRAGMENT_SHADER);

	//Check compilation / linking success
	int success;
	char infoLog[512];
	

	glLinkProgram(myProgramID);

	glGetProgramiv(myProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(myProgramID, sizeof(infoLog), NULL, infoLog);
		utilities::log("Failed to link program : " + std::string(infoLog));
		bIsValid = false;
		return;
	}

	glValidateProgram(myProgramID);
	glGetProgramiv(myProgramID, GL_VALIDATE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(myProgramID, sizeof(infoLog), NULL, infoLog);
		utilities::log("Error Validating program : " + std::string(infoLog));
		bIsValid = false;
		return;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

GLuint Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, theCode, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		utilities::log("Error compiling" + std::to_string(shaderType) + " : " + eLog);
		bIsValid = false;
		return -1;
	}

	glAttachShader(myProgramID, shader);

	return shader;
}

void Shader::use()
{
	glUseProgram(myProgramID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(myProgramID, name.c_str()), (int)value);
}


void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(myProgramID, name.c_str()), value);
}


void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(myProgramID, name.c_str()), value);
}