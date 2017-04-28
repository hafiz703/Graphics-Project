#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "vecmath.h"
#include <vector>
#include "particleSystem.h"

#include "Object.h"

class TimeStepper
{
public:
	virtual void takeStep(ParticleSystem* particleSystem,float stepSize)=0;
	virtual void objectStep(ParticleSystem* particleSystem, Object *object, float stepSize) = 0;
	virtual void combinedStep(ParticleSystem* particleSystem, Object *object, int obj_num, float stepSize) = 0;
};

//IMPLEMENT YOUR TIMESTEPPERS

class ForwardEuler:public TimeStepper
{
  void takeStep(ParticleSystem* particleSystem, float stepSize);
  void objectStep(ParticleSystem* particleSystem, Object *object, float stepSize);
};

class Trapzoidal:public TimeStepper
{
  void takeStep(ParticleSystem* particleSystem, float stepSize);
  void objectStep(ParticleSystem* particleSystem, Object *object, float stepSize);
};

/////////////////////////

//Provided
class RK4:public TimeStepper
{
  void takeStep(ParticleSystem* particleSystem, float stepSize);
   
 
   
};

class RKCustom :public TimeStepper
{
	void takeStep(ParticleSystem* particleSystem, float stepSize);

	void objectStep(ParticleSystem* particleSystem, Object *object, float stepSize);
	void combinedStep(ParticleSystem * particleSystem, Object * o, int obj_num, float stepSize);
};

#endif
