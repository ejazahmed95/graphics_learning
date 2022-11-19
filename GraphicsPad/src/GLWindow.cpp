#include <gl/glew.h>
#include "GLWindow.h"
#include "ShaderLoader.h"
#include <core/ShapeGenerator.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
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
	lightMoveSpeed = 0.1f;
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

	lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	plane = ShapeGenerator::Plane();
	cube = ShapeGenerator::Cube();
	sphere = ShapeGenerator::Sphere();
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
	glGenVertexArrays(1, &planeVAO);
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID); // Binding the Vertex Buffer (VBO)
	glBufferData(GL_ARRAY_BUFFER, plane.fullBufferSize() + cube.fullBufferSize() + sphere.fullBufferSize(), 0, GL_STATIC_DRAW);

	// Setting offsets on shapes for using the single buffer
	plane.bufferOffset = 0;
	cube.bufferOffset = plane.fullBufferSize();
	sphere.bufferOffset = cube.bufferOffset + cube.fullBufferSize();

	// Plane
	glBindVertexArray(planeVAO); {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		setShapeData(plane);
	} glBindVertexArray(0);

	// Cube
	glBindVertexArray(cubeVAO); { // All Vertex Attributes and Element Buffers will be bound to this VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		setShapeData(cube);
	} glBindVertexArray(0);

	// Sphere
	glBindVertexArray(sphereVAO); { // All Vertex Attributes and Element Buffers will be bound to this VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		setShapeData(sphere);
	} glBindVertexArray(0);

}

