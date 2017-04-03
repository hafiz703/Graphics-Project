
#include "pendulumSystem.h"
#include <iostream>
PendulumSystem::PendulumSystem(int numParticles) :ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	vector<Vector3f> initialState;
	int k;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		Vector2f temp;
		// for this system, we care about the position and the velocity
		
		Vector3f position = Vector3f(0.0f, 1.0f*i, 1.0f*i);
		Vector3f velocity = Vector3f(0.0f, 0.0f, 0.0f);
		initialState.push_back(position);
		initialState.push_back(velocity);
		
		(i + 1 > numParticles - 1)? k = -1:k=i+1;
		temp = Vector2f(i - 1, k);		
		particles.push_back(temp);
	}
	
	setState(initialState);
	
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{

	vector<Vector3f> f;

	// YOUR CODE HERE
	float m = 1.0f;
	float dragConst = 25.0f;
	//float springConst = 2000.0f;
	//float restLen = 0.8f;
	
	for (int i = 0; i < m_numParticles; i++) {			 
		 
		Vector3f v = state[(i * 2) + 1];
		Vector3f resForce = -m * Vector3f(0.0f, 9.81f, 0.0f) - dragConst*v;
		 
		 
		//for (int j = 0; j<2; j++) {	
			//int parent = particles[i][j];
			//if (parent>=0) {				
			 
			Vector3f windforce = Vector3f(-30, 0, 0); // Constant windForce
			resForce += windforce;
			//}			
		//}
		f.push_back(v);
		f.push_back(resForce / m);
		 
	}
		 

		return f;

	}



// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	 
		for (int i = 0; i < m_numParticles; i++) {
			Vector3f pos = m_vVecState[i * 2];
			glPushMatrix();
			glTranslatef(pos[0] + random(0, 1), pos[1], pos[2]);
			glutSolidSphere(0.075f, 10.0f, 10.0f);
			glPopMatrix();
		}
	 
}

int PendulumSystem:: random(int low,int upp) {
	return rand() % upp + low;
}