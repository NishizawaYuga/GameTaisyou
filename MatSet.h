#pragma once
#include <math.h>
#include "PrimitiveDrawer.h"
#include "WorldTransform.h"

class MatSet {

public:
	MatSet();
	void MatIdentity(WorldTransform& worldTransform);
	WorldTransform worldTransform;

	struct MatData MatIdentity(WorldTransform* worldTransform);

private:

	Matrix4 matScale;
	Matrix4 matIdentity;
	Matrix4 matRot = matIdentity;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matTrans = MathUtility::Matrix4Identity();
};

struct MatSet2 {

public:
	//void MatIdentity(Vector3 scale, Vector3 rotation, Vector3 translation, Matrix4 matWorld);

	Matrix4& MatIdentity(WorldTransform& worldTransform);

private:
	Matrix4 matScale;
	Matrix4 matIdentity;
	Matrix4 matRot = matIdentity;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matTrans = MathUtility::Matrix4Identity();

};
