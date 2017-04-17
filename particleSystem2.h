#ifndef PARTICLESYSTEM2_H
#define PARTICLESYSTEM2_H

#include <vector>
#include <vecmath.h>


class ParticleSystem2
{
public:

	ParticleSystem2(int numParticles = 0);

	int m_numParticles;

	// for a given state, evaluate derivative f(X,t)
	virtual std::vector<Vector3f> evalF(std::vector<Vector3f> state) = 0;

	// getter method for the system's state
	std::vector<Vector3f> getState() { return m_vVecState; };

	// setter method for the system's state
	void setState(const std::vector<Vector3f>  & newState) { m_vVecState = newState; };

	virtual void draw() = 0;

	bool is_Cloth = false;
protected:

	std::vector<Vector3f> m_vVecState;



};

#endif