#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	//ClothSystem();
	ClothSystem(unsigned length);
	vector<Vector3f> evalF(vector<Vector3f> state);
	int ClothSystem::indexOf(int i, int j);
	vector<Vector3f> getVertexes(int steps, vector<Vector3f>controlPoints);
	float springConst;
	void draw();

private:

};


#endif
