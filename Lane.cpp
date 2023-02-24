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

	this->lineModel = lineModel;

	//���ʃf�[�^������
	musicData.beatDenomonator = 0;
	musicData.beatMolecule = 0;
	musicData.BPM = 0;
	musicData.speed = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < maxNotes; k++) {
				musicData.layer[i].note[j].chart[k] = 0;
				musicData.layer[i].note[j].hit[k] = false;
				musicData.layer[i].note[j].hitTimer[k] = 0;
				musicData.layer[i].note[j].judgement[k] = 0;
				musicData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
				musicData.layer[i].note[j].startMove[k] = false;
			}
			musicData.layer[i].note[j].speed = 0;
		}
	}

	//���ߐ�������
	line.lineNum = 1;
	line.countFlame = 0;
	line.countRhythm = 1;


	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	lanePosition.translation_ = Vector3(0, -2.0f, -35.0f);
	lanePosition.Initialize();

	startTimer = resetStartTimer;
}

void Lane::Update() {
	MatSet2 matset;

	//�^�C�}�[��0�ɂȂ�����X�^�[�g
	if (startTimer > 0) {
		startTimer--;
	}
	else if (startTimer < 0) {
		line.switching = line.baseBPM * line.change;
		if (line.countFlame >= line.switching) {
			line.countRhythm++;
			line.countFlame = 0;
			//�Ȃ̔����i���q�j�ɍ��킹�ď��ߐ��^�C�~���O�n��
			if (line.countRhythm >= musicData.beatMolecule) {
				line.countRhythm = 1;
				line.linePop[line.lineNum] = true;
				line.lineNum++;
			}
		}
		line.countFlame++;
	}

	debugText_->SetPos(10, 10);
	debugText_->Printf("BPM : %d", musicData.BPM);
	debugText_->SetPos(10, 30);
	debugText_->Printf("Rhythm : %d", line.countRhythm);
	debugText_->SetPos(10, 50);
	debugText_->Printf("countFlame : %d", line.countFlame);
	debugText_->SetPos(10, 70);
	debugText_->Printf("change : %f", line.change);
	debugText_->SetPos(10, 90);
	debugText_->Printf("lineNum : %d", line.lineNum);

	//���[���̍X�V
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition, viewProjection);
	lineModel->Draw(lanePosition, viewProjection);
}

void Lane::LoadMusic(MusicData musicData) {
	//���y�f�[�^���R�s�[����
	this->musicData.beatDenomonator = musicData.beatDenomonator;
	this->musicData.beatMolecule = musicData.beatMolecule;
	this->musicData.BPM = musicData.BPM;
	this->musicData.speed = musicData.speed;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < maxNotes; k++) {
				this->musicData.layer[i].note[j].chart[k] = musicData.layer[i].note[j].chart[k];
			}
		}
	}
	//�擾�f�[�^��f�ɓǂݍ���
	line.change = line.baseBPM / this->musicData.BPM;
}

void Lane::ResetMusic() {
	//���y�f�[�^�폜
	musicData.beatDenomonator = 0;
	musicData.beatMolecule = 0;
	musicData.BPM = 0;
	musicData.speed = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < maxNotes; k++) {
				musicData.layer[i].note[j].chart[k] = 0;
				musicData.layer[i].note[j].hit[k] = false;
				musicData.layer[i].note[j].hitTimer[k] = 0;
				musicData.layer[i].note[j].judgement[k] = 0;
				musicData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
				musicData.layer[i].note[j].startMove[k] = false;
			}
			musicData.layer[i].note[j].speed = 0;
		}
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

void Lane::ReadChart(int notes, int i, int j) {
	//�m�[�c�̎�ނɍ��킹�Đݒ肷��
	//�m�[�c�̓��������J�n����
	musicData.layer[i].note[j].startMove[notes] = true;
	if (musicData.layer[i].note[j].chart[notes] == 1) {

	}
}