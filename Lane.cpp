#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel,Model*noteModel[21]) {
	//null�`�F�b�N���Ă��
	assert(laneModel);
	assert(lineModel);
	//����tap�����������݂��Ȃ��̂�0����
	assert(noteModel[0]);

	//�m�[�c���f��nullptr���
	for (int i = 0; i < modelNum; i++) {
		notesModel[i] = nullptr;
	}

	//���f���f�[�^��ǂݍ���
	this->laneModel = laneModel;
	defaultModel = laneModel;

	this->lineModel = lineModel;

	notesModel[0] = noteModel[0];

	//���ʃf�[�^������
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = 0;
				playData.layer[i].note[j].hit[k] = false;
				playData.layer[i].note[j].hitTimer[k] = 0;
				playData.layer[i].note[j].judgement[k] = 0;
				playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
				playData.layer[i].note[j].worldTransform[k].Initialize();
				playData.layer[i].note[j].startMove[k] = false;
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	//���ʑ��x�i�f�t�H���g�j
	speed = 0.4f;

	//���ߐ�������
	line.countFlame = 0;
	line.countRhythm = 1;

	//���ʏ�����
	ChartInitialize();

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//���ߐ�
	for (int i = 0; i < line.lineNum; i++) {
		line.linePop[i] = false;
		line.lineWorld[i].translation_ = Vector3(0, 0, 0);
		line.lineWorld[i].Initialize();
	}

	//���[��
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
	else if (startTimer <= 60) {
		line.switching = line.baseBPM * line.change;
		if (line.countFlame >= line.switching) {
			line.countRhythm++;
			line.countFlame = 0;
			//�Ȃ̔����i���q�j�ɍ��킹�ď��ߐ��^�C�~���O�n��
			if (line.countRhythm >= playData.beatMolecule + 1) {
				line.countRhythm = 1;
				//�g���Ă��Ȃ����ߐ����I���ɂ���
				for (int i = 0; i < line.lineNum; i++) {
					if (!line.linePop[i]) {
						line.linePop[i] = true;
						break;
					}
				}
			}
		}
		line.countFlame++;
		//���ߐ��ړ�
		for (int i = 0; i < line.lineNum; i++) {
			if (line.linePop[i]) {
				line.lineWorld[i].translation_.z -= speed;
				if (line.lineWorld[i].translation_.z < -50.0f){
					//���胉�C����ʂ�߂��ĉ�ʊO�܂ōs������false�ɂ��ď����ʒu�ɖ߂��ă��T�C�N��
					float distance = speed * 60.0f;//(60.0f->1�b)
					line.lineWorld[i].translation_ = Vector3(0, -2.0f, -45.5f + distance * playData.speed);
					line.linePop[i] = false;
				}
			}
		}

		playData.layer[0].note[0].worldTransform[0].translation_.z -= speed;
	}

	debugText_->SetPos(10, 10);
	debugText_->Printf("BPM : %d", playData.BPM);
	debugText_->SetPos(10, 30);
	debugText_->Printf("Rhythm : %d", line.countRhythm);
	debugText_->SetPos(10, 50);
	debugText_->Printf("countFlame : %d", line.countFlame);
	debugText_->SetPos(10, 70);
	debugText_->Printf("change : %f", line.change);
	debugText_->SetPos(10, 90);
	debugText_->Printf("lineNum : %d", line.lineNum);
	debugText_->SetPos(10, 110);
	debugText_->Printf("Timer : %d", startTimer);
	debugText_->SetPos(10, 130);
	debugText_->Printf("noteZ : %f", playData.layer[0].note[0].worldTransform[0].translation_.z);

	//���ߐ��X�V
	for (int i = 0; i < line.lineNum; i++) {
		matset.MatIdentity(line.lineWorld[i]);
		line.lineWorld[i].TransferMatrix();
	}
	playData.layer[0].note[0].startMove[0] = true;

	//�m�[�c�X�V
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				if (playData.layer[i].note[j].startMove[k]) {
					matset.MatIdentity(playData.layer[i].note[j].worldTransform[k]);
					playData.layer[i].note[j].worldTransform[k].TransferMatrix();
				}
			}
		}
	}
	//���[���̍X�V
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition, viewProjection);
	for(int i = 0;i < line.lineNum;i++){
		if (line.linePop[i]) {
			lineModel->Draw(line.lineWorld[i], viewProjection);
		}
	}
	notesModel[0]->Draw(playData.layer[0].note[0].worldTransform[0], viewProjection);
}

