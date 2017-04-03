#include "Ball.h"
#include <iostream>

using namespace std;


Ball::Ball()
{
	this->mass = 2.0f;
	this->radius = 0.5f;

	ostate.push_back(Vector3f(0.0f, 2.0, 0.0));
	ostate.push_back(Vector3f(0.0f, 0.0, 0.0));

}

void Ball::objectDraw() {
	Vector3f pos = getState()[0];
	pos.print();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	//glTranslatef(0, 0, 0);
	glutSolidSphere(radius, 10.0f, 10.0f);
	//glutSolidSphere(2.0f, 10.0f, 10.0f);
	glPopMatrix();


}

vector<Vector3f> Ball::evalF() {
	vector<Vector3f> f;

	// YOUR CODE HERE
	for (int i = 0; i < this->ostate.size(); i = i + 2) {
		f.push_back(this->ostate[i + 1]);

		Vector3f gravF = Vector3f(0.0f, this->mass * -1.0f * this->gravity, 0.0f);
		Vector3f dragF = cd*this->ostate[i + 1];
		Vector3f netF = gravF + dragF;

		//rotationState[i / 2] = Vector3f::cross(netF, rotationState[i / 2]).normalized();
		//rotationAngles[i / 2] = acos(Vector3f::dot(netF, rotationState[i / 2]));

		f.push_back(netF);
	}

	return f;
}


