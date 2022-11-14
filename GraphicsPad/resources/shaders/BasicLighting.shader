#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 3) vec3 normal;

uniform mat4 transformMat;

out vec3 vecOutColor;
out vec3 vecOutNormal;
out vec3 vecOutPosition;

void main()
{
	vec4 pos = vec4(position, 1.0);
	gl_Position = transformMat * pos;
	vecOutColor = vertexColor;
	vecOutNormal = vecOutNormal;
	vecOutPosition = position;
}
;

#shader FRAGMENT
#version 430

in vec3 vecOutColor;
in vec3 vecOutNormal;
in vec3 vecOutPosition;

uniform vec3 lightPos;

out vec4 drawColor;

void main()
{
	float ambientLight = 0.2f;
	
	float diffuseLight = 0.0f;
	vec3 lightDir = normalize(lightPos - vecOutPosition);
	diffuseLight = dot(lightVector, vecOutNormal);


	float brightness = (ambientLight + diffuseLight);
	drawColor = brightness * vec4(vecOutColor, 0.5f);
};