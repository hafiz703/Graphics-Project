#ifndef BALL_H
#define BALL_H

#include <vecmath.h>
#include <vector>

#include "Object.h"

class Ball : public Object {

public:

	Ball();

	void objectDraw();

	string getObjectType() {
		return type;
	}

	vector<Vector3f> evalF(vector<Vector3f> state);

protected:

	Vector3f color = Vector3f(0.4f, 0.4f, 0.4f);
	string type = "Ball";
};

#endif
#pragma once
