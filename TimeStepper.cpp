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

	vector<Vector3f> k0 = particleSystem->getState();
	vector<int> k0_lifetime = particleSystem->getLifetime();
	vector<vector<int>> k0_boxes = particleSystem->getBoxes();
	vector<vector<int>> k0_particleBoxes = particleSystem->getParticleBoxes();

	//cout << k0_particleBoxes[0].size() << endl;

	particleSystem->setOldState(k0);

	unsigned total = k0.size() / numParticles;

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

			if (newPos.y() > 1) {
				newPos_y = 1.0f;
			}
			else if (newPos.y() < -1) {
				newPos_y = -1.0f;
			}
			if (newPos.z() > 1) {
				newPos_z = 1.0f;
			}
			else if (newPos.z() < -1) {
				newPos_z = -1.0f;
			}

			a.push_back(Vector3f(newPos_x, newPos_y, newPos_z));
			a.push_back(newVel);

			int xCounter = particleSystem->getXCounter();
			int yCounter = particleSystem->getYCounter();
			int zCounter = particleSystem->getZCounter();
			int totalBoxes = xCounter * yCounter * zCounter - 1;
			vector<int> boxesToLoop;

			//cout << k0_particleBoxes[i / total].size() << endl;

			for (int j = 0; j < k0_particleBoxes[i / total].size(); j++){
				int currentBox = k0_particleBoxes[i / total][j];
				vector<int> neighbors;
				//cout << j << endl;

				int box111 = currentBox - (zCounter * yCounter) - zCounter - 1;
				int box112 = currentBox - (zCounter * yCounter) - zCounter;
				int box113 = currentBox - (zCounter * yCounter) - zCounter + 1;
				int box121 = currentBox - (zCounter * yCounter) - 1;
				int box122 = currentBox - (zCounter * yCounter);
				int box123 = currentBox - (zCounter * yCounter) + 1;
				int box131 = currentBox - (zCounter * yCounter) + zCounter - 1;
				int box132 = currentBox - (zCounter * yCounter) + zCounter;
				int box133 = currentBox - (zCounter * yCounter) + zCounter - 1;

				int box211 = currentBox - zCounter - 1;
				int box212 = currentBox - zCounter;
				int box213 = currentBox - zCounter + 1;
				int box221 = currentBox - 1;
				int box222 = currentBox;
				int box223 = currentBox + 1;
				int box231 = currentBox + zCounter - 1;
				int box232 = currentBox + zCounter;
				int box233 = currentBox + zCounter + 1;

				int box311 = currentBox + (zCounter * yCounter) - zCounter - 1;
				int box312 = currentBox + (zCounter * yCounter) - zCounter;
				int box313 = currentBox + (zCounter * yCounter) - zCounter + 1;
				int box321 = currentBox + (zCounter * yCounter) - 1;
				int box322 = currentBox + (zCounter * yCounter);
				int box323 = currentBox + (zCounter * yCounter) + 1;
				int box331 = currentBox + (zCounter * yCounter) + zCounter - 1;
				int box332 = currentBox + (zCounter * yCounter) + zCounter;
				int box333 = currentBox + (zCounter * yCounter) + zCounter - 1;

				// handle 8 corner cases
				if (currentBox == 0) {
					neighbors = { box222, box223, box232, box233, box322, box323, box332, box333 };
				}
				else if (currentBox == zCounter - 1) {
					neighbors = { box221, box222, box231, box232, box321, box322, box331, box332 };
				}
				else if (currentBox == (zCounter * yCounter) - yCounter) {
					neighbors = { box212, box213, box222, box223, box312, box313, box322, box323 };
				}
				else if (currentBox == (zCounter * yCounter) - 1) {
					neighbors = { box211, box212, box221, box222, box311, box312, box321, box322 };
				}
				else if (currentBox == totalBoxes - (zCounter * yCounter) + 1) {
					neighbors = { box122, box123, box132, box133, box222, box223, box232, box233 };
				}
				else if (currentBox == totalBoxes - (zCounter * yCounter) + zCounter) {
					neighbors = { box121, box122, box131, box132, box221, box222, box231, box232 };
				}
				else if (currentBox == totalBoxes - zCounter + 1) {
					neighbors = { box112, box113, box122, box123, box212, box213, box222, box223 };
				}
				else if (currentBox == totalBoxes) {
					neighbors = { box111, box112, box121, box122, box211, box212, box221, box222 };
				}

				// handle 12 edge cases
				else if (currentBox < zCounter) {
					neighbors = { box221, box222, box223, box231, box232, box233, box321, box322, box323, box331, box332, box333 };
				}
				else if (currentBox < zCounter * yCounter && currentBox % zCounter == 0) {
					neighbors = { box212, box213, box222, box223, box232, box233, box312, box313, box322, box323, box332, box333 };
				}
				else if (currentBox < zCounter * yCounter && currentBox % zCounter == zCounter - 1) {
					neighbors = { box211, box212, box221, box222, box231, box232, box311, box312, box321, box322, box331, box332 };
				}
				else if (currentBox < zCounter * yCounter && currentBox >= (zCounter * yCounter) - zCounter) {
					neighbors = { box211, box212, box213, box221, box222, box223, box311, box312, box313, box321, box322, box323 };
				}
				else if (currentBox % (zCounter * yCounter) == 0) {
					neighbors = { box122, box123, box132, box133, box222, box223, box232, box233, box322, box323, box332, box333 };
				}
				else if (currentBox % (zCounter * yCounter) == zCounter - 1) {
					neighbors = { box121, box122, box131, box132, box221, box222, box231, box232, box321, box322, box331, box332 };
				}
				else if ((currentBox + zCounter) % (zCounter * yCounter) == 0) {
					neighbors = { box112, box113, box122, box123, box212, box213, box222, box223, box312, box313, box322, box323 };
				}
				else if ((currentBox + 1) % (zCounter * yCounter) == 0) {
					neighbors = { box111, box112, box121, box122, box211, box212, box221, box222, box311, box312, box321, box322 };
				}
				else if (currentBox > total - (zCounter * yCounter) && currentBox <= totalBoxes - (zCounter * yCounter) + zCounter) {
					neighbors = { box121, box122, box123, box131, box132, box133, box221, box222, box223, box231, box232, box233 };
				}
				else if (currentBox > totalBoxes - (zCounter * yCounter) && currentBox % zCounter == 0) {
					neighbors = { box112, box113, box122, box123, box132, box133, box212, box213, box222, box223, box232, box233 };
				}
				else if (currentBox > totalBoxes - (zCounter * yCounter) && (currentBox + 1) % zCounter == 0) {
					neighbors = { box111, box112, box121, box122, box131, box132, box211, box212, box221, box222, box231, box232 };
				}
				else if (currentBox > totalBoxes - zCounter) {
					neighbors = { box111, box112, box113, box121, box122, box123, box211, box212, box213, box221, box222, box223 };
				}

				// handle 6 surface cases
				else if (currentBox < (zCounter * yCounter)) {
					neighbors = { box211, box212, box213, box221, box222, box223, box231, box232, box233, box311, box312, box313, box321, box322, box323, box331, box332, box333 };
				}
				else if (currentBox % (zCounter * yCounter) < zCounter) {
					neighbors = { box121, box122, box123, box131, box132, box133, box221, box222, box223, box231, box232, box233, box321, box322, box323, box331, box332, box333 };
				}
				else if (currentBox % zCounter == 0) {
					neighbors = { box112, box113, box122, box123, box132, box133, box212, box213, box222, box223, box232, box233, box312, box313, box322, box323, box332, box333 };
				}
				else if ((currentBox + 1) % zCounter == 0) {
					neighbors = { box111, box112, box121, box122, box131, box132, box211, box212, box221, box222, box231, box232, box311, box312, box321, box322, box331, box332 };
				}
				else if (currentBox % (zCounter * yCounter) >= (zCounter * yCounter) - zCounter) {
					neighbors = { box111, box112, box113, box121, box122, box123, box211, box212, box213, box221, box222, box223, box311, box312, box313, box321, box322, box323 };
				}
				else if (currentBox > totalBoxes - (zCounter * yCounter)) {
					neighbors = { box111, box112, box113, box121, box122, box123, box131, box132, box133, box211, box212, box213, box221, box222, box223, box231, box232, box233 };
				}

				// handle other cases that are completely surrounded
				else {
					neighbors = { box111, box112, box113, box121, box122, box123, box131, box132, box133, box211, box212, box213, box221, box222, box223, box231, box232, box233, box311, box312, box313, box321, box322, box323, box331, box332, box333 };
				}

				for (auto it : neighbors) {
					//cout << it << endl;
					if (std::find(boxesToLoop.begin(), boxesToLoop.end(), it) == boxesToLoop.end() && it <= totalBoxes && it >= 0 ) {
						boxesToLoop.push_back(it);
						//cout << "pushed" << endl;
					}
				}
			}
			

			for (auto j : boxesToLoop) {
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
