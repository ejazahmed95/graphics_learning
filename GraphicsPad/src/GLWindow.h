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

	unsigned int colorId, scaleId, offsetId;

	// Lighting Shader
	GLuint lightingProgram;
	unsigned int modelToProjectionId, modelToWorldId, modelToViewId;
	unsigned int lightPosId, lightColorId, viewPosId, modelColorId;
	
	// Texture With Lighting Shader
	GLuint textureProgram;
	unsigned int texture_m2pId, tex_m2wId, tex_lightPosId, tex_viewPosId, tex_lightColorId;

	// Normal Map with lighting
	GLuint normalMapProgram;
	unsigned int nmap_m2pId, nmap_m2wId, nmap_lightPosId, nmap_viewPosId, nmap_lightColorId;

	// Passthrough Shader
	GLuint passthroughProgram;
	unsigned int passthrough_m2pId;

	// Shapes and VAOs
	Shape plane, cube, sphere, arrow, teapot, simplePlane;
	GLuint planeVAO, cubeVAO, sphereVAO, arrowVAO, teapotVAO, simplePlaneVAO;

	glm::vec3 lightPosition;
	
	// Textures
	GLuint texId, nmapId;
	
};

std::string ToString(glm::vec3& v);