#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel, Model* noteModel[21]) {
	//null�`�F�b�N���Ă��
	assert(laneModel);
	assert(lineModel);
	for (int i = 0; i < modelNum; i++) {
		assert(noteModel[i]);
	}

	//�m�[�c���f��nullptr���
	for (int i = 0; i < modelNum; i++) {
		notesModel[i] = nullptr;
	}

	//���f���f�[�^��ǂݍ���
	this->laneModel = laneModel;
	defaultModel = laneModel;

	this->lineModel = lineModel;

	for (int i = 0; i < modelNum; i++) {
		notesModel[i] = noteModel[i];
	}

	//���ʃf�[�^������
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = 0;
				if (k < drawNotes) {
					playData.layer[i].note[j].hit[k] = false;
					playData.layer[i].note[j].hitTimer[k] = 0;
					playData.layer[i].note[j].judgement[k] = 0;
					playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
					playData.layer[i].note[j].worldTransform[k].Initialize();
					playData.layer[i].note[j].startMove[k] = false;
				}
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	//���ʑ��x�i�f�t�H���g�j
	speed = 0.3f;

	distance = speed * 60.0f;//(60.0f->1�b)

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

	startMusic = false;

	chartNum = 0;
}

void Lane::Update() {
	MatSet2 matset;
	//�f�o�b�O�p�ɃX�y�[�X�������Ă�Ԃ����i�ނ悤��
	if (input_->TriggerKey(DIK_SPACE)) {
		if (!moveFlag) {
			moveFlag = true;
		}
		else if (moveFlag) {
			moveFlag = false;
		}
	}
	//if (input_->PushKey(DIK_SPACE)) {
		//�^�C�}�[��0�ɂȂ�����X�^�[�g
	if (moveFlag) {
		if (startTimer > 0) {
			startTimer--;
		}
		if (startTimer <= 60) {
			LineUpdate();
		}
		if (startMusic) {
			ReadChart();
		}
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

	//�m�[�c�X�V
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
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
	//}
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition, viewProjection);
	for (int i = 0; i < line.lineNum; i++) {
		if (line.linePop[i]) {
			lineModel->Draw(line.lineWorld[i], viewProjection);
		}
	}
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.layer[i].note[j].type[k] == 1) {
					notesModel[0]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
				}
				else if (playData.layer[i].note[j].type[k] == 2 || playData.layer[i].note[j].type[k] == 5 || playData.layer[i].note[j].type[k] == 6) {
					notesModel[1]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
				}
				else if (playData.layer[i].note[j].type[k] == 3 || playData.layer[i].note[j].type[k] == 4) {
					notesModel[2]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
				}
			}
		}
	}
}

void Lane::LoadMusic(int ID) {

	//���y�f�[�^���R�s�[����
	playData.beatDenomonator = musicData[ID].beatDenomonator;
	playData.beatMolecule = musicData[ID].beatMolecule;
	playData.BPM = musicData[ID].BPM;
	playData.speed = musicData[ID].speed;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = musicData[ID].layer[i].note[j].chart[k];
				if (k < drawNotes) {
					playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.0f - i * 0.01f, -45.5f + distance * playData.speed);
				}
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
				if (k < drawNotes) {
					playData.layer[i].note[j].hit[k] = false;
					playData.layer[i].note[j].hitTimer[k] = 0;
					playData.layer[i].note[j].judgement[k] = 0;
					playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
					playData.layer[i].note[j].startMove[k] = false;
				}
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	startMusic = 0;
	startTimer = resetStartTimer;
	chartNum = 0;
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

void Lane::LineUpdate() {
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
			if (!startMusic) {
				startMusic = true;
			}
		}
	}
	line.countFlame++;
	//���ߐ��ړ�
	for (int i = 0; i < line.lineNum; i++) {
		if (line.linePop[i]) {
			line.lineWorld[i].translation_.z -= speed;
			if (line.lineWorld[i].translation_.z < -50.0f) {
				//���胉�C����ʂ�߂��ĉ�ʊO�܂ōs������false�ɂ��ď����ʒu�ɖ߂��ă��T�C�N��
				float distance = speed * 60.0f;//(60.0f->1�b)
				line.lineWorld[i].translation_ = Vector3(0, -2.0f, -45.5f + distance * playData.speed);
				line.linePop[i] = false;
			}
		}
	}
}

