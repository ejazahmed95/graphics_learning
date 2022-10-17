#include <gl/glew.h>
#include "GLWindow.h"
#include "ShaderLoader.h"
#include <core/ShapeGenerator.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	scale1 = { 0.1f, 0.1f };
	scale2 = { 0.15f, 0.15f };
	offset1 = { 0.4f, 0.1f };
	offset2 = { -0.4f, 0.1f };
	color1 = { 0.9f, 0.3f, 0.0f };
	color2 = { 0.0f, 0.3f, 0.9f };

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
	glBufferData(GL_ARRAY_BUFFER, triangle.fullBufferSize() + sizeof(vertices) + sizeof(indices) + cube.fullBufferSize(), 0, GL_STATIC_DRAW);

	glBindVertexArray(triangleVAO); { // All Vertex Attributes and Element Buffers will be bound to this VAO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, triangle.vertexBufferSize(), triangle.vertices);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, triangle.vertexBufferSize(), triangle.indexBufferSize(), triangle.indices);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(sizeof(glm::vec3)));
		currentPos += triangle.fullBufferSize();
	} glBindVertexArray(0);
	
	glBindVertexArray(shapeVAO); {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferSubData(GL_ARRAY_BUFFER, currentPos, sizeof(vertices), vertices); // Array Buffer does not care about VAO
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentPos + sizeof(vertices), sizeof(indices), indices);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(currentPos)); // Picks up from currently bound ARRAY_BUFFER
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(currentPos + sizeof(Vec2)));
		currentPos += sizeof(vertices) + sizeof(indices);
	} glBindVertexArray(0);

	glBindVertexArray(cubeVAO); {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
		glBufferSubData(GL_ARRAY_BUFFER, currentPos, cube.vertexBufferSize(), cube.indices); // Array Buffer does not care about VAO
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, currentPos + cube.vertexBufferSize(), cube.indexBufferSize(), cube.indices);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (char*)(sizeof(glm::vec3)));
		currentPos += cube.fullBufferSize();
	} glBindVertexArray(0);
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

	glUseProgram(triangleProgram); {
		glBindVertexArray(shapeVAO); { // SHAPE
			// Box
			glUniform3f(colorId, 0.5f, 0.7f, 0.5f);
			glUniform2f(scaleId, 1.0f, 1.0f);
			glUniform2f(offsetId, 0.0f, 0.0f);

			glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_SHORT, (GLvoid*)(triangle.vertexBufferSize() + triangle.indexBufferSize() + sizeof(Vertex) * 4));
		} glBindVertexArray(0);

		glBindVertexArray(triangleVAO); { // TRIANGLES
			// Player 1
			glUniform3f(colorId, color1.x, color1.y, color1.z);
			glUniform2f(scaleId, scale1.x, scale1.y);
			glUniform2f(offsetId, offset1.x, offset1.y);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(triangle.vertexBufferSize()));

			// Player 2
			glUniform3f(colorId, color2.x, color2.y, color2.z);
			glUniform2f(scaleId, scale2.x, scale2.y);
			glUniform2f(offsetId, offset2.x, offset2.y);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)(triangle.vertexBufferSize()));
		} glBindVertexArray(0);
		
	} glUseProgram(0);

	glUseProgram(cubeProgram); {
		using glm::mat4;
		GLsizeiptr startPos = triangle.fullBufferSize() + sizeof(Vertex) * 4 + sizeof(GLushort) * 4;
		mat4 translationMat = glm::translate(mat4(), glm::vec3(0.0f, 0.0f, -1.0f));
		mat4 rotationMat = glm::rotate(mat4(), 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mat4 projectionMat = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);
		mat4 transformMat = projectionMat * translationMat * rotationMat;

		glBindVertexArray(cubeVAO); { // Cube
			glUniformMatrix4fv(transformId, 1, GL_FALSE, &transformMat[0][0]);
			glDrawElements(GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_SHORT, (GLvoid*)(startPos + cube.vertexBufferSize()));
		} glBindVertexArray(0);

	} glUseProgram(0);

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

	source = GetShaderSource("resources/shaders/MatTransformation.shader");
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
		vel1 = (Vec2{ speed2, 0 }*factor);
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