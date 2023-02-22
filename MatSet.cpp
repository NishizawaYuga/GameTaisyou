#include "MatSet.h"

MatSet::MatSet() {
	//matWorld_に単位行列を代入
		/*
			1, 0, 0, 0
			0, 1, 0, 0
			0, 0, 1, 0
			0, 0, 0, 1
		*/
		//単位行列
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;
}

void MatSet::MatIdentity(WorldTransform& worldTransform) {
	this->worldTransform = worldTransform;

	//スケーリング倍率を行列に設定する
	/*
		Sx,0,0,0
		0,Sy,0,0
		0,0,Sz,0
		0,0, 0,1
	*/
	matScale.m[0][0] = this->worldTransform.scale_.x; //Sx
	matScale.m[1][1] = this->worldTransform.scale_.y; //Sy
	matScale.m[2][2] = this->worldTransform.scale_.z; //Sz
	matScale.m[3][3] = 1;						 // 1

	

	//Z軸回転行列の各要素を設定する
	/*
		 cos, sin, 0, 0
		-sin, cos, 0, 0
		   0,   0, 1, 0
		   0,   0, 0, 1
	*/
	matRotZ = matIdentity;
	matRotZ.m[0][0] = cos(this->worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(this->worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(this->worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(this->worldTransform.rotation_.z);

	//X軸回転行列の各要素を設定する
	/*
		   1,    0,   0, 0
		   0,  cos, sin, 0
		   0, -sin, cos, 0
		   0,    0,   0, 1
	*/
	matRotX = matIdentity;
	matRotX.m[1][1] = cos(this->worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(this->worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(this->worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(this->worldTransform.rotation_.x);

	//Y軸回転行列の各要素を設定する
	/*
		 cos,   0, -sin, 0
		   0,   1,    0, 0
		 sin,   0,  cos, 0
		   0,   0,    0, 1
	*/
	matRotY = matIdentity;
	matRotY.m[0][0] = cos(this->worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(this->worldTransform.rotation_.y);
	matRotY.m[2][0] = sin(this->worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(this->worldTransform.rotation_.y);

	//各軸の回転行列を合成
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//移動量を行列に設定する
	/*
		 1,  0,  0, 0
		 0,  1,  0, 0
		 0,  0,  1, 0
		Tx, Ty, Tz, 1
	*/
	matTrans.m[3][0] = this->worldTransform.translation_.x;
	matTrans.m[3][1] = this->worldTransform.translation_.y;
	matTrans.m[3][2] = this->worldTransform.translation_.z;

	//代入
	this->worldTransform.matWorld_ = matIdentity;
	this->worldTransform.matWorld_ *= matScale;
	this->worldTransform.matWorld_ *= matRot;
	this->worldTransform.matWorld_ *= matTrans;

	//スケーリング行列　→　回転行列　→　平行移動行列の順に掛け算

	worldTransform.matWorld_ = this->worldTransform.matWorld_;
}

Matrix4& MatSet2::MatIdentity(WorldTransform& worldTransform) {
	//matWorld_に単位行列を代入
		/*
			1, 0, 0, 0
			0, 1, 0, 0
			0, 0, 1, 0
			0, 0, 0, 1
		*/
		//単位行列
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;

	//スケーリング倍率を行列に設定する
	/*
		Sx,0,0,0
		0,Sy,0,0
		0,0,Sz,0
		0,0, 0,1
	*/
	matScale.m[0][0] = worldTransform.scale_.x; //Sx
	matScale.m[1][1] = worldTransform.scale_.y; //Sy
	matScale.m[2][2] = worldTransform.scale_.z; //Sz
	matScale.m[3][3] = 1;						 // 1



	//Z軸回転行列の各要素を設定する
	/*
		 cos, sin, 0, 0
		-sin, cos, 0, 0
		   0,   0, 1, 0
		   0,   0, 0, 1
	*/
	matRotZ = matIdentity;
	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);

	//X軸回転行列の各要素を設定する
	/*
		   1,    0,   0, 0
		   0,  cos, sin, 0
		   0, -sin, cos, 0
		   0,    0,   0, 1
	*/
	matRotX = matIdentity;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);

	//Y軸回転行列の各要素を設定する
	/*
		 cos,   0, -sin, 0
		   0,   1,    0, 0
		 sin,   0,  cos, 0
		   0,   0,    0, 1
	*/
	matRotY = matIdentity;
	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);

	//各軸の回転行列を合成
	matRot = matIdentity;
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//移動量を行列に設定する
	/*
		 1,  0,  0, 0
		 0,  1,  0, 0
		 0,  0,  1, 0
		Tx, Ty, Tz, 1
	*/
	matTrans.m[3][0] = worldTransform.translation_.x;
	matTrans.m[3][1] = worldTransform.translation_.y;
	matTrans.m[3][2] = worldTransform.translation_.z;

	//代入
	worldTransform.matWorld_ = matIdentity;
	worldTransform.matWorld_ *= matScale;
	worldTransform.matWorld_ *= matRot;
	worldTransform.matWorld_ *= matTrans;

	//スケーリング行列　→　回転行列　→　平行移動行列の順に掛け算

	return worldTransform.matWorld_;
}