void Lane::LoadMusic(int ID) {
	float distance = speed * 60.0f;//(60.0f->1�b)

	//���y�f�[�^���R�s�[����
	playData.beatDenomonator = musicData[ID].beatDenomonator;
	playData.beatMolecule = musicData[ID].beatMolecule;
	playData.BPM = musicData[ID].BPM;
	playData.speed = musicData[ID].speed;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = musicData[ID].layer[i].note[j].chart[k];
				playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0, -2.0f, -45.5f + distance * playData.speed);
			}
		}
	}
	//�擾�f�[�^��f�ɓǂݍ���
	line.change = line.baseBPM / playData.BPM;
	//���ߐ��̈ʒu����
	for (int i = 0; i < line.lineNum; i++) {
		//���胉�C���܂ł̋����� �W���X�g���W + (�ړ����x * 1�b�̃t���[����) * �Ȃ��Ƃ̑��x�{�� �Ŏ�� ����ʂɏo�����画�胉�C���܂�1�b�ŗ���z��(�f�t�H)
		line.lineWorld[i].translation_ = Vector3(0, -2.0f, -45.5f + distance * playData.speed);
		line.lineWorld[i].TransferMatrix();
	}
}

void Lane::ResetMusic() {
	//���y�f�[�^�폜
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = 0;
				playData.layer[i].note[j].hit[k] = false;
				playData.layer[i].note[j].hitTimer[k] = 0;
				playData.layer[i].note[j].judgement[k] = 0;
				playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
				playData.layer[i].note[j].startMove[k] = false;
			}
			playData.layer[i].note[j].speed = 0;
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
	playData.layer[i].note[j].startMove[notes] = true;
	if (playData.layer[i].note[j].chart[notes] == 1) {

	}
}

void Lane::ChartInitialize() {
	//�S�Ă̋ȃf�[�^�������i�l�dfor���c�j
	for (int M = 0; M < musicNum; M++) {								//�Ȑ�
		for (int i = 0; i < layerNum; i++) {						//���C���[��
			for (int j = 0; j < columnNum; j++) {					//��
				for (int k = 0; k < maxNotes; k++) {				//�u����m�[�c��
					musicData[M].layer[i].note[j].chart[k] = 0;
				}
			}
		}
		musicData[M].BPM = 0;
		musicData[M].beatDenomonator = 0;
		musicData[M].beatMolecule = 0;
		musicData[M].speed = 0;
	}

	//�S�Ă̋Ȃ̃f�[�^�ǂݍ���
	ID000("Resources/musicData/testmc.txt");
}

void Lane::ID000(string filePass) {
	//test�p�f�[�^
	//����
	musicData[0].beatDenomonator = 4;
	//���q
	musicData[0].beatMolecule = 4;
	//BPM
	musicData[0].BPM = 120;
	//���ʑ��x�i�{���j
	musicData[0].speed = 1;

	LoadData(0, filePass);
}

void Lane::LoadData(int ID, string filePass) {

	//���͂��ꂽ�t�@�C���p�X����t�@�C���I�u�W�F�N�g�쐬
	ifstream ifs(filePass);

	//�J���Ȃ�������G���[
	if (!ifs) {
		cout << "�t�@�C�����J���܂���ł���" << endl;
		return;
	}

	string str = "";

	//���C���[
	int i = 0;
	//��
	int j = 0;
	//�u����m�[�c��
	int k = 0;

	//�t�@�C���̒��g����s���ǂݍ���
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		//��؂蕶�����Ȃ��Ȃ�܂ŕ�������؂�
		while (getline(stream, tmp, ',')) {
			musicData[ID].layer[i].note[j].chart[k] = atoi(tmp.c_str());
			k++;
		}
		//�m�[�c�ǂݍ��݂��ŏ������
		k = 0;
		//���̗��
		j++;
		//j���S�Ă̍s��ǂݍ��񂾂烊�Z�b�g���ă��C���[�ύX
		if (j > 3) {
			j = 0;
			i++;
		}
	}
}