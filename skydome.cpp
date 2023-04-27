#include "skydome.h"
#include "MatSet.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	//���f���f�[�^�̑��
	this->model = model;
	//���[���h���W
	worldTransform_.scale_ = { 300,300,300 };
	worldTransform_.Initialize();
}

void Skydome::Update() {
	//worldTransform_.rotation_.z += 0.0005f;
	worldTransform_.rotation_.y += 0.0005f;

	//���[���h���W�X�V
	MatSet2 matSet;
	worldTransform_.matWorld_ = matSet.MatIdentity(worldTransform_);
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection viewProjeciton) {
	model->Draw(worldTransform_, viewProjeciton);
}