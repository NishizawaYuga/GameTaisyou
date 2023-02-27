#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel) {
	//null�`�F�b�N���Ă��
	assert(laneModel);
	assert(lineModel);
	//���f���f�[�^��ǂݍ���
	this->laneModel = laneModel;
	defaultModel = laneModel;

	this->lineModel = lineModel;

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
				playData.layer[i].note[j].startMove[k] = false;
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	//���ߐ�������
	line.lineNum = 1;
	line.countFlame = 0;
	line.countRhythm = 1;

	//���ʏ�����
	ChartInitialize();

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
	else if (startTimer <= 0) {
		line.switching = line.baseBPM * line.change;
		if (line.countFlame >= line.switching) {
			line.countRhythm++;
			line.countFlame = 0;
			//�Ȃ̔����i���q�j�ɍ��킹�ď��ߐ��^�C�~���O�n��
			if (line.countRhythm >= playData.beatMolecule + 1) {
				line.countRhythm = 1;
				line.linePop[line.lineNum] = true;
				line.lineNum++;
			}
		}
		line.countFlame++;
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

	//���[���̍X�V
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition, viewProjection);
	lineModel->Draw(lanePosition, viewProjection);
}

void Lane::LoadMusic(int ID) {
	//���y�f�[�^���R�s�[����
	playData.beatDenomonator = musicData[ID].beatDenomonator;
	playData.beatMolecule = musicData[ID].beatMolecule;
	playData.BPM = musicData[ID].BPM;
	playData.speed = musicData[ID].speed;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = musicData[ID].layer[i].note[j].chart[k];
			}
		}
	}
	//�擾�f�[�^��f�ɓǂݍ���
	line.change = line.baseBPM / playData.BPM;
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
	ID000();
}

void Lane::ID000() {
	//test�p�f�[�^
	//����
	musicData[0].beatDenomonator = 4;
	//���q
	musicData[0].beatMolecule = 4;
	//BPM
	musicData[0].BPM = 120;
	//���ʑ��x�i�{���j
	musicData->speed = 1;

	LoadData(0, "musicData/testmc.txt");
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