#include "TimeStepper.hpp"
#include <iostream>

///TODO: implement Explicit Euler time integrator here
vector<Vector3f> eulerStep(ParticleSystem* particleSystem, float stepSize) {
	vector<Vector3f> a;
	vector<Vector3f> currState = particleSystem->getState();
	vector<Vector3f> fx = particleSystem->evalF(currState);

	for (unsigned i = 0; i < currState.size(); ++i) {
		Vector3f x = currState[i] + fx[i] * stepSize;
		a.push_back(x);
	}

	return a;
}

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	
	vector<Vector3f> a = eulerStep(particleSystem, stepSize);
	particleSystem->setState(a);

}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> currState = particleSystem->getState();
	vector<Vector3f> f0 = particleSystem->evalF(currState);
	vector<Vector3f> f1 = particleSystem->evalF(eulerStep(particleSystem, stepSize));
	Vector3f temp;
	vector<Vector3f> a;
	for (int i = 0; i < currState.size(); ++i) {
		temp = currState[i] + ((stepSize / 2.0)*(f0[i] + f1[i]));
		a.push_back(temp);
	}

	particleSystem->setState(a);




}
void RKCustom::takeStep(ParticleSystem* particleSystem, float stepSize)
{


	int numParticles = particleSystem->m_numParticles;
	vector<vector<Vector3f>> box_boundaries = particleSystem->getBoxBoundaries();
	Object* o = particleSystem->getObject();

	vector<Vector3f> k0 = particleSystem->getState();
	vector<int> k0_lifetime = particleSystem->getLifetime();
	vector<vector<int>> k0_boxes = particleSystem->getBoxes();
	vector<vector<int>> k0_particleBoxes = particleSystem->getParticleBoxes();

	//cout << k0_particleBoxes[0].size() << endl;

	particleSystem->setOldState(k0);

	unsigned total = k0.size() / numParticles;

	for (int i = 0; i < k0.size(); i += total) {
		
		if (o->getObjectType() == "Ball") {
			Vector3f dxyz = o->getState()[0] - k0[i];
			float dist = sqrt(dxyz.absSquared());
			if (dist <= o->radius + 0.01f) {
				//cout << "Called" << endl;
				Vector3f normal = (k0[i] - o->getState()[0]).normalized();
				k0[i] = o->getState()[0] + (o->radius + 0.05f) * normal;
			}
		}
		else if (o->getObjectType() == "Cube") {

		}
	}

	vector<Vector3f> k1 = particleSystem->evalFNew(k0, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k1State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k1State.push_back(k0[i + j] + ((stepSize / 2.0f)*k1[i + j]));
		}
	}

	vector<Vector3f> k2 = particleSystem->evalFNew(k1State, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k2State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k2State.push_back(k0[i + j] + ((stepSize / 2.0f)*k2[i + j]));
		}
	}

	vector<Vector3f> k3 = particleSystem->evalFNew(k2State, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k3State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k3State.push_back(k0[i + j] + (stepSize*k3[i + j]));
		}
	}

	vector<Vector3f> k4 = particleSystem->evalFNew(k3State, k0_boxes, k0_particleBoxes);
	vector<Vector3f> a;
	vector<vector<int>> newBox(k0_boxes.size());
	vector<vector<int>> newParticleBox;
	for (int i = 0; i<k0.size(); i += total) {

		k0_lifetime[i / total]--;
		//cout << i / total << endl;
		
		if (k0_lifetime[i / total] < 0)
		{
			//a.push_back(Vector3f(0));
			//a.push_back(Vector3f(0));
		}
		else
		{
			vector<int> inBox;
			
			Vector3f newPos = k0[i] +
				(stepSize*(k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6.0);
			Vector3f newVel = k0[i + 1] +
				(stepSize*(k1[i + 1] + 2 * k2[i + 1] + 2 * k3[i + 1] + k4[i + 1]) / 6.0);
			
			float newPos_x = newPos.x();
			float newPos_y = newPos.y();
			float newPos_z = newPos.z();

			if (newPos.y() > 0.75f) {
				float temp = ((float)(rand() % 150))/100.0f;
				newPos_y = -0.75f + temp;
				//newPos_y = 0.75f;
			}
			else if (newPos.y() < -0.75f) {
				float temp = ((float)(rand() % 150)) / 100.0f;
				newPos_y = -0.75f + temp;
				//newPos_y = -0.75f;
			}
			if (newPos.z() > 0.25f) {
				float temp = ((float)(rand() % 100)) / 100.0f;
				newPos_z = -0.25f + temp;
				//newPos_z = 0.25f;
			}
			else if (newPos.z() < -0.25f) {
				float temp = ((float)(rand() % 100)) / 100.0f;
				newPos_z = -0.25f + temp;
				//newPos_z = -0.25f;
			}
			Vector3f newPos2 = Vector3f(newPos_x, newPos_y, newPos_z);

			a.push_back(newPos2);
			a.push_back(newVel);

			int xCounter = particleSystem->getXCounter();
			int yCounter = particleSystem->getYCounter();
			int zCounter = particleSystem->getZCounter();
			int totalBoxes = xCounter * yCounter * zCounter - 1;
			vector<int> boxesToLoop;

			//cout << k0_particleBoxes[i / total].size() << endl;

			//for (auto j : boxesToLoop) {
			for (int j = 0; j < box_boundaries.size(); j++) {
				//cout << j << endl;
				if (newPos.x() <= box_boundaries[j][0].x() && newPos.x() >= box_boundaries[j][1].x() &&
					newPos.y() <= box_boundaries[j][0].y() && newPos.y() >= box_boundaries[j][1].y() &&
					newPos.z() <= box_boundaries[j][0].z() && newPos.z() >= box_boundaries[j][1].z()) {

					newBox[j].push_back(i / total);
					//cout << j << endl;
					//cout << "Called" << endl;
					inBox.push_back(j);
				}
			}

			newParticleBox.push_back(inBox);
		}
	}

	//cout << k0_lifetime[0] << endl;

	particleSystem->setState(a);
	particleSystem->setLifetime(k0_lifetime);
	particleSystem->setBoxes(newBox);
	particleSystem->setParticleBoxes(newParticleBox);
	if (k0_lifetime[0] < 0)
	{
		particleSystem->delParticles();
	}
	particleSystem->addParticles();
}

