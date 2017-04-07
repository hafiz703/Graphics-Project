#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include <vecmath.h>

#include "Object.h"

using namespace std;

class ParticleSystem
{
public:

	ParticleSystem(int numParticles=0);

	int m_numParticles;
	unsigned m_length;
	vector<Vector4f> strucLinks;
	vector<Vector4f> shearLinks;
	vector<Vector4f> flexLinks;
	bool swingState;
	bool clothState;
	float dist = 1.0f;
	float theta = 0.0f;
	bool asc = true;
	bool windState;
	bool ballState;

	Object *o;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state) = 0;
	
	// getter method for the system's state
	vector<Vector3f> getState(){ return m_vVecState; };
	
	vector<int> getLifetime() { return m_vLifetime; };

	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { m_vVecState = newState; };

	void setLifetime(const vector<int> & newLifetime) { m_vLifetime = newLifetime; };

	// getter method for the system's OLD state
	vector<Vector3f> getOldState() { return old_vVecState; };

	// setter method for the system's OLD state
	void setOldState(const vector<Vector3f>  & newState) { old_vVecState = newState; };
	
	virtual void draw() = 0;

	virtual void addParticles(int number) = 0;

	virtual void delParticles() = 0;
	
protected:

	vector<Vector3f> m_vVecState;
	vector<int> m_vLifetime;

	vector<Vector3f> old_vVecState;
	
};

#endif
