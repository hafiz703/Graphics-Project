#include "ClothSystem2.h"
#include <iostream>



int ClothSystem2::getStateIndex(int row, int col) {
	return row*cloth_size + col;
}

//TODO: Initialize here
ClothSystem2::ClothSystem2(int numParticles) : PendulumSystem2(numParticles)
{
	m_numParticles = numParticles*numParticles;
	cloth_size = numParticles;
	float seperation = 0.08f;
	is_Cloth = true;
	bool isVerlet = false;

	for (int row = 0; row < numParticles; row++) {
		for (int col = 0; col < numParticles; col++) {
			//cout << "row: "<< row << endl;
			//cout << "col: " << col << endl;
			if (isVerlet) {
				m_vVecState.push_back(Vector3f((float)col*seperation, (float)-1.0f*row*seperation, 0.0f)); //x
				m_vVecState.push_back(Vector3f((float)col*seperation, (float)-1.0f*row*seperation, 0.0f)); //old x
			}
			else {
				m_vVecState.push_back(Vector3f(0.0f, (float)0.5-(-1.0f*row*seperation), (float)(-1.0*cloth_size/2+col)*seperation));
				m_vVecState.push_back(Vector3f(0.0f, 0.0f, 0.0f));
				particles.push_back(Vector3f(0.0f, (float)0.5 - (-1.0f*row*seperation), (float)(-1.0*cloth_size / 2 + col)*seperation));
			}
		}
	}

	structural_force = 60.0f;
	shear_force = 60.0f;
	flex_force = 60.0f;


	//structural springs
	struc_rest_length = 0.2f;

	for (int row = 0; row < cloth_size; row++) {
		for (int col = 0; col < cloth_size; col++) {
			if (row < cloth_size - 1) {
				springs.push_back(Vector4f(getStateIndex(row, col), getStateIndex(row + 1, col), struc_rest_length, structural_force));
			}
			if (col < cloth_size - 1) {
				springs.push_back(Vector4f(getStateIndex(row, col), getStateIndex(row, col + 1), struc_rest_length, structural_force));
			}
		}
	}

	//shear springs
	shear_rest_length = 0.28f;

	for (int row = 0; row < cloth_size; row++) {
		for (int col = 0; col < cloth_size - 1; col++) {
			if ((row < cloth_size - 1) && (col < cloth_size - 1)) {
				springs.push_back(Vector4f(getStateIndex(row, col), getStateIndex(row + 1, col + 1), shear_rest_length, shear_force));
			}

			//cout << "SHEAR1 OKAY" << endl;

			if ((row > 0) && (col < cloth_size - 1)) {
				//cout << "rowcol" << row << endl;
				springs.push_back(Vector4f(getStateIndex(row, col), getStateIndex(row - 1, col + 1), shear_rest_length, shear_force));
			}
		}
	}

	//flexion
	flex_rest_length = 0.4f;

	for (int row = 0; row < cloth_size; row++) {
		for (int col = 0; col < cloth_size; col++) {
			if (col < cloth_size - 2) {
				springs.push_back(Vector4f(getStateIndex(row, col), getStateIndex(row, col + 2), flex_rest_length, flex_force));
			}

			//cout << "FLEX OKAY" << endl;

			if (row < cloth_size - 2) {
				//cout << "rowcol" << row << endl;
				springs.push_back(Vector4f(getStateIndex(row, col), getStateIndex(row + 2, col), flex_rest_length, flex_force));
			}
		}
	}

	adjacencyList.resize(m_numParticles);
	int face_index = 0;
	//faces
	for (int row = 0; row < cloth_size - 1; row++) {
		for (int col = 0; col < cloth_size - 1; col++) {
			int i = getStateIndex(row, col);

			faces1.push_back(Vector3f(i + cloth_size, i + cloth_size + 1, i + 1)); //ccw
			faces1.push_back(Vector3f(i + cloth_size, i + 1, i));

			faces2.push_back(Vector3f(i + cloth_size, i + 1, i + cloth_size + 1)); //cw
			faces2.push_back(Vector3f(i + cloth_size, i, i + 1));

			adjacencyList[i + cloth_size].push_back(face_index);
			adjacencyList[i + cloth_size].push_back(face_index + 1);

			adjacencyList[i + cloth_size + 1].push_back(face_index);

			adjacencyList[i + 1].push_back(face_index);
			adjacencyList[i + 1].push_back(face_index + 1);

			adjacencyList[i].push_back(face_index + 1);

			face_index += 2;

		}
	}

	std::cout << "faces" << endl;


}
