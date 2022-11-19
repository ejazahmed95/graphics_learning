#shader VERTEX
#version 430

in layout(location = 0) vec4 position;
in layout(location = 1) vec3 vertexColor;

uniform mat4 transformMat;

out vec3 fragColor;

void main()
{
	gl_Position = transformMat * position;
	fragColor = vec3(1,1,1);
};

#shader FRAGMENT
#version 430

in vec3 fragColor;
out vec4 drawColor;

void main()
{
	drawColor = vec4(fragColor, 1);
};