void RKCustom::objectStep(ParticleSystem* particleSystem, Object *o, float stepSize) {
	vector<Vector3f> airOldState = particleSystem->getOldState();
	vector<Vector3f> airCurrState = particleSystem->getState();

	//Range Kutta------------
	int numParticles = o->num_particles;

	vector<Vector3f> k0 = o->getState();

	unsigned total = k0.size() / numParticles;

	vector<Vector3f> k1 = o->evalF(k0);
	vector<Vector3f> k1State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k1State.push_back(k0[i + j] + ((stepSize / 2.0f)*k1[i + j]));
		}
	}

	vector<Vector3f> k2 = o->evalF(k1State);
	vector<Vector3f> k2State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k2State.push_back(k0[i + j] + ((stepSize / 2.0f)*k2[i + j]));
		}
	}

	vector<Vector3f> k3 = o->evalF(k2State);
	vector<Vector3f> k3State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k3State.push_back(k0[i + j] + (stepSize*k3[i + j]));
		}
	}

	vector<Vector3f> k4 = o->evalF(k3State);
	vector<Vector3f> a;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			a.push_back(k0[i + j] +
				(stepSize*(k1[i + j] + 2 * k2[i + j] + 2 * k3[i + j] + k4[i + j]) / 6.0)
				);
		}
	}

	for (int i = 0; i < k0.size(); i += 2) {
		if (a[i].y() < 0) {
			a[i] = Vector3f(a[i].x(), 0.0f, a[i].z());
		}
	}

	o->setState(a);

	//end Range Kutta-------------------------------------------


}

