#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "shader.hpp"

// We will use this function to grab our shader code
std::string shader::loadFile(std::string filename)
{
	std::string buffer{};
	// Next we are going to open the file
	std::ifstream fileStream(filename.c_str(), std::ios::in);
	// Next we are going to load the file or throw an error if someone is an idiot
	if (fileStream.is_open())
	{
		// We will use a string stream to grab all the data
		std::stringstream sstr;
		sstr << fileStream.rdbuf();
		// Then we will assign the data to buffer
		buffer = sstr.str();
		// Then close the file
		fileStream.close();
	}
	else
	{
		std::cout << filename << " failed to load!\n";
	}
	return buffer;
}

void shader::compileShader(const char* sourcePtr, GLuint* ID)
{
	glShaderSource(*ID, 1, &sourcePtr, NULL);
	glCompileShader(*ID);
}

GLuint shader::createShader(GLuint* vert, GLuint* frag)
{
	// Create a program and link the shaders to it
	GLuint ProgID = glCreateProgram();
	glAttachShader(ProgID, *vert);
	glAttachShader(ProgID, *frag);
	glLinkProgram(ProgID);
	glDetachShader(ProgID, *vert);
	glDetachShader(ProgID, *frag);
	glDeleteShader(*vert);
	glDeleteShader(*frag);
	return ProgID;
}

// This is the function we will use the load our shaders
void shader::loadShader(std::string vertexFile, std::string fragmentFile)
{
	// We are going to create a vertex and fragment shader
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	// We are going to use a string to hold our shader code
	std::string vertCode = loadFile(vertexFile);
	std::string fragCode = loadFile(fragmentFile);
	// Compile the shaders
	compileShader(vertCode.c_str(), &vertShader);
	compileShader(fragCode.c_str(), &fragShader);
	// Link the program
	programID = createShader(&vertShader, &fragShader);
}

void shader::useShader()
{
	glUseProgram(programID);
}