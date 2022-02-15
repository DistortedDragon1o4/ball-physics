#ifndef PHYSICS_CLASS_H
#define PHYSICS_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#define SEGMENTS 90
#define OBJ_RADIUS 0.03

#define PI atan(1) * 4

#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Physics {
public:
	float mass;
	glm::vec2 gravity;
	glm::vec2 friction;
	glm::vec2 resultant;
	glm::vec2 velocity;
  glm::vec2 Pos;

	std::vector<float> mesh;
	std::vector<glm::vec2> points;
	std::vector<glm::vec2> normals;
	std::vector<glm::vec2> pointNormals;

	void generateMesh();
	void generateMeshPoints();
	void doPhysicsTick();
	void generateNormals();
	void fixClipping(int pointIndex);
	float getDistanceFromOBJ(int pointIndex);
	float getLineDistance(int pointIndex);
};

#endif
