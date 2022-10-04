#include <gl\glew.h>
#include "GLWindow.h"
#include "ShaderLoader.h"
//#include <iostream>

void GLWindow::initializeGL() {
	glewInit();
	initData();
	sendData();
	installShaders();
}

/// <summary>
/// Data Setup; Initializing Shaders
/// </summary>
void GLWindow::initData()
{
	deltaTime = 0.2f;
	speed1 = 0.5f;
	speed2 = 0.5f;
	vel1 = { 0.0f, 0.0f };
	vel2 = { 0.0f, 0.0f };
	scale1 = { 1.0f, 1.0f };
	scale2 = { 1.5f, 1.5f };
	offset1 = { 0.4f, 0.1f };
	offset2 = { -0.4f, 0.1f };
	color1 = { 0.9f, 0.3f, 0.0f };
	color2 = { 0.0f, 0.3f, 0.9f };
}

void GLWindow::sendData() {
	Vertex vertices[] = {
		+0.0f, +0.1f,
		+0.2f, +0.5f, +0.8f,
		-0.05f, -0.05f,
		+0.4f, +0.8f, +0.2f,
		+0.05f, -0.05f,
		+0.8f, +0.2f, +0.4f,

		+0.9f, +0.9f,
		+0.8f, +0.8f, +0.8f,
		+0.9f, -0.9f,
		+0.8f, +0.8f, +0.8f,
		-0.9f, -0.9f,
		+0.8f, +0.8f, +0.8f,
		-0.9f, +0.9f,
		+0.8f, +0.8f, +0.8f,
	};

	GLushort indices[] = { 0,1,2, 3,4,5,6 };
	

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	// Buffer Data needs a buffer binding point = GL_ARRAY_BUFFER (or GL_ELEMENT_ARRAY_BUFFER) instead of buffer id
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*7 + sizeof(indices), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vertex) * 7, sizeof(indices), indices);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));
}

void GLWindow::installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	ShaderSource source = GetShaderSource("resources/shaders/BasicTransformation.shader");
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

	colorId = glGetUniformLocation(programID, "color");
	scaleId = glGetUniformLocation(programID, "scale");
	offsetId = glGetUniformLocation(programID, "offset");

	glUseProgram(programID);
}

/// <summary>
/// Painting the Window
/// </summary>
void GLWindow::paintGL() {
	offset1 = translatePos(offset1, vel1 * deltaTime, Vec4{ -0.8f, 0.8f, -0.8f, 0.8f });
	offset2 = translatePos(offset2, vel2 * deltaTime, Vec4{ -0.8f, 0.8f, -0.8f, 0.8f });

	//std::cout << "{" << offset1.x  << ", " << offset1.y << "}" << std::endl;
	glViewport(width() / 4, 0, width() / 2, height());
	glClearColor(+0.9f, +0.8f, 0.4f, +0.1f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Box
	glUniform3f(colorId, 0.5f, 0.7f, 0.5f);
	glUniform2f(scaleId, 1.0f, 1.0f);
	glUniform2f(offsetId, 0.0f, 0.0f);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(Vertex) * 7 + sizeof(GLshort)*3));

	// Player 1
	glUniform3f(colorId, color1.x, color1.y, color1.z);
	glUniform2f(scaleId, scale1.x, scale1.y);
	glUniform2f(offsetId, offset1.x, offset1.y);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(Vertex) * 7));

	// Player 2
	glUniform3f(colorId, color2.x, color2.y, color2.z);
	glUniform2f(scaleId, scale2.x, scale2.y);
	glUniform2f(offsetId, offset2.x, offset2.y);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(Vertex) * 7));

}


Vec2 GLWindow::translatePos(Vec2 initialPos, Vec2 speed, Vec4 bounds)
{
	float xPos = initialPos.x + speed.x * deltaTime;
	float yPos = initialPos.y + speed.y * deltaTime;
	if (xPos < bounds.x) xPos = bounds.x;
	if (xPos > bounds.y) xPos = bounds.y;
	if (yPos < bounds.z) yPos = bounds.z;
	if (yPos > bounds.w) yPos = bounds.w;
	return { xPos, yPos };
}

/// <summary>
/// Input Handling
/// </summary>
/// <param name="event"></param>
void GLWindow::keyPressEvent(QKeyEvent* event) {
	handleInput(event, true);
}

void GLWindow::keyReleaseEvent(QKeyEvent* event) {
	handleInput(event, false);
}

void GLWindow::handleInput(QKeyEvent* event, bool pressed)
{
	float factor = pressed ? 1.0f : -0.0f;
	float forceFactor = 500;

	switch (event->key()) {
	case Qt::Key::Key_W: // W
		vel1 = (Vec2{ 0, speed1 }*factor);
		break;
	case 0x0041: // A
		vel1 = (Vec2{ -speed1, 0.0f }*factor);
		break;
	case 0x0053: // S
		vel1 = (Vec2{ 0, -speed1 }*factor);
		break;
	case 0x0044: // D
		vel1 = (Vec2{ speed2, 0}*factor);
		break;
	case Qt::Key::Key_Up: // W
		vel2 = (Vec2{ 0, speed2 }*factor);
		break;
	case Qt::Key::Key_Left: // A
		vel2 = (Vec2{ -speed2, 0.0f }*factor);
		break;
	case Qt::Key::Key_Down: // S
		vel2 = (Vec2{ 0, -speed2 }*factor);
		break;
	case Qt::Key::Key_Right: // D
		vel2 = (Vec2{ speed2, 0 }*factor);
		break;
	}
	//std::cout << "{" << vel1.x << ", " << vel1.y << "}" << std::endl;
	repaint();
}