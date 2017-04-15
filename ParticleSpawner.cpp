
#include "ParticleSpawner.h"
#include <iostream>
#include <cmath>
ParticleSpawner::ParticleSpawner(int numParticles) :ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	vector<Vector3f> initialState;
	vector<int> initialLifetime;
	vector<vector<int>> allInBox;

	// range of bounding boxes: X = (-3.4520, 4), Y = (-1, 1), Z = (-1, 1)
	// Each entry has 2 Vector3f, which contains the min and max values of x, y and z
	float d = 60 * this->particleRadius;
	float length = 160 * this->particleRadius;
	float increment = length - d;
	radiusOfConsideration = d / 2;

	xCounter = ceil(((4 + 3.4520) - increment) / increment) + 1;
	yCounter = ceil(((0.75 + 0.75) - increment) / increment) + 1;
	zCounter = ceil(((0.25 + 0.25) - increment) / increment) + 1;

	for (int i = 0; i < xCounter; i++) {
		for (int j = 0; j < yCounter; j++) {
			for (int k = 0; k < zCounter; k++) {
				vector<Vector3f> boundary_values;
	
				boundary_values.push_back(Vector3f(4.0f - i * increment, 0.75f - j * increment, 0.25f - k * increment));
				boundary_values.push_back(Vector3f((4.0f - i * increment) - length, (0.75f - j * increment) - length, (0.25f - k * increment) - length));

				box_boundaries.push_back(boundary_values);

				/*cout << "Box " << box_boundaries.size() - 1 << endl;
				cout << "Upper Limit:" << endl;
				Vector3f(4.0f - i * increment, 1.0f - j * increment, 1.0f - k * increment).print();
				cout << "Lower Limit:" << endl;
				Vector3f((4.0f - i * increment) - length, (1.0f - j * increment) - length, (1.0f - k * increment) - length).print();*/
			}
		}
	}

	// boxes stores the particles that are currently in this particular box
	// particleBoxes stores the boxes this particular particle is in
	vector<vector<int>> boxes(xCounter * yCounter * zCounter);
	//cout << boxes.size() << endl;

	int k;
	int lifeSpan = this->particleLifetime;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		Vector2f temp;
		vector<int> inBox;

		// for this system, we care about the position and the velocity		
		Vector3f position = Vector3f(4, random(-0.75f, 0.75f), random(-0.25f, 0.25f));
		Vector3f velocity = Vector3f(0.0f, 0.0f, 0.0f);
		initialState.push_back(position);
		initialState.push_back(velocity);
		initialLifetime.push_back(lifeSpan);

		for (int j = 0; j < yCounter * zCounter; j++) {

			/*cout << "upper limit:" << endl;
			box_boundaries[j][0].print();
			cout << "lower limit:" << endl;
			box_boundaries[j][1].print();
			cout << "pos:" << endl;
			position.print();*/

			if (position.x() <= box_boundaries[j][0].x() && position.x() >= box_boundaries[j][1].x() &&
				position.y() <= box_boundaries[j][0].y() && position.y() >= box_boundaries[j][1].y() &&
				position.z() <= box_boundaries[j][0].z() && position.z() >= box_boundaries[j][1].z()) {

				//cout << "Particle: " << i << " added to box " << j << endl;
			
				boxes[j].push_back(i);
				inBox.push_back(j);
			}
		}

		//cout << inBox.size() << endl;
		allInBox.push_back(inBox);

		//(i + 1 > numParticles - 1) ? k = -1 : k = i + 1;
		//temp = Vector2f(i - 1, k);
		//particles.push_back(temp);
	}

	setState(initialState);
	setLifetime(initialLifetime);
	setBoxes(boxes);
	setParticleBoxes(allInBox);

	//cout << particleBoxes.size() << endl;

	//o = new Ball();
	o = new Cube();
}

void ParticleSpawner::addParticles()
{
	int init_numParticles = m_numParticles;
	m_numParticles += particlesPerTick;

	// vector<Vector3f> initialState;
	int k;
	int lifeSpan = this->particleLifetime;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < particlesPerTick; i++) {
		Vector2f temp;
		vector<int> inBox;

		// for this system, we care about the position and the velocity		
		Vector3f position = Vector3f(4, random(-0.75f, 0.75f), random(-0.25f, 0.25f));
		Vector3f velocity = Vector3f(0.0f, 0.0f, 0.0f);
		m_vVecState.push_back(position);
		m_vVecState.push_back(velocity);

		m_vLifetime.push_back(lifeSpan);

		for (int j = 0; j < yCounter * zCounter; j++) {
			if (position.x() <= box_boundaries[j][0].x() && position.x() >= box_boundaries[j][1].x() &&
				position.y() <= box_boundaries[j][0].y() && position.y() >= box_boundaries[j][1].y() &&
				position.z() <= box_boundaries[j][0].z() && position.z() >= box_boundaries[j][1].z()) {

				boxes[j].push_back(init_numParticles + i);
				inBox.push_back(j);
			}
		}
		particleBoxes.push_back(inBox);

		//(i + 1 > number - 1) ? k = -1 : k = i + 1;
		//temp = Vector2f(i - 1, k);
		//particles.push_back(temp);
	}
}

void ParticleSpawner::delParticles()
{
	m_numParticles -= particlesPerTick;

	m_vLifetime.erase(m_vLifetime.begin(), m_vLifetime.begin() + particlesPerTick);
}

