#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;

uniform mat4 transformMat; // Model To Projection Mat
uniform mat4 modelToWorldMat;

// Interpolated Values outputted by vertex shader
out vec3 fragPosition;
out vec3 fragColor;
out vec3 vecOutNormal; 

void main()
{
	vec4 pos = vec4(position, 1.0);
	gl_Position = transformMat * pos;

	fragPosition = vec3(modelToWorldMat * vec4(position, 0));
	fragColor = vec3(1.0, 1.0, 1.0); // Reset Color to White
	//vecOutNormal = vec3(modelToWorldMat * vec4(normal, 0));
	vecOutNormal = mat3(transpose(inverse(modelToWorldMat)))* normal;
};

#shader FRAGMENT
#version 430

in vec3 fragPosition;
in vec3 fragColor;
in vec3 vecOutNormal;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 drawColor;

void main()
{
	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambientColor = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - fragPosition);
	float diffuseStrength = max(dot(lightDir, normalize(vecOutNormal)), 0);
	vec3 diffuseColor = diffuseStrength * lightColor;


	/*
	// Diffuse
	float diffuseLight = 0.0f;
	vec3 lightDir = normalize(lightPos - fragPosition);
	diffuseLight = max(dot(lightDir, normalize(vecOutNormal)),0) * 0.8f;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normalize(vecOutNormal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 300);
	vec3 specular = spec * vec3(0.0, 1.0, 0.0) * specularStrength;

	float brightness = (ambientLight + diffuseLight + spec);
	//drawColor = vec4(viewPos, 1.0f);
	*/
	vec3 brightness = ambientColor + diffuseColor;
	drawColor = vec4(brightness * fragColor, 1.0f);
};