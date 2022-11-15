#include <gl/glew.h>
#include "GLWindow.h"
#include "ShaderLoader.h"
#include <core/ShapeGenerator.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

glm::vec3 randomPositions[] = {
	   glm::vec3(3.0f,  0.0f,  -5.0f),
	   glm::vec3(2.0f,  5.0f, -10.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -8.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(2.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f),

	   glm::vec3(-2.5f,  1.7f, -1.8f),
	   glm::vec3(-3.0f,  -1.7f, -0.8f)
};

void GLWindow::initializeGL() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
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
	scale1 = { 0.1f, 0.1f };
	scale2 = { 0.15f, 0.15f };
	offset1 = { 0.4f, 0.1f };
	offset2 = { -0.4f, 0.1f };
	color1 = { 0.9f, 0.3f, 0.0f };
	color2 = { 0.0f, 0.3f, 0.9f };

	cubeRotation = 0;

	triangle = ShapeGenerator::Triangle();
	cube = ShapeGenerator::Cube();
}

void GLWindow::sendData() {
	Vertex vertices[] = {
		+0.9f, +0.9f,
		+0.8f, +0.8f, +0.8f,
		+0.9f, -0.9f,
		+0.8f, +0.8f, +0.8f,
		-0.9f, -0.9f,
		+0.8f, +0.8f, +0.8f,
		-0.9f, +0.9f,
		+0.8f, +0.8f, +0.8f,
	};
	GLushort indices[] = { 0, 1, 2, 3 };
	GLsizeiptr currentPos = 0;
	
	// Generating Buffers(VBO) and Vertex Array Objects(VAO)
	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glGenVertexArrays(1, &triangleVAO);
	glGenVertexArrays(1, &shapeVAO);
	glGenVertexArrays(1, &cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID); // Binding the Vertex Buffer (VBO)
	glBufferData(GL_ARRAY_BUFFER, cube.fullBufferSize(), 0, GL_STATIC_DRAW);

	std::cout << "Cube Starting Position = " << currentPos << std::endl;
	glBindVertexArray(cubeVAO); {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferSubData(GL_ARRAY_BUFFER, currentPos,									cube.vertexBufferSize(), cube.vertices); // Array Buffer does not care about VAO
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentPos + cube.vertexBufferSize(),	cube.indexBufferSize(), cube.indices);
		glEnableVertexAttribArray(0); // Vertex
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), 0);
		glEnableVertexAttribArray(1); // Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(2); // Normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(sizeof(glm::vec3) * 2));
		currentPos += cube.fullBufferSize();
	} glBindVertexArray(0);

	/*
	glBindVertexArray(triangleVAO); { // All Vertex Attributes and Element Buffers will be bound to this VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferSubData(GL_ARRAY_BUFFER,			currentPos,									triangle.vertexBufferSize(),	triangle.vertices);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,	currentPos + triangle.vertexBufferSize(),	triangle.indexBufferSize(),		triangle.indices);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(cube.fullBufferSize()));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(cube.fullBufferSize() + sizeof(glm::vec3)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(cube.fullBufferSize() + sizeof(glm::vec3)));
		currentPos += triangle.fullBufferSize();
	} glBindVertexArray(0);*/

}

