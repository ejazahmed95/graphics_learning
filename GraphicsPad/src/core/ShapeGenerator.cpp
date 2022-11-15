#include "ShapeGenerator.h"
#include <glm/glm.hpp>

#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

glm::vec3 randomColor()
{
	glm::vec3 ret;
	ret.x = rand() / (float)RAND_MAX;
	ret.y = rand() / (float)RAND_MAX;
	ret.z = rand() / (float)RAND_MAX;
	return ret;
}

Shape ShapeGenerator::Triangle() {
	Shape triangle;

	Vertex3D vertices[] = {
		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};
	triangle.numVertices = NUM_ARRAY_ELEMENTS(vertices);
	triangle.vertices = new Vertex3D[triangle.numVertices];
	memcpy(triangle.vertices, vertices, sizeof(vertices));

	GLushort indices[] = { 0, 1, 2 };
	triangle.numIndices = NUM_ARRAY_ELEMENTS(indices);
	triangle.indices = new GLushort[triangle.numIndices];
	memcpy(triangle.indices, indices, sizeof(indices));

	return triangle;
}

Shape ShapeGenerator::Cube() {
	Shape cube;
	using glm::vec3;
	Vertex3D vertices[] = {
		vec3(-1.0f, +1.0f, +1.0f),  // 0
		vec3(+1.0f, +0.0f, +0.0f),	// Color
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
		vec3(+1.0f, +1.0f, +1.0f),  // 1
		vec3(+0.0f, +1.0f, +0.0f),	// Color
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
		vec3(+1.0f, +1.0f, -1.0f),  // 2
		vec3(+0.0f, +0.0f, +1.0f),  // Color
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
		vec3(-1.0f, +1.0f, -1.0f),  // 3
		vec3(+1.0f, +1.0f, +1.0f),  // Color
		vec3(+0.0f, +1.0f, +0.0f),  // Normal

		vec3(-1.0f, +1.0f, -1.0f),  // 4
		vec3(+1.0f, +0.0f, +1.0f),  // Color
		vec3(+0.0f, +0.0f, -1.0f),  // Normal
		vec3(+1.0f, +1.0f, -1.0f),  // 5
		vec3(+0.0f, +0.5f, +0.2f),  // Color
		vec3(+0.0f, +0.0f, -1.0f),  // Normal
		vec3(+1.0f, -1.0f, -1.0f),  // 6
		vec3(+0.8f, +0.6f, +0.4f),  // Color
		vec3(+0.0f, +0.0f, -1.0f),  // Normal
		vec3(-1.0f, -1.0f, -1.0f),  // 7
		vec3(+0.3f, +1.0f, +0.5f),  // Color
		vec3(+0.0f, +0.0f, -1.0f),  // Normal

		vec3(+1.0f, +1.0f, -1.0f),  // 8
		vec3(+0.2f, +0.5f, +0.2f),  // Color
		vec3(+1.0f, +0.0f, +0.0f),  // Normal
		vec3(+1.0f, +1.0f, +1.0f),  // 9
		vec3(+0.9f, +0.3f, +0.7f),  // Color
		vec3(+1.0f, +0.0f, +0.0f),  // Normal
		vec3(+1.0f, -1.0f, +1.0f),  // 10
		vec3(+0.3f, +0.7f, +0.5f),  // Color
		vec3(+1.0f, +0.0f, +0.0f),  // Normal
		vec3(+1.0f, -1.0f, -1.0f),  // 11
		vec3(+0.5f, +0.7f, +0.5f),  // Color
		vec3(+1.0f, +0.0f, +0.0f),  // Normal

		vec3(-1.0f, +1.0f, +1.0f),  // 12
		vec3(+0.7f, +0.8f, +0.2f),  // Color
		vec3(-1.0f, +0.0f, +0.0f),  // Normal
		vec3(-1.0f, +1.0f, -1.0f),  // 13
		vec3(+0.5f, +0.7f, +0.3f),  // Color
		vec3(-1.0f, +0.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, -1.0f),  // 14
		vec3(+0.4f, +0.7f, +0.7f),  // Color
		vec3(-1.0f, +0.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, +1.0f),  // 15
		vec3(+0.2f, +0.5f, +1.0f),  // Color
		vec3(-1.0f, +0.0f, +0.0f),  // Normal

		vec3(+1.0f, +1.0f, +1.0f),  // 16
		vec3(+0.6f, +1.0f, +0.7f),  // Color
		vec3(+0.0f, +0.0f, +1.0f),  // Normal
		vec3(-1.0f, +1.0f, +1.0f),  // 17
		vec3(+0.6f, +0.4f, +0.8f),  // Color
		vec3(+0.0f, +0.0f, +1.0f),  // Normal
		vec3(-1.0f, -1.0f, +1.0f),  // 18
		vec3(+0.2f, +0.8f, +0.7f),  // Color
		vec3(+0.0f, +0.0f, +1.0f),  // Normal
		vec3(+1.0f, -1.0f, +1.0f),  // 19
		vec3(+0.2f, +0.7f, +1.0f),  // Color
		vec3(+0.0f, +0.0f, +1.0f),  // Normal

		vec3(+1.0f, -1.0f, -1.0f),  // 20
		vec3(+0.8f, +0.3f, +0.7f),  // Color
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, -1.0f),  // 21
		vec3(+0.8f, +0.9f, +0.5f),  // Color
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, +1.0f),  // 22
		vec3(+0.5f, +0.8f, +0.5f),  // Color
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
		vec3(+1.0f, -1.0f, +1.0f),  // 23
		vec3(+0.9f, +1.0f, +0.2f),  // Color
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
	};
	/* OpenGL Vertices
	Vertex3D vertices[] = {
		vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f),
		vec3(0.5f,  0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f),
		vec3(0.5f,  0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f),
		vec3(-0.5f,  0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f),
		vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f,  0.0f, -1.0f),

		vec3(-0.5f, -0.5f,  0.5f),  vec3(0.0f,  0.0f, 1.0f),
		vec3(0.5f, -0.5f,  0.5f),  vec3(0.0f,  0.0f, 1.0f),
		vec3(0.5f,  0.5f,  0.5f),  vec3(0.0f,  0.0f, 1.0f),
		vec3(0.5f,  0.5f,  0.5f),  vec3(0.0f,  0.0f, 1.0f),
		vec3(-0.5f,  0.5f,  0.5f),  vec3(0.0f,  0.0f, 1.0f),
		vec3(-0.5f, -0.5f,  0.5f),  vec3(0.0f,  0.0f, 1.0f),

		vec3(-0.5f,  0.5f,  0.5f), vec3(-1.0f,  0.0f,  0.0f),
		vec3(-0.5f,  0.5f, -0.5f), vec3(-1.0f,  0.0f,  0.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f,  0.0f,  0.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f,  0.0f,  0.0f),
		vec3(-0.5f, -0.5f,  0.5f), vec3(-1.0f,  0.0f,  0.0f),
		vec3(-0.5f,  0.5f,  0.5f), vec3(-1.0f,  0.0f,  0.0f),

		vec3(0.5f,  0.5f,  0.5f),  vec3(1.0f,  0.0f,  0.0f),
		vec3(0.5f,  0.5f, -0.5f),  vec3(1.0f,  0.0f,  0.0f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(1.0f,  0.0f,  0.0f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(1.0f,  0.0f,  0.0f),
		vec3(0.5f, -0.5f,  0.5f),  vec3(1.0f,  0.0f,  0.0f),
		vec3(0.5f,  0.5f,  0.5f),  vec3(1.0f,  0.0f,  0.0f),

		vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f, -1.0f,  0.0f),
		vec3(0.5f, -0.5f, -0.5f),  vec3(0.0f, -1.0f,  0.0f),
		vec3(0.5f, -0.5f,  0.5f),  vec3(0.0f, -1.0f,  0.0f),
		vec3(0.5f, -0.5f,  0.5f),  vec3(0.0f, -1.0f,  0.0f),
		vec3(-0.5f, -0.5f,  0.5f),  vec3(0.0f, -1.0f,  0.0f),
		vec3(-0.5f, -0.5f, -0.5f),  vec3(0.0f, -1.0f,  0.0f),

		vec3(-0.5f,  0.5f, -0.5f),  vec3(0.0f,  1.0f,  0.0f),
		vec3(0.5f,  0.5f, -0.5f),  vec3(0.0f,  1.0f,  0.0f),
		vec3(0.5f,  0.5f,  0.5f),  vec3(0.0f,  1.0f,  0.0f),
		vec3(0.5f,  0.5f,  0.5f),  vec3(0.0f,  1.0f,  0.0f),
		vec3(-0.5f,  0.5f,  0.5f),  vec3(0.0f,  1.0f,  0.0f),
		vec3(-0.5f,  0.5f, -0.5f),  vec3(0.0f,  1.0f,  0.0f)
	};
	*/

	cube.numVertices = NUM_ARRAY_ELEMENTS(vertices);
	cube.vertices = new Vertex3D[cube.numVertices];
	memcpy(cube.vertices, vertices, sizeof(vertices));

	unsigned short indices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};
	cube.numIndices = NUM_ARRAY_ELEMENTS(indices);
	cube.indices = new GLushort[cube.numIndices];
	memcpy(cube.indices, indices, sizeof(indices));

	return cube;
}


