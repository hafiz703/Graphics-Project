#include <iostream>
#include "pendulumSystem2.h"
#include <math.h>


using namespace std;

PendulumSystem2::PendulumSystem2(int numParticles) :ParticleSystem2(numParticles)
{

	mg = -1.3f;
	drag = -5.0f;
	stiffness = 20.0f;

	//if (is_Cloth){ cloth_size = numParticles; }
	// fill in code for initializing the state based on the number of particles

}

void PendulumSystem2::toggle_wind() {
	ifWind = !ifWind;
	cout << "toggling wind" << endl;
};

void PendulumSystem2::toggle_faces() {
	faces_on = !faces_on;
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem2::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	//std::cout << " evalf" << endl;
	// YOUR CODE HERE
	for (int i = 0; i < state.size(); i = i + 2) {
		f.push_back(state[i + 1]);

		Vector3f gravF = Vector3f(0.0f, mg, 0.0f);
		Vector3f dragF = drag*state[i + 1];
		Vector3f netF = gravF + dragF;

		f.push_back(netF);
	}

	//spring forces
	for (int i = 0; i < springs.size(); i++) {
		Vector3f point0 = state[2 * springs[i][0]];
		Vector3f point1 = state[2 * springs[i][1]];

		float rest_length = springs[i][2];

		Vector3f v = point0 - point1;
		float dist = v.abs();

		float spring_force = springs[i][3];
		float c = 1.0f;
		if (dist > 2.1f*rest_length) {
			c = 1.4f;
		}

		Vector3f springF0 = -1.0f * c * spring_force * (dist - rest_length) * (v / dist);
		Vector3f springF1 = -1.0f * c * springF0;

		f[(2 * springs[i][0]) + 1] += springF0;
		f[(2 * springs[i][1]) + 1] += springF1;

	}

	//wind - 1 direction
	if (ifWind) {
		for (int i = 0; i < m_numParticles; i++) {
			float strength = (float)rand() / RAND_MAX;
			float x = f[2 * i][0];
			float y = f[2 * i][1];
			float z = f[2 * i][2];
			//f[2 * i + 1] += strength * Vector3f(sin(x*y), cos(x*y), sin(x*y*z));
			f[2 * i + 1] += strength * Vector3f(4.0f, 0.0f, 1.0f);
		}
	}

	//f[1] = Vector3f(0.0f, 0.0f, 0.0f);
	//if (is_Cloth) { f[2*(cloth_size - 1) + 1] = Vector3f(0.0f, 0.0f, 0.0f); }

	return f;
}

Vector3f PendulumSystem2::getVertexNormal(vector<int> faceList, vector<Vector3f> f) {
	Vector3f vn = Vector3f(0.0f, 0.0f, 0.0f);
	for (unsigned i = 0; i < faceList.size(); i++) {
		Vector3f face = f[faceList[i]];

		Vector3f facev1 = getState()[2 * face[0]];
		Vector3f facev2 = getState()[2 * face[1]];
		Vector3f facev3 = getState()[2 * face[2]];

		Vector3f line1 = facev1 - facev3;
		Vector3f line2 = facev2 - facev3;

		Vector3f n = Vector3f::cross(line1, line2).normalized();

		vn += n;
	}
	return vn;
}

// render the system (ie draw the particles)
void PendulumSystem2::draw()
{
	//std::cout << "wind " << ifWind<< endl;
	//if (is_Cloth) { m_numParticles *= m_numParticles; }

	if ((is_Cloth) && (faces_on)) {
		//std::cout << "drawing faces" << endl;
		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < faces1.size(); i++) {
			//faces[i].print();


			Vector3f v1 = getState()[2 * (int)faces1[i][0]];
			Vector3f v2 = getState()[2 * (int)faces1[i][1]];
			Vector3f v3 = getState()[2 * (int)faces1[i][2]];

			vector<int> faceList = adjacencyList[(int)faces1[i][0]];
			Vector3f vn1 = getVertexNormal(faceList, faces1);

			faceList = adjacencyList[(int)faces1[i][1]];
			Vector3f vn2 = getVertexNormal(faceList, faces1);

			faceList = adjacencyList[(int)faces1[i][2]];
			Vector3f vn3 = getVertexNormal(faceList, faces1);

			glNormal3f(vn1[0], vn1[1], vn1[2]);
			glVertex3f(v1[0], v1[1], v1[2]);
			glNormal3f(vn2[0], vn2[1], vn2[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
			glNormal3f(vn3[0], vn3[1], vn3[2]);
			glVertex3f(v3[0], v3[1], v3[2]);


		}

		for (unsigned int i = 0; i < faces2.size(); i++) {
			//faces[i].print();


			Vector3f v1 = getState()[2 * (int)faces2[i][0]];
			Vector3f v2 = getState()[2 * (int)faces2[i][1]];
			Vector3f v3 = getState()[2 * (int)faces2[i][2]];

			vector<int> faceList = adjacencyList[(int)faces2[i][0]];
			Vector3f vn1 = getVertexNormal(faceList, faces2);

			faceList = adjacencyList[(int)faces2[i][1]];
			Vector3f vn2 = getVertexNormal(faceList, faces2);

			faceList = adjacencyList[(int)faces2[i][2]];
			Vector3f vn3 = getVertexNormal(faceList, faces2);

			glNormal3f(vn1[0], vn1[1], vn1[2]);
			glVertex3f(v1[0], v1[1], v1[2]);
			glNormal3f(vn2[0], vn2[1], vn2[2]);
			glVertex3f(v2[0], v2[1], v2[2]);
			glNormal3f(vn3[0], vn3[1], vn3[2]);
			glVertex3f(v3[0], v3[1], v3[2]);


		}
		glEnd();

	}

	else {

		for (int i = 0; i < m_numParticles; i++) {

			Vector3f pos = getState()[2 * i];//  position of particle i. YOUR CODE HERE
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2]);
			glutSolidSphere(0.04f, 10.0f, 10.0f);
			glPopMatrix();
		}
		glDisable(GL_LIGHTING);
		for (unsigned int a = 0; a < springs.size(); a++) {


			Vector3f p0 = getState()[2 * (int)springs[a][0]];
			Vector3f p1 = getState()[2 * (int)springs[a][1]];
			glLineWidth(3.0f);
			glColor3f(1, 1, 1);

			//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glBegin(GL_LINES);
			glVertex3f(p0.x(), p0.y(), p0.z());
			glVertex3f(p1.x(), p1.y(), p1.z());
			glEnd();

		}
		glEnable(GL_LIGHTING);
		//cout << "dones " << endl;
	}

}
