#include "ClothSystem.h"
#include "particleSystem.h"
#include  <iostream> 

//TODO: Initialize here
ClothSystem::ClothSystem(unsigned length):ParticleSystem(length)
{	
	springConst = 1000.0f;
	ballState = false;
	windState = false;
	swingState = false;
	clothState = false;
	m_length = length;
	m_numParticles =length*length;	
	float m = 15.0f;
	float dragConst = 25.0f;	 
	float restLen = 0.8f;	
	vector<Vector3f> initialState;	
	Vector3f vel = Vector3f(0.0f, 0.0f, 0.0f);
	
	for (unsigned i = 0; i < length; ++i) {
		for (unsigned j = 0; j < length; ++j) {
			Vector3f pos = Vector3f(dist*i, 0.0f, dist*j);
			initialState.push_back(pos);
			initialState.push_back(vel);	
			 			
			int p;			 
			int r;			 
			
			(i + 1 > length-1) ? p = -1 : p = i + 1;
			(j + 1 > length-1) ? r = -1 : r = j + 1;			 
			
			strucLinks.push_back(Vector4f(indexOf(i-1, j), indexOf(p, j), indexOf(i, j-1), indexOf(i, r)));
			shearLinks.push_back(Vector4f(indexOf(i-1, j-1), indexOf(i - 1, r), indexOf(p, j-1), indexOf(p, r)));
			(i + 2 > length-1) ? p = -1 : p = i + 2;
			(j + 2 > length-1) ? r = -1 : r = j + 2;
			flexLinks.push_back(Vector4f(indexOf(i - 2, j), indexOf(p, j), indexOf(i, j - 2), indexOf(i, r)));			
			 

		}
		
	}
	
	setState(initialState);
	

}
	