Shape ShapeGenerator::MakePlaneVertices(uint dimensions)
{
	Shape ret;
	ret.numVertices = dimensions * dimensions;
	int half = dimensions / 2;
	ret.vertices = new Vertex3D[ret.numVertices];
	for (int i = 0; i < dimensions; i++)
	{
		for (int j = 0; j < dimensions; j++)
		{
			Vertex3D& thisVert = ret.vertices[i * dimensions + j];
			thisVert.position.x = j - half;
			thisVert.position.z = i - half;
			thisVert.position.y = 0;
			thisVert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			thisVert.color = randomColor();
		}
	}
	return ret;
}

Shape ShapeGenerator::MakePlaneIndices(uint dimensions)
{
	Shape ret;
	ret.numIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (int row = 0; row < dimensions - 1; row++)
	{
		for (int col = 0; col < dimensions - 1; col++)
		{
			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;

			ret.indices[runner++] = dimensions * row + col;
			ret.indices[runner++] = dimensions * row + col + dimensions + 1;
			ret.indices[runner++] = dimensions * row + col + 1;
		}
	}
	assert(runner = ret.numIndices);
	return ret;
}

Shape ShapeGenerator::Plane(uint dimensions)
{
	Shape ret = MakePlaneVertices(dimensions);
	Shape ret2 = MakePlaneIndices(dimensions);
	ret.numIndices = ret2.numIndices;
	ret.indices = ret2.indices;
	return ret;
}
