#pragma once
#include <gl/glew.h>
#include<QtOpenGL/qglWidget>
#include<QtGui/qkeyevent>
#include<core/Shape.h>
#include<Camera.h>
#include<glm/glm.hpp>

class GLWindow : public QGLWidget {
protected:
	void initializeGL();
	void paintGL();
	virtual void keyPressEvent(QKeyEvent*) override; // Function defined in QWidget
	virtual void keyReleaseEvent(QKeyEvent*) override; // Function defined in QWidget
	virtual void mouseMoveEvent(QMouseEvent* e) override;
private:
	void initData();
	void sendData();
	void installShaders();
	
	GLuint createAndLinkShader(const char* fileName);
	void setShapeData(const Shape& shape);
	inline void drawShape(Shape& shape, glm::mat4& modelToWorldMat, glm::mat4& worldToProjectionMat);
	void drawShape(Shape& shape, glm::mat4& modelToWorldMat, glm::mat4& worldToProjectionMat, glm::vec3& modelColor);
	void handleInput(QKeyEvent* event, bool pressed);
	Vec2 translatePos(Vec2 initialPos, Vec2 speed, Vec4 bounds);

	Camera camera;

	float deltaTime;
	float lightMoveSpeed, speed2;
	Vec2 vel1, vel2;
	Vec2 offset1, offset2;
	Vec2 scale1, scale2;

	Vec3 color1, color2;

	float cubeRotation;

	// Uniforms
	unsigned int colorId, scaleId, offsetId;
	unsigned int modelToProjectionId, modelToWorldId, modelToViewId;
	unsigned int lightPosId, lightColorId, viewPosId, modelColorId;
	unsigned int passthrough_m2pId;

	Shape plane, cube, sphere, arrow, teapot;
	GLuint planeVAO, cubeVAO, sphereVAO, arrowVAO, teapotVAO;

	glm::vec3 lightPosition;
	
	GLuint lightingProgram, passthroughProgram;
};

std::string ToString(glm::vec3& v);