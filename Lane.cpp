#include"Lane.h"
#include <cassert>
#include "MatSet.h"

void Lane::Initialize(Model* laneModel, Model* lineModel) {
	//null�`�F�b�N���Ă��
	assert(laneModel);
	assert(lineModel);
	//���f���f�[�^��ǂݍ���
	this->laneModel = laneModel;
	defaultModel = laneModel;

	line = lineModel;

	musicData.beatDenomonator = 0;
	musicData.beatMolecule = 0;
	musicData.BPM = 0;
	musicData.speed = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < maxNotes; j++) {
			musicData.layer[i].chart[j] = 0;
			musicData.layer[i].hit[j] = false;
			musicData.layer[i].hitTimer[j] = 0;
			musicData.layer[i].judgement[j] = 0;
			musicData.layer[i].worldTransform[j].translation_ = { 0,0,0 };
			musicData.layer[i].startMove[j] = false;
		}
		musicData.layer[i].speed = 0;
	}

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	lanePosition.translation_ = Vector3(0, -2.0f, -35.0f);
	lanePosition.Initialize();

	countFlame = 0;
	countRhythm = 0;

	startTimer = resetStartTimer;
}

void Lane::Update() {
	MatSet2 matset;

	//�^�C�}�[��0�ɂȂ�����X�^�[�g
	if (startTimer > 0) {
		startTimer--;
	}
	else if (startTimer < 0) {
		switching = baseBPM * change;
		if (countFlame >= switching) {
			countRhythm++;
			countFlame = 0;
		}
		countFlame++;
	}

	//���[���̍X�V
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition,viewProjection);
	line->Draw(lanePosition, viewProjection);
}

void Lane::LoadMusic(MusicData musicData) {
	//���y�f�[�^���R�s�[����
	this->musicData.beatDenomonator = musicData.beatDenomonator;
	this->musicData.beatMolecule = musicData.beatMolecule;
	this->musicData.BPM = musicData.BPM;
	this->musicData.speed = musicData.speed;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < maxNotes; j++) {
			this->musicData.layer[i].chart[j] = musicData.layer[i].chart[j];
		}
	}
	//�擾�f�[�^��f�ɓǂݍ���
	change = baseBPM / this->musicData.BPM;
}

void Lane::ResetMusic() {
	//���y�f�[�^�폜
	musicData.beatDenomonator = 0;
	musicData.beatMolecule = 0;
	musicData.BPM = 0;
	musicData.speed = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < maxNotes; j++) {
			musicData.layer[i].chart[j] = 0;
			musicData.layer[i].hit[j] = false;
			musicData.layer[i].hitTimer[j] = 0;
			musicData.layer[i].judgement[j] = 0;
			musicData.layer[i].worldTransform[j].translation_ = {0,0,0};
			musicData.layer[i].startMove[j] = false;
		}
		musicData.layer[i].speed = 0;
	}
}

void Lane::ChangeLane(Model* model) {
	//null�`�F�b�N
	assert(model);
	//��񃂃f���f�[�^������
	laneModel = nullptr;
	//���̌�����
	laneModel = model;
}

void Lane::DefaultLane() {
	//��񃂃f���f�[�^������
	laneModel = nullptr;
	//���̌�����
	laneModel = defaultModel;
}

void Lane::Auto(bool select) {
	if (select) {
		autoPlay = true;
	}
	else {
		autoPlay = false;
	}
}

void Lane::ReadChart(int notes,int i) {
	//�m�[�c�̎�ނɍ��킹�Đݒ肷��
	//�m�[�c�̓��������J�n����
	musicData.layer[i].startMove[notes] = true;
	if (musicData.layer[i].chart[notes] == 1) {

	}
}