int ClothSystem::indexOf(int i, int j) {
	if (i < 0 || j < 0) return -1;
	return (i*m_length) + j;
}
// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	
	
	float restLen = 0.8f;
	float m = 15.0f;
	float dragConst = 10.0f;
	vector<Vector3f> f;
	 
	//f.push_back(Vector3f());
	//f.push_back(Vector3f());
	
	for (unsigned i = 0; i<m_numParticles; ++i) {
		if (i==0||i == m_length - 1) {
			if (swingState) {

				//cout << theta << endl;
				f.push_back(Vector3f(2 * cos(theta), 0.0f, 0.0f));
				f.push_back(Vector3f(2 * cos(theta), 0.0f, 0.0f));				
				 
				(asc)?theta += 0.008:theta -= 0.008;
				 
				if (theta > 0.05|| theta <=0) (!asc);
				
			}
			else {
				f.push_back(Vector3f());
				f.push_back(Vector3f());
			}
			
		}
		else {

			Vector3f x = state[i * 2];
			Vector3f v = state[(i * 2) + 1];

			Vector3f resForce = -m * Vector3f(0.0f,9.81f,0.0f) - dragConst*v;	
			if (windState) {
				float random = rand() % 600;
				resForce += Vector3f(random, 0.0f, 0.0f);
			}
			for (unsigned j = 0; j<4; ++j) {
				int parent1 = strucLinks[i][j];	
				int parent2 = shearLinks[i][j];
				int parent3 = flexLinks[i][j];
				if (parent1 >= 0) {
					//cout << "p1: " << parent1 << endl;
					
					Vector3f tempdist = x - state[parent1 * 2];
					Vector3f springForce = (-springConst*(tempdist.abs() - restLen)* tempdist) / tempdist.abs();
					resForce += springForce;
				}
			   
				
				if (parent2 >= 0) {
					//cout << "p2: " << parent2 << endl;
					Vector3f tempdist = x - state[parent2 * 2];
					float restLen2 = sqrt(dist*dist + dist*dist);
					Vector3f springForce = (-springConst*(tempdist.abs() - restLen2)* tempdist) / tempdist.abs();
					resForce += springForce;
				}
			
				
				
				if (parent3 >= 0) {
					//cout << "p3: "<<parent3 << endl;
					Vector3f tempdist = x - state[parent3 * 2];
					float restLen2 = dist*2;
					Vector3f springForce = (-springConst*(tempdist.abs() - restLen2)* tempdist) / tempdist.abs();
					resForce += springForce;
				}
				
			}

			 

			f.push_back(v);
			f.push_back(resForce / m);
		}
	}
	 
	return f;
}
vector<Vector3f> ClothSystem::getVertexes(int steps, vector<Vector3f>controlPoints) {
	vector<Vector3f> V_array;	 
	for (int step = 0; step<steps; step++) {
		float t = (step*1.0) / steps;
		float x = (1 - t)*(1 - t)*(1 - t)*controlPoints[0][0]
			+ (3 * t)*(1 - t)*(1 - t)*controlPoints[1][0]
			+ (3 * t*t)*(1 - t)*controlPoints[2][0]
			+ t*t*t*controlPoints[3][0];
		float y = (1 - t)*(1 - t)*(1 - t)*controlPoints[0][1]
			+ (3 * t)*(1 - t)*(1 - t)*controlPoints[1][1]
			+ (3 * t*t)*(1 - t)*controlPoints[2][1]
			+ t*t*t*controlPoints[3][1];
		float z = (1 - t)*(1 - t)*(1 - t)*controlPoints[0][2]
			+ (3 * t)*(1 - t)*(1 - t)*controlPoints[1][2]
			+ (3 * t*t)*(1 - t)*controlPoints[2][2]
			+ t*t*t*controlPoints[3][2];
		V_array.push_back(Vector3f(x, y, z));
	}




	return V_array;

}
///TODO: render the system (ie draw the particles)
void ClothSystem::draw(){
	  
		if(!clothState){
			for (int i = 0; i < m_numParticles; i++) {
				Vector3f p = getState()[i * 2];
				glPushMatrix();
				glTranslatef(p[0], p[1], p[2]);
				glutSolidSphere(dist / 10, 10.0f, 10.0f);
				glPopMatrix();
				for (int j = 0; j < 4; j++) {
					int parent = strucLinks[i][j];
					if (parent >= 0) {
						Vector3f a = getState()[i * 2];
						Vector3f b = getState()[parent * 2];

						glBegin(GL_LINE_STRIP);
						glVertex3f(a[0], a[1], a[2]);
						glVertex3f(b[0], b[1], b[2]);
						glEnd();
					}

				}
			}
			

		
	}
		else {
			  
			vector<vector<Vector3f>> CtrlPts;
			vector<vector<Vector3f>> Vertices;
			vector<vector<Vector3f>> meshSet;

			for (int i = 0; i<m_length; i++) {
				vector<Vector3f> temp;
				for (int j = 0; j<m_length; j++) {
					temp.push_back(getState()[indexOf(i, j) * 2]);
				}
				CtrlPts.push_back(temp);
			}

			for (int i = 0; i<CtrlPts.size(); i++) {
				Vertices.push_back(getVertexes(20, CtrlPts[i]));

			}

			for (int i = 0; i<Vertices[0].size(); i++) {
				vector<Vector3f> controlPoints;
				for (int j = 0; j<CtrlPts.size(); j++) {
					controlPoints.push_back(Vertices[j][i]);
				}
				meshSet.push_back(getVertexes(20, controlPoints));
			}
			 

			glEnable(GL_LIGHTING);
			glPushAttrib(GL_ALL_ATTRIB_BITS);			 
			glBegin(GL_TRIANGLES);
			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
			

			for (int i = 0; i<meshSet.size() - 1; ++i) {
				for (int j = 0; j<meshSet[0].size() - 1; ++j) {
					vector<Vector3f> meshVert;
					meshVert.push_back(meshSet[i][j]);
					meshVert.push_back(meshSet[i][j + 1]);
					meshVert.push_back(meshSet[i + 1][j]);
					meshVert.push_back(meshSet[i + 1][j + 1]);				
					Vector3f n1 = Vector3f::cross(meshVert[1] - meshVert[0], meshVert[2] - meshVert[0]);
					Vector3f n2 = Vector3f::cross(meshVert[1] - meshVert[2], meshVert[3] - meshVert[2]);

					 
					glNormal3f(n1[0], n1[1], n1[2]);
					glColor3f(0.0f, 1.0f, 0.0f);
				
					glVertex3f(meshVert[0][0], meshVert[0][1], meshVert[0][2]);
					glVertex3f(meshVert[1][0], meshVert[1][1], meshVert[1][2]);
					glVertex3f(meshVert[2][0], meshVert[2][1], meshVert[2][2]);
					 
					glColor3f(0.0f, 1.0f, 0.0f);
					glVertex3f(meshVert[0][0], meshVert[0][1], meshVert[0][2]);
					glVertex3f(meshVert[2][0], meshVert[2][1], meshVert[2][2]);
					glVertex3f(meshVert[1][0], meshVert[1][1], meshVert[1][2]);
					
					
					glNormal3f(n2[0], n2[1], n2[2]);
					glColor3f(0.0f, 1.0f, 0.0f);
					glVertex3f(meshVert[2][0], meshVert[2][1], meshVert[2][2]);
					glVertex3f(meshVert[1][0], meshVert[1][1], meshVert[1][2]);
					glVertex3f(meshVert[3][0], meshVert[3][1], meshVert[3][2]);					 

					glVertex3f(meshVert[3][0], meshVert[3][1], meshVert[3][2]);
					glVertex3f(meshVert[1][0], meshVert[1][1], meshVert[1][2]);
					glVertex3f(meshVert[2][0], meshVert[2][1], meshVert[2][2]);

				}
			}
			
			glDisable(GL_COLOR_MATERIAL);
			glEnd();
			if (ballState) {
				Vector3f ballCentre = Vector3f(-1.0f, -3.5f, 1.5f);
				float radius = 1.0f;
				float eps = 0.8f;
				glPopAttrib();
				glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
				glEnable(GL_COLOR_MATERIAL);
				glColor3f(1.0f, 1.0f, 0.0f);				
				glPushMatrix();
				glTranslatef(ballCentre.x(), ballCentre.y(), ballCentre.z());
				glutSolidSphere(radius, 100.0f, 100.0f);
				glPopMatrix();
				glDisable(GL_COLOR_MATERIAL);
				for (int n = 12; n < m_numParticles; n++) {
					Vector3f partPoint = getState()[2 * n];
					Vector3f tempState;
					if ((partPoint - ballCentre).abs() <= (radius + eps)) {

						tempState = ballCentre + (radius + eps) * (partPoint - ballCentre).normalized();
						vector<Vector3f> newState = getState();
						newState[2 * n] = tempState;
						setState(newState);

					}

				}
			}
			
		}

		
	 
}