vector<Vector3f> ParticleSpawner::collisionDetector_ball(Object* ball, Vector3f particlePos, Vector3f particleVel)
{
	 
	Vector3f ballPos = ball->getState()[0];
	Vector3f ballVel = ball->getState()[1];	 

	Vector3f dxyz = ballPos - particlePos;
	vector<Vector3f> res;
	float dist = sqrt(Vector3f::dot(dxyz,dxyz));
	if (dist <= ball->radius + particleRadius)  {		
		Vector3f normal = (particlePos - ballPos).normalized();
		//// cout << "Collided! " << normal[0] << normal[1] << normal[2] << endl;
		const float cor = 3.0f;

		//// inverse mass quantities
		//float im1 = 1 / ball->mass;
		//float im2 = 1 / 0.01; // mass of particle

		//// impact speed
		//Vector3f v = (particleVel - ball->getState()[1]);
		//float vn = Vector3f::dot(v, normal);

		//// sphere intersecting but moving away from each other already
		////if (vn > 0.0f) return;

		//// collision impulse
		//float i = (-(1.0f + cor) * vn) / (im1 + im2);
		//Vector3f impulse = normal * i;

		//// change in momentum
		//
		//Vector3f newPartVel;
		//Vector3f newBallVel;

		//newPartVel = particleVel + impulse * im1*100;
		//newBallVel = ball->getState()[1] - (impulse * im2);
		//res.push_back(newPartVel);
		//res.push_back(newBallVel);
		////cout << newPartVel[0] <<" "<< newPartVel[1] <<" " << newPartVel[2] << endl;

		float impact_angle = acos(Vector3f::dot(normal, particleVel) / (sqrt(normal.absSquared()) * sqrt(particleVel.absSquared())));
		Vector3f impact_dVel = (sqrt(particleVel.absSquared()) * cos(impact_angle)) * normal * (1.0f + cor + ((ball->radius + particleRadius) - dist));
		res.push_back(impact_dVel);
		res.push_back(Vector3f(0));
		return res;

	}
	else {
		res.push_back(Vector3f(0, 0, 0));
		res.push_back(Vector3f(0, 0, 0));
		return res;
	}
}

bool ParticleSpawner::collisionDetector_cube(Object* box, Vector3f particlePos, Vector3f particleVel) {
	 
	float eps = 0.01f;
	Vector3f boxPos = box->getState()[0];
	Vector3f boxVel = box->getState()[1];
	float halflen = box->radius / 2.0f;
	float minX = boxPos[0] - halflen;
	float maxX = boxPos[0] + halflen;

	float minY = boxPos[1] - halflen;
	float maxY = boxPos[1] + halflen;

	float minZ = boxPos[2] - halflen;
	float maxZ = boxPos[2] + halflen;

	float x = fmax(minX, fmin(particlePos[0], maxX));
	float y = fmax(minY, fmin(particlePos[1], maxY));
	float z = fmax(minZ, fmin(particlePos[2], maxZ));
	
	 
	float distance =  (x - particlePos[0]) * (x - particlePos[0]) +
		(y - particlePos[1]) * (y - particlePos[1]) +
		(z - particlePos[2]) * (z - particlePos[2]);
	if (distance < eps) {
		cout << "Collided with cube!" << endl;
	}
	return distance < 0.01?1:0; //particle radius
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

		Vector3f windforce = Vector3f(-3000, 0, 0); //+ Vector3f(random(-10, 30), random(-10, 30), random(-10, 30)); // Constant windForce + Randomness
		resForce += windforce;
		collisionDetector_ball(o, state[i * 2], state[(i * 2) + 1]);
		//collisionDetector_cube(o, state[i * 2], state[(i * 2) + 1]);
		f.push_back(v);

		f.push_back(resForce / m);

	}
	


	return f;

}

vector<Vector3f> ParticleSpawner::evalFNew(vector<Vector3f> state, vector<vector<int>> boxes, vector<vector<int>> particleBoxes)
{

	vector<Vector3f> f;

	// YOUR CODE HERE
	float m = 1.0f;
	float dragConst = 25.0f;
	float reductionFactor = 5.0f;
	//float springConst = 2000.0f;
	//float restLen = 0.8f;

	//cout << state.size() << endl;

	for (int i = 0; i < m_numParticles; i++) {
		 
		Vector3f v = state[(i * 2) + 1];
		Vector3f resForce = -m * Vector3f(0.0f, 0.0f, 0.0f) - dragConst*v;

		Vector3f windforce = Vector3f(-30, 0, 0); //+ Vector3f(random(-10, 30), random(-10, 30), random(-10, 30)); // Constant windForce + Randomness
		resForce += windforce;

		// cout << "array size: " << particleBoxes.size() << endl;
		// cout << "i: " << i << endl;

		for (int j = 0; j < particleBoxes[i].size(); j++) {

			int currentBox = particleBoxes[i][j];

			// cout << "currentBox: " << currentBox << endl;
			// cout << "boxes size: " << boxes.size() << endl;

			for (int k = 0; k < boxes[currentBox].size(); k++) {
				if (boxes[currentBox][k] != i) {
					
					Vector3f difference = state[i * 2] - state[boxes[currentBox][k] * 2];
					float distance = sqrt(difference.absSquared());
					//cout << distance << endl;
					if (distance < radiusOfConsideration) {
						resForce += (difference.normalized() / distance) * reductionFactor;
						//resForce += Vector3f(0, 10, 0);
						//cout << "Called" << endl;
					}
				}
			}
		}

		/*Vector3f newV = collisionDetector_ball(o, state[i * 2], state[(i * 2) + 1])[0];
		v += newV;*/

		collisionDetector_cube(o, state[i * 2], state[(i * 2) + 1]);
		f.push_back(v);

		//resForce.print();
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

float ParticleSpawner::random(float low, float upp) {
	int low_int = low * 100;
	int upp_int = upp * 100;
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upp_int - low_int) * 100));

}