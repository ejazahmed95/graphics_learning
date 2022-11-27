#include <Qt\qapplication.h>
#include "GLWindow.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	GLWindow window;
	window.show();

	return app.exec();
}