void GLWindow::setShapeData(const Shape& shape) {
	glBufferSubData(GL_ARRAY_BUFFER, shape.bufferOffset, shape.vertexBufferSize(), shape.vertices); // Array Buffer does not care about VAO
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape.bufferOffset + shape.vertexBufferSize(), shape.indexBufferSize(), shape.indices);
	glEnableVertexAttribArray(0); // Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(shape.bufferOffset));
	glEnableVertexAttribArray(1); // Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(shape.bufferOffset + sizeof(glm::vec3)));
	glEnableVertexAttribArray(2); // Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(shape.bufferOffset + sizeof(glm::vec3) * 2));
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

	glm::mat4 viewToProjectionMat = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	glm::mat4 worldToViewMat = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMat = viewToProjectionMat * worldToViewMat;

	glUseProgram(lightingProgram); {
		using glm::mat4;
		
		glUniform3fv(lightPosId, 1, &lightPosition[0]);
		glm::vec3 viewPosition = camera.getPosition();
		glUniform3fv(viewPosId, 1, &viewPosition[0]);
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(lightColorId, 1, &lightColor[0]);
		
		glBindVertexArray(planeVAO); { // Plane
			mat4 translationMat = glm::translate(mat4(), glm::vec3(0.0f, -1.0f, -3.0f));
			mat4 rotationMat = glm::rotate(cubeRotation, glm::vec3(1.0f, 0.0f, 0.0f));
			mat4 scaleMat = glm::scale(mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
			mat4 modelToWorldMat = translationMat * rotationMat * scaleMat;

			// Render the vertices in Camera's view
			mat4 modelToProjection = worldToProjectionMat * modelToWorldMat;
			glUniformMatrix4fv(modelToProjectionId, 1, GL_FALSE, &modelToProjection[0][0]);
			glUniformMatrix4fv(modelToWorldId, 1, GL_FALSE, &modelToWorldMat[0][0]);
			glDrawElements(GL_TRIANGLES, plane.numIndices, GL_UNSIGNED_SHORT, (GLvoid*)(plane.bufferOffset + plane.vertexBufferSize()));

		} glBindVertexArray(0);

		glBindVertexArray(cubeVAO); { // Cube
			mat4 translationMat = glm::translate(mat4(), glm::vec3(-1.5f, 0.2f, -3.0f));
			mat4 rotationMat = glm::rotate(cubeRotation, glm::vec3(1.0f, 0.0f, 0.0f));
			mat4 scaleMat = glm::scale(mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
			mat4 modelToWorldMat = translationMat * rotationMat * scaleMat;

			// Render the vertices in Camera's view
			mat4 modelToProjection = worldToProjectionMat * modelToWorldMat;
			glUniformMatrix4fv(modelToProjectionId, 1, GL_FALSE, &modelToProjection[0][0]);
			glUniformMatrix4fv(modelToWorldId, 1, GL_FALSE, &modelToWorldMat[0][0]);
			glDrawElements(GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_SHORT, (GLvoid*)(cube.bufferOffset + cube.vertexBufferSize()));

		} glBindVertexArray(0);

		glBindVertexArray(sphereVAO); { // Sphere
			mat4 translationMat = glm::translate(mat4(), glm::vec3(3.0f, -0.0f, -5.0f));
			mat4 rotationMat = glm::rotate(cubeRotation, glm::vec3(1.0f, 0.0f, 0.0f));
			mat4 scaleMat = glm::scale(mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
			mat4 modelToWorldMat = translationMat * rotationMat * scaleMat;

			// Render the vertices in Camera's view
			mat4 modelToProjection = worldToProjectionMat * modelToWorldMat;
			glUniformMatrix4fv(modelToProjectionId, 1, GL_FALSE, &modelToProjection[0][0]);
			glUniformMatrix4fv(modelToWorldId, 1, GL_FALSE, &modelToWorldMat[0][0]);
			glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_SHORT, (GLvoid*)(sphere.bufferOffset + sphere.vertexBufferSize()));

		} glBindVertexArray(0);

	} glUseProgram(0);
}


void GLWindow::installShaders() {
	// Set Shader Source
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	ShaderSource source = GetShaderSource("resources/shaders/BasicLighting.shader");
	const char* adapter[1];
	adapter[0] = source.VertexSource.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = source.FragmentSource.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	// Compile Shader
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// Attach shader to a program id; Link Program
	lightingProgram = glCreateProgram();
	glAttachShader(lightingProgram, vertexShaderID);
	glAttachShader(lightingProgram, fragmentShaderID);
	glLinkProgram(lightingProgram);

	modelToProjectionId = glGetUniformLocation(lightingProgram, "transformMat");
	modelToWorldId = glGetUniformLocation(lightingProgram, "modelToWorldMat");
	lightPosId = glGetUniformLocation(lightingProgram, "lightPos");
	lightColorId = glGetUniformLocation(lightingProgram, "lightColor");
	viewPosId = glGetUniformLocation(lightingProgram, "viewPos");
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
		camera.moveForward();
		break;
	case Qt::Key::Key_A: // A
		camera.strafeLeft();
		break;
	case 0x0053: // S
		camera.moveBackward();
		break;
	case Qt::Key::Key_D: // D
		camera.strafeRight();
		break;
	case Qt::Key::Key_Q:
		camera.moveDown();
		break;
	case Qt::Key::Key_E:
		camera.moveUp();
		break;
	case Qt::Key::Key_Up:
		lightPosition += lightMoveSpeed * camera.getUp();
		break;
	case Qt::Key::Key_Left:
		lightPosition += -lightMoveSpeed * camera.getStrafeDirection();
		break;
	case Qt::Key::Key_Down:
		lightPosition += -lightMoveSpeed * camera.getUp();
		break;
	case Qt::Key::Key_Right:
		lightPosition += lightMoveSpeed * camera.getStrafeDirection();
		break;
	case Qt::Key::Key_C: // Move Forward
		lightPosition += lightMoveSpeed * camera.getViewDirection();
		break;
	case Qt::Key::Key_X: // Move Backward
		lightPosition += -lightMoveSpeed * camera.getViewDirection();
		break;
	}
	std::cout << "Light Position:" << ToString(lightPosition) << std::endl;
	//std::cout << "{" << vel1.x << ", " << vel1.y << "}" << std::endl;
	repaint();
}

std::string ToString(glm::vec3& v) {
	return "[x=" + std::to_string(v.x) + " | y=" + std::to_string(v.y) + "| z=" + std::to_string(v.z) + "]";
}