void Lane::ReadChart() {
	//1�t���[�����Ƃɕ��ʂ�ǂ�
	for (int i = 0; i < layerNum; i++) {	//���C���[��
		for (int j = 0; j < columnNum; j++) {	//��
			if (playData.layer[i].note[j].chart[chartNum] == 1) {	//�m�[�c�̗L���`�F�b�N
				for (int k = 0; k < drawNotes; k++) {	//�󂢂Ă鏇����t���O���I���ɂ���
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						break;											//�󂫂����������瑦�E�o
					}
				}
			}
			else if (playData.layer[i].note[j].chart[chartNum] == 2 || playData.layer[i].note[j].chart[chartNum] == 5 || playData.layer[i].note[j].chart[chartNum] == 6) {	//�m�[�c�̗L���`�F�b�N
				for (int k = 0; k < drawNotes; k++) {	//�󂢂Ă鏇����t���O���I���ɂ���
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						break;											//�󂫂����������瑦�E�o
					}
				}
			}
			else if (playData.layer[i].note[j].chart[chartNum] == 3 || playData.layer[i].note[j].chart[chartNum] == 4) {	//�m�[�c�̗L���`�F�b�N
				for (int k = 0; k < drawNotes; k++) {	//�󂢂Ă鏇����t���O���I���ɂ���
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.01f - i * 0.01f, -45.5f + distance * playData.speed);
						break;											//�󂫂����������瑦�E�o
					}
				}
			}
		}
	}

	for (int i = 0; i < layerNum; i++) {	//���C���[��
		for (int j = 0; j < columnNum; j++) {	//��
			for (int k = 0; k < drawNotes; k++) {	//�\���\�m�[�c��
				if (playData.layer[i].note[j].startMove[k]) {	//�X�^�[�g�t���O�I���Ȃ�
					playData.layer[i].note[j].worldTransform[k].translation_.z -= speed;	//���ʑ��x�ɍ��킹�Ĉړ�
					//�m�[�c����胉�C����ʂ�߂��邩�q�b�g���肪�I���Ń��Z�b�g
					if (playData.layer[i].note[j].worldTransform[k].translation_.z < -50.0f || playData.layer[i].note[j].hit[k]) {
						playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.0f - i * 0.01f, -45.5f + distance * playData.speed);
						playData.layer[i].note[j].hit[k] = false;
						playData.layer[i].note[j].hitTimer[k] = 0;
						playData.layer[i].note[j].type[k] = 0;
						playData.layer[i].note[j].startMove[k] = false;
						playData.layer[i].note[j].judgement[k] = 0;
					}
				}
			}
		}
	}

	//�J�E���g��i�߂�
	chartNum++;
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
	//���ʃf�[�^������t�@�C���̏ꏊ�Ɗi�[�������z��̔ԍ����w�肷��
	ID000("Resources/musicData/testmc.txt",0);
}

void Lane::ID000(string filePass,int musicID) {
	//test�p�f�[�^
	//����
	musicData[musicID].beatDenomonator = 4;
	//���q
	musicData[musicID].beatMolecule = 4;
	//BPM
	musicData[musicID].BPM = 175;
	//���ʑ��x�i�{���j
	musicData[musicID].speed = 1;

	LoadData(musicID, filePass);
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

void Lane::SetNote(int i, int j,int k, int typeNum) {
	playData.layer[i].note[j].startMove[k] = true;	//�J�n�t���O�I��
	playData.layer[i].note[j].type[k] = typeNum;			//�m�[�c�̎�ސݒ�
	playData.layer[i].note[j].hitTimer[k] = 60;		//����J�E���g�̐ݒ�
}