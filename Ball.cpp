#include "Ball.h"
#include <iostream>

using namespace std;


Ball::Ball(float e, float radius, float mass)
{
	this->mass = mass;
	this->radius = radius;

	ostate.push_back(Vector3f(0.0f + e, 0.0f, 0.0f));
	ostate.push_back(Vector3f(0.0f, 0.0f, 0.0f));

	num_particles = 1;

}

void Ball::objectDraw() {
	Vector3f pos = getState()[0];
	//pos.print();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	//cout << pos[0] << " " << pos[1] << " " << pos[2] << endl;
	//glTranslatef(0, 0, 0);
	glutSolidSphere(radius, 20.0f, 20.0f);
	//glutSolidSphere(2.0f, 10.0f, 10.0f);
	glPopMatrix();


}

vector<Vector3f> Ball::evalF(vector<Vector3f> state) {
	vector<Vector3f> f;



	// YOUR CODE HERE
	for (int i = 0; i < state.size(); i = i + 2) {
		f.push_back(state[i + 1]);

		Vector3f gravF = Vector3f(0.0f, this->mass * -1.0f * this->gravity, 0.0f);
		Vector3f dragF = cd*state[i + 1];
		Vector3f netF = gravF + dragF;

		//rotationState[i / 2] = Vector3f::cross(netF, rotationState[i / 2]).normalized();
		//rotationAngles[i / 2] = acos(Vector3f::dot(netF, rotationState[i / 2]));

		f.push_back(netF);
	}

	return f;
}


