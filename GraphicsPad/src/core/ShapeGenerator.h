#pragma once
#include <core/Shape.h>

typedef unsigned int uint;

class ShapeGenerator {
public:
	static Shape Triangle();
	static Shape Cube();

	static Shape Arrow();
	static Shape Plane(uint dimensions = 10);
	static Shape Teapot(uint tesselation = 10, const glm::mat4& lidTransform = glm::mat4());
	static Shape GenerateNormals(const Shape& data);

private:
	static Shape MakePlaneVertices(uint dimensions);
	static Shape MakePlaneIndices(uint dimensions);

	// Teapot helpers
	static void GeneratePatches(float* v, float* n, float* tc, unsigned short* el, int grid);
	static void MoveLid(int grid, float* v, glm::mat4 lidTransform);
	static void BuildPatchReflect(int patchNum,
		float* B, float* dB,
		float* v, float* n,
		float* tc, unsigned short* el,
		int& index, int& elIndex, int& tcIndex, int grid,
		bool reflectX, bool reflectY);
	static void BuildPatch(glm::vec3 patch[][4],
		float* B, float* dB,
		float* v, float* n, float* tc,
		unsigned short* el,
		int& index, int& elIndex, int& tcIndex,
		int grid, glm::mat3 reflect,
		bool invertNormal);
	static void GetPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);
	static void ComputeBasisFunctions(float* B, float* dB, int grid);
	static glm::vec3 Evaluate(int gridU, int gridV, float* B, glm::vec3 patch[][4]);
	static glm::vec3 EvaluateNormal(int gridU, int gridV,
		float* B, float* dB, glm::vec3 patch[][4]);


};

