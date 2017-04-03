#ifndef BALL_H
#define BALL_H

#include <vecmath.h>
#include <vector>

#include "Object.h"

class Ball : public Object {

public:

	Ball();

	void objectDraw();

	vector<Vector3f> evalF();

protected:

	Vector3f color = Vector3f(0.4f, 0.4f, 0.4f);

};

#endif
#pragma once
