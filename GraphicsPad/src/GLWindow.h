#pragma once
#include<QtOpenGL/qglWidget>
#include<QtGui/qkeyevent>
#include<Types.h>
#include<glm/glm.hpp>

class GLWindow : public QGLWidget {
protected:
	void initializeGL();
	void paintGL();
	virtual void keyPressEvent(QKeyEvent*) override; // Function defined in QWidget
	virtual void keyReleaseEvent(QKeyEvent*) override; // Function defined in QWidget
private:
	void installShaders();
	void sendData();
	void initData();
	void handleInput(QKeyEvent* event, bool pressed);
	Vec2 translatePos(Vec2 initialPos, Vec2 speed, Vec4 bounds);

	float deltaTime;
	float speed1, speed2;
	Vec2 vel1, vel2;
	Vec2 offset1, offset2;
	Vec2 scale1, scale2;

	Vec3 color1, color2;

	// Uniforms
	unsigned int colorId, scaleId, offsetId;
};
