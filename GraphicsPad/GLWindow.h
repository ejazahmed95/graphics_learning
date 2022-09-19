#pragma once
#include<QtOpenGL/qglWidget>

class GLWindow : public QGLWidget {
protected:
	void initializeGL();
	void paintGL();
};

