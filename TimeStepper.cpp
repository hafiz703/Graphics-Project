#include "TimeStepper.hpp"

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

	vector<Vector3f> k0 = particleSystem->getState();

	particleSystem->setOldState(k0);

	unsigned total = k0.size() / numParticles;

	vector<Vector3f> k1 = particleSystem->evalF(k0);
	vector<Vector3f> k1State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k1State.push_back(k0[i + j] + ((stepSize / 2.0f)*k1[i + j]));
		}
	}

	vector<Vector3f> k2 = particleSystem->evalF(k1State);
	vector<Vector3f> k2State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k2State.push_back(k0[i + j] + ((stepSize / 2.0f)*k2[i + j]));
		}
	}

	vector<Vector3f> k3 = particleSystem->evalF(k2State);
	vector<Vector3f> k3State;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			k3State.push_back(k0[i + j] + (stepSize*k3[i + j]));
		}
	}

	vector<Vector3f> k4 = particleSystem->evalF(k3State);
	vector<Vector3f> a;
	for (int i = 0; i<k0.size(); i += total) {
		for (int j = 0; j<total; j++) {
			a.push_back(k0[i + j] +
				(stepSize*(k1[i + j] + 2 * k2[i + j] + 2 * k3[i + j] + k4[i + j]) / 6.0)
				);
		}
	}

	particleSystem->setState(a);
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
		if (a[i].y() < -2) {
			a[i] = Vector3f(a[i].x(), -2, a[i].z());
		}
	}

	o->setState(a);

	//end Range Kutta-------------------------------------------


}
