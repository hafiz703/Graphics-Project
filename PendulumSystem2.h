#ifndef PENDULUMSYSTEM2_H
#define PENDULUMSYSTEM2_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem2.h"

using namespace std;


class PendulumSystem2 : public ParticleSystem2
{
public:
	PendulumSystem2(int numParticles);
	void toggle_wind();
	void toggle_faces();
	Vector3f getVertexNormal(vector<int> faceList, vector<Vector3f> f);
	vector<Vector3f> evalF(vector<Vector3f> state);

	void draw();
	float ball_dz = 0.0f;

protected:

	float mg;
	float drag;
	float stiffness;

	bool ifWind = false;
	int cloth_size;

	bool faces_on = false;

	vector<Vector3f> faces1;
	vector<Vector3f> faces2;

	//usage: adjacencyList[vertex] = [list of face indexes involving vertex]
	vector< vector<int> > adjacencyList;

	//first particle, second particle, rest length and stiffness
	vector<Vector4f> springs;

	vector<Vector3f> particles;
};



#endif
