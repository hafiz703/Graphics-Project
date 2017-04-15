#include "Rect3D.h"
#include <iostream>

using namespace std;


Rect3D::Rect3D()
{
	this->mass = 2.0f;
	this->radius = 0.5f;

	ostate.push_back(startingPos);
	ostate.push_back(Vector3f(0.0f, 0.0f, 0.0f));

	num_particles = 1;

}

void Rect3D::objectDraw() {
	Vector3f pos = getState()[0];
	pos.print();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(yRotation, 0.0, 1.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	//glTranslatef(0, 0, 0);
	glBegin(GL_QUADS);        // Draw The Cube Using quads

	glVertex3f(1.0f, 1.0f, 0.75f);    // Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 0.75f);    // Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
	glVertex3f(1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)

	glVertex3f(1.0f, -1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, 0.75f);    // Bottom Left Of The Quad (Bottom)
	glVertex3f(1.0f, -1.0f, 0.75f);    // Bottom Right Of The Quad (Bottom)
   
	glVertex3f(1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
	glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Front)

	glVertex3f(1.0f, -1.0f, 0.75f);    // Top Right Of The Quad (Back)
	glVertex3f(-1.0f, -1.0f, 0.75f);    // Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f, 0.75f);    // Bottom Left Of The Quad (Back)
	glVertex3f(1.0f, 1.0f, 0.75f);    // Bottom Right Of The Quad (Back)

	glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f, 0.75f);    // Top Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, 0.75f);    // Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, 1.0f);    // Bottom Right Of The Quad (Left)

	glVertex3f(1.0f, 1.0f, 0.75f);    // Top Right Of The Quad (Right)
	glVertex3f(1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, 0.75f);    // Bottom Right Of The Quad (Right)
	glEnd(); 
	glPopMatrix();


}

vector<Vector3f> Rect3D::evalF(vector<Vector3f> state) {
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
