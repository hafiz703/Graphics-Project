#ifndef CLOTHSYSTEM2_H
#define CLOTHSYSTEM2_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem2.h"
#include "pendulumSystem2.h"



class ClothSystem2 : public PendulumSystem2
{
	///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem2(int numParticles);
	int getStateIndex(int row, int col);

	float struc_rest_length;
	float structural_force;

	float shear_rest_length;
	float shear_force;

	float flex_rest_length;
	float flex_force;


private:

};


#endif