void RKCustom::combinedStep(ParticleSystem* particleSystem, Object* o, float stepSize)
{
	vector<Vector3f> airOldState = particleSystem->getOldState();
	vector<Vector3f> airCurrState = particleSystem->getState();

	int numParticles = particleSystem->m_numParticles;
	int o_numParticles = o->num_particles;

	vector<vector<Vector3f>> box_boundaries = particleSystem->getBoxBoundaries();
	PendulumSystem2* cloth = particleSystem->getCloth();
	//Object* o = particleSystem->getObject();

	vector<Vector3f> k0 = particleSystem->getState();
	vector<Vector3f> o_k0 = o->getState();

	vector<int> k0_lifetime = particleSystem->getLifetime();
	vector<vector<int>> k0_boxes = particleSystem->getBoxes();
	vector<vector<int>> k0_particleBoxes = particleSystem->getParticleBoxes();

	//cout << k0_particleBoxes[0].size() << endl;
	bool temp = false;

	particleSystem->setOldState(k0);

	unsigned total = k0.size() / numParticles;
	unsigned o_total = o_k0.size() / o_numParticles;

	for (int i = 0; i < k0.size(); i += total) {

		if (o->getObjectType() == "Ball") {
			Vector3f dxyz = o->getState()[0] - k0[i];
			float dist = sqrt(dxyz.absSquared());
			if (dist <= o->radius + 0.01f) {
				//cout << "Called" << endl;
				Vector3f normal = (k0[i] - o->getState()[0]).normalized();
				k0[i] = o->getState()[0] + (o->radius + 0.05f) * normal;
			}
		}
		else if (particleSystem->getIsCloth()) {
			if ((k0[i] - cloth->getState()[12]).abs() < 0.5f) {
				temp = true;
				for (unsigned j = 0; j < cloth->getState().size(); j += 2) {
					//int i = 4;
					Vector3f ballPos = cloth->getState()[j];

					Vector3f ballVel = cloth->getState()[j + 1];

					Vector3f dxyz = ballPos - k0[i];
					float dist = sqrt(Vector3f::dot(dxyz, dxyz));
					Vector3f ray_dir = k0[i + 1].normalized();
					Vector3f ray_ori = k0[i] - ballPos;

					float a = Vector3f::dot(ray_dir, ray_dir);
					float b = 2.0f * Vector3f::dot(ray_dir, ray_ori);
					float c = Vector3f::dot(ray_ori, ray_ori) - (0.2f * 0.2f);

					bool t1 = false;
					bool t2 = false;

					if ((b*b - 4 * a*c) >= 0) {
						if ((-1.0f * b - sqrt(b*b - 4 * a*c)) / (2.0f * a) >= 0) {
							t1 = true;
						}
						if ((-1.0f * b + sqrt(b*b - 4 * a*c)) / (2.0f * a) >= 0) {
							t2 = true;
						}
					}

					if (dist <= (0.18f) + (0.02f) || t1 || t2) {
						//cout << particlePos[0] << " " << particlePos[1] << " " << particlePos[2] << endl;
						//cout << "collide" << endl;  
						if (dxyz.x() < 0) {
							Vector3f normal = -1.0f*(k0[i] - ballPos).normalized();
							float impact_angle = acos(Vector3f::dot(normal, k0[i]) / (sqrt(normal.absSquared()) * sqrt(k0[i].absSquared())));
							k0[i] = ballPos + ((0.18f + 0.05f) * cos(impact_angle) * normal);
						}
						else {
							Vector3f normal = (k0[i] - ballPos).normalized();
							float impact_angle = acos(Vector3f::dot(normal, k0[i]) / (sqrt(normal.absSquared()) * sqrt(k0[i].absSquared())));
							k0[i] = ballPos + ((0.18f + 0.05f) * cos(impact_angle) * normal);
						}
					}
				}
			}
		}
		else if (o->getObjectType() == "Cube") {

		}
	}

	vector<vector<Vector3f>> k1_temp = particleSystem->evalFCombined(k0, o_k0, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k1 = k1_temp[0];
	vector<Vector3f> o_k1 = k1_temp[1];
	vector<Vector3f> k1State;
	vector<Vector3f> o_k1State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k1State.push_back(k0[i + j] + ((stepSize / 2.0f)*k1[i + j]));
		}
	}
	for (int i = 0; i<o_k0.size(); i += o_total) {
		for (int j = 0; j<o_total; j++) {
			o_k1State.push_back(o_k0[i + j] + ((stepSize / 2.0f)*o_k1[i + j]));
		}
	}
	//cout << "this1" << endl;
	//o_k1State[1].print();

	vector<vector<Vector3f>> k2_temp = particleSystem->evalFCombined(k1State, o_k1State, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k2 = k2_temp[0];
	vector<Vector3f> o_k2 = k2_temp[1];
	vector<Vector3f> k2State;
	vector<Vector3f> o_k2State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k2State.push_back(k0[i + j] + ((stepSize / 2.0f)*k2[i + j]));
		}
	}
	for (int i = 0; i<o_k0.size(); i += o_total) {
		for (int j = 0; j<o_total; j++) {
			o_k2State.push_back(o_k0[i + j] + ((stepSize / 2.0f)*o_k2[i + j]));
		}
	}
	//cout << "this2" << endl;
	//o_k2State[1].print();

	vector<vector<Vector3f>> k3_temp = particleSystem->evalFCombined(k2State, o_k2State, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k3 = k3_temp[0];
	vector<Vector3f> o_k3 = k3_temp[1];
	vector<Vector3f> k3State;
	vector<Vector3f> o_k3State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k3State.push_back(k0[i + j] + (stepSize*k3[i + j]));
		}
	}
	for (int i = 0; i<o_k0.size(); i += o_total) {
		for (int j = 0; j<o_total; j++) {
			o_k3State.push_back(o_k0[i + j] + ((stepSize / 2.0f)*o_k3[i + j]));
		}
	}
	//cout << "this3" << endl;
	//o_k3State[1].print();

	vector<vector<Vector3f>> k4_temp = particleSystem->evalFCombined(k3State, o_k3State, k0_boxes, k0_particleBoxes);
	vector<Vector3f> k4 = k4_temp[0];
	vector<Vector3f> o_k4 = k4_temp[1];
	vector<Vector3f> a;
	vector<Vector3f> o_a;
	vector<vector<int>> newBox(k0_boxes.size());
	vector<vector<int>> newParticleBox;
	for (int i = 0; i<k0.size(); i += total) {

		k0_lifetime[i / total]--;
		//cout << i / total << endl;

		if (k0_lifetime[i / total] < 0)
		{
			//a.push_back(Vector3f(0));
			//a.push_back(Vector3f(0));
		}
		else
		{
			vector<int> inBox;

			Vector3f newPos = k0[i] +
				(stepSize*(k1[i] + 2 * k2[i] + 2 * k3[i] + k4[i]) / 6.0);
			Vector3f newVel = k0[i + 1] +
				(stepSize*(k1[i + 1] + 2 * k2[i + 1] + 2 * k3[i + 1] + k4[i + 1]) / 6.0);

			float newPos_x = newPos.x();
			float newPos_y = newPos.y();
			float newPos_z = newPos.z();

			if (newPos.y() > 0.75f) {
				float temp = ((float)(rand() % 150)) / 100.0f;
				newPos_y = -0.75f + temp;
				//newPos_y = 0.75f;
			}
			else if (newPos.y() < -0.75f) {
				float temp = ((float)(rand() % 150)) / 100.0f;
				newPos_y = -0.75f + temp;
				//newPos_y = -0.75f;
			}
			if (newPos.z() > 0.25f) {
				float temp = ((float)(rand() % 100)) / 100.0f;
				newPos_z = -0.25f + temp;
				//newPos_z = 0.25f;
			}
			else if (newPos.z() < -0.25f) {
				float temp = ((float)(rand() % 100)) / 100.0f;
				newPos_z = -0.25f + temp;
				//newPos_z = -0.25f;
			}
			Vector3f newPos2 = Vector3f(newPos_x, newPos_y, newPos_z);

			a.push_back(newPos2);
			a.push_back(newVel);

			int xCounter = particleSystem->getXCounter();
			int yCounter = particleSystem->getYCounter();
			int zCounter = particleSystem->getZCounter();
			int totalBoxes = xCounter * yCounter * zCounter - 1;
			vector<int> boxesToLoop;

			//cout << k0_particleBoxes[i / total].size() << endl;

			//for (auto j : boxesToLoop) {
			for (int j = 0; j < box_boundaries.size(); j++) {
				//cout << j << endl;
				if (newPos.x() <= box_boundaries[j][0].x() && newPos.x() >= box_boundaries[j][1].x() &&
					newPos.y() <= box_boundaries[j][0].y() && newPos.y() >= box_boundaries[j][1].y() &&
					newPos.z() <= box_boundaries[j][0].z() && newPos.z() >= box_boundaries[j][1].z()) {

					newBox[j].push_back(i / total);
					//cout << j << endl;
					//cout << "Called" << endl;
					inBox.push_back(j);
				}
			}

			newParticleBox.push_back(inBox);
		}
	}

	for (int i = 0; i<o_k0.size(); i += o_total) {
		for (int j = 0; j<o_total; j++) {
			o_a.push_back(o_k0[i + j] +
				(stepSize*(o_k1[i + j] + 2 * o_k2[i + j] + 2 * o_k3[i + j] + o_k4[i + j]) / 6.0)
			);
		}
	}

	for (int i = 0; i < o_k0.size(); i += 2) {
		if (o_a[i].y() < 0) {
			o_a[i] = Vector3f(o_a[i].x(), 0.0f, o_a[i].z());
		}
	}

	//cout << "this" << endl;
	//o_a[1].print();
	//cout << k0_lifetime[0] << endl;

	particleSystem->setState(a);
	o->setState(o_a);
	particleSystem->setLifetime(k0_lifetime);
	particleSystem->setBoxes(newBox);
	particleSystem->setParticleBoxes(newParticleBox);
	if (k0_lifetime[0] < 0)
	{
		particleSystem->delParticles();
	}
	particleSystem->addParticles();
}
