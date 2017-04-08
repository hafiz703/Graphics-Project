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

class ParticleSpawner : public ParticleSystem
{
public:
	ParticleSpawner(int numParticles);
	float particleRadius = 0.01f;
	int particleLifetime = 125;

	vector<vector<Vector3f>> box_boundaries;
	vector<vector<int>> boxes;
	int xCounter;
	int yCounter;
	int zCounter;


	vector<Vector3f> evalF(vector<Vector3f> state);

	void draw();
	vector<Vector2f> particles;
	float ParticleSpawner::random(int upp, int low = 0);

	void addParticles(int number);

	void delParticles();
	void ParticleSpawner::collisionDetector(Object* ball, Vector3f particlePos);

	void setBoxes(vector<vector<int>> newBoxes) { boxes = newBoxes; };

};

#endif
