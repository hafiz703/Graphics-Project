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

class ParticleSpawner : public ParticleSystem
{
public:
	ParticleSpawner(int numParticles);

	vector<Vector3f> evalF(vector<Vector3f> state);

	void draw();
	vector<Vector2f> particles;
	float ParticleSpawner::random(int upp, int low = 0);


};

#endif
