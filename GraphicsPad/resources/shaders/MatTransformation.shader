#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

out vec3 vecOutColor;

uniform mat4 transformMat;

void main()
{
	vec4 pos = vec4(position, 1.0);
	gl_Position = transformMat * pos;
	vecOutColor = vertexColor;
}
;

#shader FRAGMENT
#version 430

out vec4 drawColor;
in vec3 vecOutColor;

void main()
{
	drawColor = vec4(vecOutColor, 0.5f);
};