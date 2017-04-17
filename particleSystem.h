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
	Object *o2;
	Object *o3;
	Object *o4;
	
	// for a given state, evaluate derivative f(X,t)
	virtual vector<Vector3f> evalF(vector<Vector3f> state) = 0;

	virtual vector<Vector3f> evalFNew(vector<Vector3f> state, vector<vector<int>> boxes, vector<vector<int>> particleBoxes) = 0;

	virtual vector<vector<Vector3f>> evalFCombined(vector<Vector3f> state, vector<Vector3f> o_state, vector<vector<int>> boxes, vector<vector<int>> particleBoxes) = 0;
	
	// getter method for the system's state
	Object* getObject() { return o; };

	vector<Vector3f> getState(){ return m_vVecState; };
	
	vector<int> getLifetime() { return m_vLifetime; };

	virtual vector<vector<int>> getBoxes() = 0;

	virtual vector<vector<int>> getParticleBoxes() = 0;

	virtual int getXCounter() = 0;

	virtual int getYCounter() = 0;

	virtual int getZCounter() = 0;

	virtual vector<vector<Vector3f>> getBoxBoundaries() = 0;

	// setter method for the system's state
	void setState(const vector<Vector3f>  & newState) { m_vVecState = newState; };

	void setLifetime(const vector<int> & newLifetime) { m_vLifetime = newLifetime; };

	virtual void setBoxes(const vector<vector<int>> newBox) = 0;

	virtual void setParticleBoxes(const vector<vector<int>> newInBox) = 0;

	// getter method for the system's OLD state
	vector<Vector3f> getOldState() { return old_vVecState; };

	// setter method for the system's OLD state
	void setOldState(const vector<Vector3f>  & newState) { old_vVecState = newState; };
	
	virtual void draw() = 0;

	virtual void addParticles() = 0;

	virtual void delParticles() = 0;
	
	void toggleDrop() { drop = !drop; }
protected:

	vector<Vector3f> m_vVecState;
	vector<int> m_vLifetime;

	vector<Vector3f> old_vVecState;
	
	bool drop = false;
};

#endif
