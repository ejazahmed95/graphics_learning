#include <gl\glew.h>
#include "GLWindow.h"

void GLWindow::initializeGL() {
	glewInit();
	sendData();
	installShaders();
}

void GLWindow::installShaders() {

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

void GLWindow::paintGL() {
	glViewport(0, 0, width(), height());
	glClearColor(0.2f, 0.2f, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
