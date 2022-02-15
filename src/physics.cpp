#include "physics.h"

bool tmp = 1;

void Physics::doPhysicsTick() {
	resultant = gravity;
	std::vector<float> distance;
	std::vector<float> pointDistance;
	for (int i = 0; i < size(points); i++) {
		distance.push_back(getDistanceFromOBJ(i));
		pointDistance.push_back(getLineDistance(i));
	}
	for (int i = 0; i < size(pointDistance); i++) {
		if (distance.at(i) <= OBJ_RADIUS) {
			fixClipping(i);
			//A lot of MATH
			float resultantAngle = atan(resultant.y / resultant.x);
			float normalAngle = atan(pointNormals.at(i).y / pointNormals.at(i).x);
			float diffAngle = resultantAngle - normalAngle;
			float resultantLength = sqrt(pow(resultant.x, 2) + pow(resultant.y, 2));
			float normalLength = cos(diffAngle) * resultantLength;
			resultant += pointNormals.at(i) * normalLength;
			float velocityAngle = atan(velocity.y / velocity.x);
			diffAngle = velocityAngle - normalAngle;
			glm::vec2 finalVelocity = glm::rotate(velocity, float(PI - (2 * diffAngle)));
			velocity = finalVelocity;
		} else {
			float dist1;
			float dist2;
			dist1 = distance.at(i);
			if (i == size(pointDistance) - 1)
				dist2 = distance.at(0);
			else
				dist2 = distance.at(i + 1);
			float S = (dist1 + dist2 + pointDistance.at(i)) / 2;
			float area = sqrt(S * (S - dist1) * (S - dist2) * (S - pointDistance.at(i)));
			float height = area * 2 / pointDistance.at(i);
			float maxDist = sqrt(pow(pointDistance.at(i), 2) + pow(OBJ_RADIUS, 2));
			if (height <= OBJ_RADIUS && dist1 < maxDist && dist2 < maxDist) {
				fixClipping(i);
				//A lot of MATH
				float resultantAngle = atan(resultant.y / resultant.x);
				float normalAngle = atan(normals.at(i).y / normals.at(i).x);
				float diffAngle = resultantAngle - normalAngle;
				float resultantLength = sqrt(pow(resultant.x, 2) + pow(resultant.y, 2));
				float normalLength = cos(diffAngle) * resultantLength;
				resultant += normals.at(i) * normalLength;
				float velocityAngle = atan(velocity.y / velocity.x);
				diffAngle = velocityAngle - normalAngle;
				glm::vec2 finalVelocity;
				finalVelocity = glm::rotate(velocity, float(PI - (2 * diffAngle)));
				velocity = finalVelocity;
			}
		}
	}
	velocity += resultant / mass;
	velocity = velocity * 0.998f;
	Pos += velocity;
}

void Physics::generateMeshPoints() {
	for (int i = 0; i < size(mesh); i += 2) {
		glm::vec2 vertex;
		vertex.x = mesh.at(i);
		vertex.y = mesh.at(i + 1);
		points.push_back(vertex);
	}
}

void Physics::generateNormals() {
	for (int i = 0; i < size(points); i++) {
		float xDistance;
		float yDistance;
		if (i == size(points) - 1) {
			xDistance = points.at(i).x - points.at(0).x;
			yDistance = points.at(i).y - points.at(0).y;
		} else {
			xDistance = points.at(i).x - points.at(i + 1).x;
			yDistance = points.at(i).y - points.at(i + 1).y;
		}
		glm::vec2 normal(yDistance * -1, xDistance);
		normals.push_back(glm::normalize(normal));
	}
	for (int i = 0; i < size(points); i++) {
		if (i == size(points) - 1)
			pointNormals.push_back(glm::normalize(normals.at(i) + normals.at(0)));
		else
			pointNormals.push_back(glm::normalize(normals.at(i) + normals.at(i + 1)));
	}
}

float Physics::getLineDistance(int pointIndex) {
	float distance;
	if (pointIndex == size(points) - 1)
		distance = sqrt(pow((points.at(pointIndex).x - points.at(0).x), 2) + pow((points.at(pointIndex).y - points.at(0).y), 2));
	else
		distance = sqrt(pow((points.at(pointIndex).x - points.at(pointIndex + 1).x), 2) + pow((points.at(pointIndex).y - points.at(pointIndex + 1).y), 2));
	return distance;
}

float Physics::getDistanceFromOBJ(int pointIndex) {
	float distance = sqrt(pow((points.at(pointIndex).x - Pos.x), 2) + pow((points.at(pointIndex).y - Pos.y), 2));
	return distance;
}

void Physics::fixClipping(int pointIndex) {
	glm::vec2 reverse = normals.at(pointIndex) * 0.00001f;
	glm::vec2 actualPos;
	actualPos.x = Pos.x;
	actualPos.y = Pos.y;
	std::vector<float> pointDistance;
	for (int i = 0; i < size(points); i++) {
		pointDistance.push_back(getLineDistance(i));
	}
	for (int i = 0; i < 1000000 * OBJ_RADIUS; i++) {
		actualPos += reverse;
		float dist1 = sqrt(pow((points.at(pointIndex).x - actualPos.x), 2) + pow((points.at(pointIndex).y - actualPos.y), 2));
		float dist2;
		if (pointIndex == size(pointDistance) - 1)
			dist2 = sqrt(pow((points.at(0).x - actualPos.x), 2) + pow((points.at(0).y - actualPos.y), 2));
		else
			dist2 = sqrt(pow((points.at(pointIndex + 1).x - actualPos.x), 2) + pow((points.at(pointIndex + 1).y - actualPos.y), 2));
		float S = (dist1 + dist2 + pointDistance.at(pointIndex)) / 2;
		float area = sqrt(S * (S - dist1) * (S - dist2) * (S - pointDistance.at(pointIndex)));
		float height = area * 2 / pointDistance.at(pointIndex);
		if (height <= OBJ_RADIUS && height > (OBJ_RADIUS * 0.999)) {
			Pos = actualPos;
			break;
		}
	}
}

void Physics::generateMesh() {
	float angle = PI / 180 * (360 / 360);
	for (int i = 0; i < 360; i++) {
		std::vector<float> part;
		float currentAngle = angle * i;
		part.push_back(sin(currentAngle) * 0.7);
		part.push_back(cos(currentAngle) * 0.7);
		mesh.insert(mesh.end(), part.begin(), part.end());
	}
}
