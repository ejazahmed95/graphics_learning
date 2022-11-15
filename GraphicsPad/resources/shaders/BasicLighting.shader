#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;

uniform mat4 transformMat;
uniform mat4 modelToWorldMat;
uniform mat4 modelToViewMat;

out vec3 vecOutColor;
out vec3 vecOutNormal;
out vec3 vecOutPosition;

void main()
{
	vec4 pos = vec4(position, 1.0);
	gl_Position = transformMat * pos;

	vecOutPosition = vec3(modelToWorldMat * vec4(position, 0));
	vecOutColor = vec3(1.0, 1.0, 1.0);
	//vecOutNormal = mat3(transpose(inverse(modelToViewMat))) * normal;
	vecOutNormal = vec3(modelToWorldMat * vec4(normal, 0));
};

#shader FRAGMENT
#version 430

in vec3 vecOutColor;
in vec3 vecOutNormal;
in vec3 vecOutPosition;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 drawColor;

void main()
{
	// Ambient
	float ambientLight = 0.2f;

	// Diffuse
	float diffuseLight = 0.0f;
	vec3 lightDir = normalize(lightPos - vecOutPosition);
	diffuseLight = max(dot(lightDir, normalize(vecOutNormal)),0) * 0.8f;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - vecOutPosition);
	vec3 reflectDir = reflect(-lightDir, normalize(vecOutNormal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 300);
	vec3 specular = spec * vec3(0.0, 1.0, 0.0) * specularStrength;

	float brightness = (ambientLight + diffuseLight + spec);
	//drawColor = vec4(viewPos, 1.0f);
	drawColor = brightness * vec4(vecOutColor, 1.0f);
};