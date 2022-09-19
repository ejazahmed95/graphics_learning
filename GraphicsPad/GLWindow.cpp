#include <gl\glew.h>
#include "GLWindow.h"

void GLWindow::initializeGL() {
	glewInit();

	GLfloat vertices[] = {
		+0.0f, +1.0f,
		-1.0f, -1.0f,
		+1.0f, -1.0f,
	};

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Buffer Data needs a buffer binding point = GL_ARRAY_BUFFER (or GL_ELEMENT_ARRAY_BUFFER) instead of buffer id
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLWindow::paintGL() {
	glViewport(0, 0, width(), height());
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
