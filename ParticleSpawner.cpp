
#include "ParticleSpawner.h"
#include <iostream>
ParticleSpawner::ParticleSpawner(int numParticles) :ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	vector<Vector3f> initialState;
	vector<int> initialLifetime;

	// range of bounding boxes: X = (-3.4520, 4), Y = (-1, 1), Z = (-1, 1)
	// Each entry has 2 Vector3f, which contains the min and max values of x, y and z
	float d = 2 * this->particleRadius;
	float length = 100 * this->particleRadius;
	float increment = length - d;

	xCounter = ceil(((4 + 3.4520) - 8 * this->particleRadius) / (3 * this->particleRadius));
	yCounter = ceil(((1 + 1) - 8 * this->particleRadius) / (3 * this->particleRadius));
	zCounter = ceil(((1 + 1) - 8 * this->particleRadius) / (3 * this->particleRadius));

	for (int i = 0; i < xCounter; i++) {
		for (int j = 0; j < yCounter; j++) {
			for (int k = 0; k < zCounter; k++) {
				vector<Vector3f> boundary_values;
	
				boundary_values.push_back(Vector3f(4.0f - i * increment, 1.0f - j * increment, 1.0f - k * increment));
				boundary_values.push_back(Vector3f((4.0f - i * increment) - length, (1.0f - j * increment) - length, (1.0f - k * increment) - length));

				box_boundaries.push_back(boundary_values);
			}
		}
	}

	vector<vector<int>> boxes(xCounter * yCounter * zCounter);
	//cout << boxes.size() << endl;

	int k;
	int lifeSpan = this->particleLifetime;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		Vector2f temp;
		// for this system, we care about the position and the velocity		
		Vector3f position = Vector3f(4, random(-1.0f, 1.0f), random(-1.0f, 1.0f));
		Vector3f velocity = Vector3f(0.0f, 0.0f, 0.0f);
		initialState.push_back(position);
		initialState.push_back(velocity);
		initialLifetime.push_back(lifeSpan);

		for (int j = 0; j < yCounter * zCounter; j++) {
			if (box_boundaries[j][0].x() > position.x() > box_boundaries[j][1].x() &&
				box_boundaries[j][0].y() > position.y() > box_boundaries[j][1].y() &&
				box_boundaries[j][0].z() > position.z() > box_boundaries[j][1].z()) {
				//cout << j << endl;

				boxes[j].push_back(i);
			}
		}

		//(i + 1 > numParticles - 1) ? k = -1 : k = i + 1;
		//temp = Vector2f(i - 1, k);
		//particles.push_back(temp);
	}

	setState(initialState);
	setLifetime(initialLifetime);
	setBoxes(boxes);

	o = new Ball();
}

void ParticleSpawner::addParticles(int number)
{
	m_numParticles += number;

	// vector<Vector3f> initialState;
	int k;
	int lifeSpan = this->particleLifetime;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < number; i++) {
		Vector2f temp;
		// for this system, we care about the position and the velocity		
		Vector3f position = Vector3f(4, random(-1.0f, 1.0f), random(-1.0f, 1.0f));
		Vector3f velocity = Vector3f(0.0f, 0.0f, 0.0f);
		m_vVecState.push_back(position);
		m_vVecState.push_back(velocity);
		m_vLifetime.push_back(lifeSpan);

		for (int j = 0; j < yCounter * zCounter; j++) {
			if (box_boundaries[j][0].x() > position.x() > box_boundaries[j][1].x() &&
				box_boundaries[j][0].y() > position.y() > box_boundaries[j][1].y() &&
				box_boundaries[j][0].z() > position.z() > box_boundaries[j][1].z()) {
				boxes[j].push_back(m_vLifetime.size() + i);
			}
		}

		//(i + 1 > number - 1) ? k = -1 : k = i + 1;
		//temp = Vector2f(i - 1, k);
		//particles.push_back(temp);
	}
}

void ParticleSpawner::delParticles()
{
	m_numParticles -= 10;

	m_vVecState.erase(m_vVecState.begin(), m_vVecState.begin() + 20);
	m_vLifetime.erase(m_vLifetime.begin(), m_vLifetime.begin() + 10);

	for (int i = boxes.size() / 2; i < boxes.size(); i++) {
		for (int j = 0; j < boxes[i].size(); j++) {
			int erased_counter = 0;
			if (boxes[i][j - erased_counter] < 10) {
				boxes[i].erase(boxes[i].begin() + (j - erased_counter));
				erased_counter ++;
			}
			else {
				boxes[i][j - erased_counter] -= 10;
			}
		}
	}
}

void ParticleSpawner::collisionDetector(Object* ball,Vector3f particlePos)
{
	 
	Vector3f ballPos = ball->getState()[0];
	Vector3f dxyz = ballPos - particlePos;

	float dist = sqrt(Vector3f::dot(dxyz,dxyz));
	if (dist <= ball->radius + 0.01f)  {
		cout << "Collided!" << endl;
	}
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ParticleSpawner::evalF(vector<Vector3f> state)
{

	vector<Vector3f> f;

	// YOUR CODE HERE
	float m = 1.0f;
	float dragConst = 25.0f;
	//float springConst = 2000.0f;
	//float restLen = 0.8f;

	for (int i = 0; i < m_numParticles; i++) {

		Vector3f v = state[(i * 2) + 1];
		Vector3f resForce = -m * Vector3f(0.0f, 0.0f, 0.0f) - dragConst*v;

		Vector3f windforce = Vector3f(-30, 0, 0); //+ Vector3f(random(-10, 30), random(-10, 30), random(-10, 30)); // Constant windForce + Randomness
		resForce += windforce;
		collisionDetector(o, state[i * 2]);
		f.push_back(v);

		f.push_back(resForce / m);

	}


	return f;

}



// render the system (ie draw the particles)
void ParticleSpawner::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = m_vVecState[i * 2];
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glutSolidSphere(this->particleRadius, 10.0f, 10.0f);
		glPopMatrix();
	}

	o->objectDraw();

}

float ParticleSpawner::random(int low, int upp) {
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upp - low)));

}