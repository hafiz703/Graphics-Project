#ifndef RECT3D_H
#define RECT3D_H

#include <vecmath.h>
#include <vector>

#include "Object.h"

class Rect3D : public Object {

public:

	Rect3D();

	void objectDraw();

	string getObjectType() {
		return type;
	}

	vector<Vector3f> evalF(vector<Vector3f> state);

	

protected:

	Vector3f color = Vector3f(0.4f, 0.4f, 0.4f);
	string type = "Rect3D";
	
	
};

#endif
#pragma once
