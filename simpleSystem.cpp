
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem(unsigned numParticles)
{
	m_numParticles = numParticles;
	m_vVecState.push_back(Vector3f(0.1f, 0.1f, 0.0f));
	//setState(m_vVecState);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
// YOUR CODE HERE
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	for (int i = 0; i < state.size(); ++i) {
		Vector3f temp = Vector3f(state[i][1] * -1.0, state[i][0], 0.0);
		f.push_back(temp);

	}

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	Vector3f pos = getState()[0];//YOUR PARTICLE POSITION	 
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glutSolidSphere(0.075f, 10.0f, 10.0f);
	glPopMatrix();
}
