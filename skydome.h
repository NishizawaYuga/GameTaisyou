#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Skydome {
public:
	//������
	void Initialize(Model* model);
	//�X�V
	void Update();
	//�`��
	void Draw(ViewProjection viewProjection);
private:
	//���[���h���W
	WorldTransform worldTransform_;
	//���f��
	Model* model = nullptr;

};
