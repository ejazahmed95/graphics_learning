#include <gl\glew.h>
#include "GLWindow.h"
#include "ShaderLoader.h"

void GLWindow::initializeGL() {
	glewInit();
	sendData();
	installShaders();
}

void GLWindow::sendData() {
	GLfloat vertices[] = {
		0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, -0.5f,
		1.0f, 0.0f, 0.0f,
		-1.0f, -0.5f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.5f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.5f,
		0.0f, 1.0f, 0.0f,
	};

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Buffer Data needs a buffer binding point = GL_ARRAY_BUFFER (or GL_ELEMENT_ARRAY_BUFFER) instead of buffer id
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));

	GLushort indices[] = { 0,1,4, 0,3,2 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

	/*glCreateShader(GL_VERTEX_SHADER);
	glCreateShader(GL_FRAGMENT_SHADER);*/
}

void GLWindow::installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	ShaderSource source = GetShaderSource("resources/shaders/BasicColorPassing.shader");
	/*std::cout << "Vertex Shader::\n" << source.VertexSource << std::endl;
	std::cout << "Fragment Shader::\n" << source.FragmentSource << std::endl;*/
	const char* adapter[1];
	adapter[0] = source.VertexSource.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = source.FragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glUseProgram(programID);
}

void GLWindow::paintGL() {
	glViewport(0, 0, width(), height());
	glClearColor(0.2f, 0.2f, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
