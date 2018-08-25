#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "bus.hpp"
#include "renderer.hpp"
#include "shader.hpp"


//We are going to init our renderer
void renderer::init(bus* Bus)
{
	// Our engine only needs one renderer, so it's safe to assume its name.
	// We point its node to a bus and call it renderer
	Node = new node(Bus, "renderer");
	// We are enabling experimental glew features to take advantage of modern OpenGL
	glewExperimental = true;
	// We need to initialise GLFW. GLFW is a helpful wrapper that allows for easy window creation
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
	// We call the createWindow function. It creates a 1920x1080 window called renderer
	Window = glfwCreateWindow(1920, 1080, "renderer", NULL, NULL);
	glfwMakeContextCurrent(Window);
	// We also initialise glew, which is the GL extension wrangler. Important if we use extensions
	glewInit();
}

// Next is our event handler.
// Right now we don't have any events that we need to handle
// Currently the event handler is just polling for messages and the Command variable reads the messages and separates flags
// In the future we may run into a situation where many events are being sent to the event handler at once.
// In our current setup, we handle one event per "frame" or run of the main loop
// We may need to create a loop that will handle multiple events at once.
// We can do this by checking the number of messages our node currently has and looping through all of them

void renderer::event()
{
	Node->update();
	Command.readString(Node->getMessage());
}

// We have a super simple update function at the moment. Currently we poll for events
// And then we clear the screen, swap buffers, and have glfw poll for any input

void renderer::update()
{
	event();
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Tell the renderer to update
	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
	glfwSwapBuffers(Window);
	glfwPollEvents();
}
