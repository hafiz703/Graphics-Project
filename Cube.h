#ifndef CUBE_H
#define CUBE_H

#include <vecmath.h>
#include <vector>

#include "Object.h"

class Cube : public Object {

public:

	Cube();

	void objectDraw();
	string getObjectType() {
		return type;
	}
	vector<Vector3f> evalF(vector<Vector3f> state);

protected:

	Vector3f color = Vector3f(0.4f, 0.4f, 0.4f);
	string type = "Cube";
};

#endif