/// <summary>
/// Painting the Window
/// </summary>
void GLWindow::paintGL() {
	offset1 = translatePos(offset1, vel1 * deltaTime, Vec4{ -0.8f, 0.8f, -0.8f, 0.8f });
	offset2 = translatePos(offset2, vel2 * deltaTime, Vec4{ -0.8f, 0.8f, -0.8f, 0.8f });

	//std::cout << "{" << offset1.x  << ", " << offset1.y << "}" << std::endl;
	glViewport(width() / 4, 0, width() / 2, height());
	glClearColor(+0.0f, +0.0f, 0.0f, +1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	GLsizeiptr startPos = 0;


	glm::mat4 viewToProjectionMat = glm::perspective(60.0f, ((float)width()/2) / height(), 0.1f, 20.0f);
	glm::mat4 worldToViewMat = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMat = viewToProjectionMat * worldToViewMat;

	glUseProgram(cubeProgram); {
		using glm::mat4;
		startPos = 0;
		std::cout << "Cube Drawing Start = " << startPos << std::endl;
		mat4 translationMat = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
		mat4 rotationMat = glm::rotate(mat4(), glm::radians(cubeRotation), glm::vec3(1.0f, 0.0f, 0.0f));
		mat4 scaleMat = glm::scale(mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
		mat4 modelToWorldMat = translationMat * rotationMat * scaleMat;

		mat4 modelToProjection = worldToProjectionMat * modelToWorldMat;

		glBindVertexArray(cubeVAO); { // Cube
			glUniformMatrix4fv(transformId, 1, GL_FALSE, &modelToProjection[0][0]);
			glUniformMatrix4fv(modelToWorldId, 1, GL_FALSE, &modelToWorldMat[0][0]);
			glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 3.0f);
			glUniform3fv(lightPosId, 1, &lightPosition[0]);
			glDrawElements(GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_SHORT, (GLvoid*)(startPos + cube.vertexBufferSize()));

		} glBindVertexArray(0);

	} glUseProgram(0);

	/*
	glUseProgram(triangleProgram); {
		glBindVertexArray(shapeVAO); { // SHAPE
			// Box
			glUniform3f(colorId, 0.5f, 0.7f, 0.5f);
			glUniform2f(scaleId, 1.0f, 1.0f);
			glUniform2f(offsetId, 0.0f, 0.0f);

			//glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_SHORT, (GLvoid*)(triangle.vertexBufferSize() + triangle.indexBufferSize() + sizeof(Vertex) * 4));
		} glBindVertexArray(0);

		glBindVertexArray(triangleVAO); { // TRIANGLES
			startPos = cube.fullBufferSize();
			// Player 1
			glUniform3f(colorId, color1.x, color1.y, color1.z);
			glUniform2f(scaleId, scale1.x, scale1.y);
			glUniform2f(offsetId, offset1.x, offset1.y);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(startPos + triangle.vertexBufferSize()));

			// Player 2
			glUniform3f(colorId, color2.x, color2.y, color2.z);
			glUniform2f(scaleId, scale2.x, scale2.y);
			glUniform2f(offsetId, offset2.x, offset2.y);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(startPos + triangle.vertexBufferSize()));
		} glBindVertexArray(0);

	} glUseProgram(0);
	*/
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

	triangleProgram = glCreateProgram();
	glAttachShader(triangleProgram, vertexShaderID);
	glAttachShader(triangleProgram, fragmentShaderID);
	glLinkProgram(triangleProgram);

	colorId = glGetUniformLocation(triangleProgram, "color");
	scaleId = glGetUniformLocation(triangleProgram, "scale");
	offsetId = glGetUniformLocation(triangleProgram, "offset");

	/// <summary>
	/// Cube Shader
	/// </summary>
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	source = GetShaderSource("resources/shaders/BasicLighting.shader");
	adapter[0] = source.VertexSource.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = source.FragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	cubeProgram = glCreateProgram();
	glAttachShader(cubeProgram, vertexShaderID);
	glAttachShader(cubeProgram, fragmentShaderID);
	glLinkProgram(cubeProgram);

	transformId = glGetUniformLocation(cubeProgram, "transformMat");
	modelToWorldId = glGetUniformLocation(cubeProgram, "modelToWorldMat");
	lightPosId = glGetUniformLocation(cubeProgram, "lightPos");
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

void GLWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
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
		vel1 = (Vec2{ speed2, 0 }*factor);
		break;
	case Qt::Key::Key_Up: // W
		vel2 = (Vec2{ 0, speed2 }*factor);
		break;
	case Qt::Key::Key_Left: // A
		vel2 = (Vec2{ -speed2, 0.0f }*factor);
		cubeRotation += 30;
		break;
	case Qt::Key::Key_Down: // S
		vel2 = (Vec2{ 0, -speed2 }*factor);
		break;
	case Qt::Key::Key_Right: // D
		vel2 = (Vec2{ speed2, 0 }*factor);
		cubeRotation -= 30;
		break;
	}
	//std::cout << "{" << vel1.x << ", " << vel1.y << "}" << std::endl;
	repaint();
}