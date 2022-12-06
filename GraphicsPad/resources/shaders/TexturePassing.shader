#shader VERTEX
#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 texCoord;

uniform mat4 modelToProjectionMat; // Model To Projection Mat
uniform mat4 modelToWorldMat;

// Interpolated Values outputted by vertex shader
out vec3 fragPosition;
out vec3 fragColor;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
	vec4 pos = vec4(position, 1.0);
	gl_Position = modelToProjectionMat * pos;

	fragPosition = vec3(modelToWorldMat * vec4(position, 1.0));
	fragColor = vec3(1, 1, 1);
	fragNormal = vec3(modelToWorldMat * vec4(normal, 0.0));
	fragTexCoord = texCoord;
};

#shader FRAGMENT
#version 430

in vec3 fragPosition;
in vec3 fragColor;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D tex;

out vec4 drawColor;

void main()
{
	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambientColor = ambientStrength * lightColor;

	// Diffuse
	vec3 lightDir = normalize(lightPos - fragPosition);
	float diffuseStrength = 0.7f;
	float diffuseComponent = max(dot(lightDir, normalize(fragNormal)), 0);
	vec3 diffuseColor = diffuseStrength * diffuseComponent * lightColor;

	// Specular
	float specularStrength = 0.8f;
	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-lightDir, fragNormal);
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), 50);
	vec3 specularColor = specularStrength * specularComponent * vec3(1, 0, 0);

	vec3 brightness = ambientColor + diffuseColor + specularColor;

	drawColor = texture(tex, fragTexCoord) * vec4(brightness * fragColor, 1.0f);
	//drawColor = texture(tex, fragTexCoord);
};