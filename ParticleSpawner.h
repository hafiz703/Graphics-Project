#ifndef PARTICLESPAWNER_H
#define PARTICLESPAWNER_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"
#include "Object.h"
#include "Ball.h"
#include "Cube.h"
#include "Rect3D.h"
#include "ClothSystem2.h"

class ParticleSpawner : public ParticleSystem
{
public:
	ParticleSpawner(int numParticles);
	float particleRadius = 0.01f;
	int particleLifetime = 125;
	int particlesPerTick = 10;
	bool isCloth;
	vector<vector<Vector3f>> box_boundaries;
	vector<vector<int>> boxes;
	vector<vector<int>> particleBoxes;
	vector<float> impact_counter;
	int xCounter;
	int yCounter;
	int zCounter;
	float radiusOfConsideration;

	int sysType = 2;

	

	vector<Vector3f> evalF(vector<Vector3f> state);

	vector<Vector3f> evalFNew(vector<Vector3f> state, vector<vector<int>> boxes, vector<vector<int>> particleBoxes);

	vector<vector<Vector3f>> evalFCombined(vector<Vector3f> state, vector<Vector3f> o_state, int obj_num, vector<vector<int>> boxes, vector<vector<int>> particleBoxes);

	void draw();
	vector<Vector2f> particles;
	float ParticleSpawner::random(float upp, float low = 0);

	void addParticles();

	void delParticles();
	vector<Vector3f> ParticleSpawner::collisionDetector_ball(Object* ball, Vector3f particlePos, Vector3f particleVel);
	bool ParticleSpawner::collisionDetector_cube(Object* box, Vector3f particlePos, Vector3f particleVel);
	Vector3f ParticleSpawner::collisionDetector_cloth(Vector3f particlePos, Vector3f particleVel);

	void setBoxes(vector<vector<int>> newBoxes) { boxes = newBoxes; };

	void setParticleBoxes(vector<vector<int>> newInBox) { particleBoxes = newInBox; };

	vector<vector<int>> getBoxes() { return boxes; } ;

	vector<vector<int>> getParticleBoxes() { return particleBoxes; };

	int getXCounter() { return xCounter; };

	int getYCounter() { return yCounter; };

	int getZCounter() { return zCounter; };

	vector<vector<Vector3f>> getBoxBoundaries() { return box_boundaries; };

	bool getIsCloth() { return isCloth; };
	
	void setSystem(int i) {
		sysType = i;
	}
	int getSysType() {
		return sysType;
	}
};

#endif
