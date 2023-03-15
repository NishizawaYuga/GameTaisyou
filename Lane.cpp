#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel, Model* noteModel[12]) {
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
					playData.layer[i].note[j].hitTimer[k] = 60;
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
	audioSE = Audio::GetInstance();
	audioSE->Initialize();
	audioMusic = Audio::GetInstance();
	audioMusic->Initialize();

	//���y
	music[0] = audioMusic->LoadWave("musicData/000/test.wav");
	music[1] = audioMusic->LoadWave("musicData/001/banbado.wav");

	//���ʉ�
	SE[0] = audioSE->LoadWave("se/perfect.wav");
	SE[1] = audioSE->LoadWave("se/great.wav");

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

	combo = 0;
	maxCombo = 0;
	perfect = 0;
	great = 0;
	miss = 0;

	fastJudge = 4;
	lateJudge = -4;

	autoPlay = true;
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
		if (startTimer > -121) {
			startTimer--;
		}
		if (startTimer <= 60) {
			LineUpdate();
		}
		if (startMusic) {
			ReadChart();
			Judgement();
			if (startTimer == -100) {
				//testmc�͊J�n120
				//if (!audioMusic->IsPlaying(music[0])) {
				audioMusic->PlayWave(music[1]);
			}
			//}
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
	debugText_->SetPos(10, 150);
	debugText_->Printf("Combo : %d", combo);
	debugText_->SetPos(10, 170);
	debugText_->Printf("MAXCombo : %d", maxCombo);
	debugText_->SetPos(10, 190);
	debugText_->Printf("PERFECT : %d", perfect);
	debugText_->SetPos(10, 210);
	debugText_->Printf("GREAT : %d", great);
	debugText_->SetPos(10, 230);
	debugText_->Printf("MISS : %d", miss);

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
				int drawNum = playData.layer[i].note[j].type[k];
				if (drawNum > 20) { drawNum -= 12;}
				else if (drawNum > 16) { drawNum -= 8; }
				else if (drawNum > 12) { drawNum -= 8; }
				else if (drawNum > 8) { drawNum -= 4; }

				if (playData.layer[i].note[j].type[k] != 0) {
					notesModel[drawNum - 1]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
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
					playData.layer[i].note[j].hitTimer[k] = 60;
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

void Lane::Judgement() {
	//�e�m�[�c��hit����
	for (int i = 0; i < layerNum; i++) {
		for (int k = 0; k < drawNotes; k++) {
			//�񂲂Ƃ�hit��������
			//�m�[�c�̎�ނɂ���ď����̎d����ς���
			if (playData.layer[i].note[0].type[k] % 4 == 1) { ColumnHit(i, 0, k, input_->TriggerKey(DIK_F), input_->PushKey(DIK_F)); }
			if (playData.layer[i].note[1].type[k] % 4 == 1) { ColumnHit(i, 1, k, input_->TriggerKey(DIK_G), input_->PushKey(DIK_G)); }
			if (playData.layer[i].note[2].type[k] % 4 == 1) { ColumnHit(i, 2, k, input_->TriggerKey(DIK_H), input_->PushKey(DIK_H)); }
			if (playData.layer[i].note[3].type[k] % 4 == 1) { ColumnHit(i, 3, k, input_->TriggerKey(DIK_J), input_->PushKey(DIK_J)); }
			//1/2�m�[�c
			if (playData.layer[i].note[0].type[k] % 4 == 2) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G)));
			}
			if (playData.layer[i].note[1].type[k] % 4 == 2) {
				ColumnHit(i, 1, k, ThickColumn(input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H)), ThickColumn(input_->PushKey(DIK_G), input_->PushKey(DIK_H)));
			}
			if (playData.layer[i].note[2].type[k] % 4 == 2) {
				ColumnHit(i, 2, k, ThickColumn(input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
			//3/4�m�[�c
			if (playData.layer[i].note[0].type[k] % 4 == 3) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G), input_->PushKey(DIK_H)));
			}
			if (playData.layer[i].note[1].type[k] % 4 == 3) {
				ColumnHit(i, 1, k, ThickColumn(input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_G), input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
			//1/1�m�[�c
			if (playData.layer[i].note[0].type[k] % 4 == 0 && playData.layer[i].note[0].type[k] != 0) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G), input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
		}
	}
	//hitTimer����ɕ]������
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.layer[i].note[j].hit[k]) {
					//PERFECT����i6�t���[��)
					if (playData.layer[i].note[j].hitTimer[k] >= lateJudge && playData.layer[i].note[j].hitTimer[k] <= fastJudge) {
						playData.layer[i].note[j].judgement[k] = 1;
						if (playData.layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[0]);
						}
					}
					//GREAT����(FAST)�i2�t���[���j
					else if (playData.layer[i].note[j].hitTimer[k] > fastJudge && playData.layer[i].note[j].hitTimer[k] <= fastJudge + 2) {
						playData.layer[i].note[j].judgement[k] = 2;
						if (playData.layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[1]);
						}
						if (playData.layer[i].note[j].type[k] > 12 && playData.layer[i].note[j].type[k] < 17) {
							//HOLD�I�_�̂�FAST���薳��
							playData.layer[i].note[j].judgement[k] = 1;
						}
					}
					//GREAT����(LATE)�i2�t���[���j
					else if (playData.layer[i].note[j].hitTimer[k] < lateJudge && playData.layer[i].note[j].hitTimer[k] >= lateJudge - 2) {
						playData.layer[i].note[j].judgement[k] = 2;
						if (playData.layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[1]);
						}
					}
					//MISS����(FAST)�i1�t���[���j
					else if (playData.layer[i].note[j].hitTimer[k] > fastJudge + 2 && playData.layer[i].note[j].hitTimer[k] <= fastJudge + 3) {
						playData.layer[i].note[j].judgement[k] = 3;
						if (playData.layer[i].note[j].type[k] > 12 && playData.layer[i].note[j].type[k] < 17) {
							//HOLD�I�_�̂�FAST���薳��
							playData.layer[i].note[j].judgement[k] = 1;
						}
					}
				}
				//MISS����i�X���[����j
				if (playData.layer[i].note[j].hitTimer[k] < lateJudge - 2) {
					playData.layer[i].note[j].judgement[k] = 3;
					playData.layer[i].note[j].hit[k] = true;
				}
				//�]���ɉ����ăJ�E���g����l��ϓ�������
				//HOLD�̒��Ԃ����̓X���[������
				if (playData.layer[i].note[j].type[k] != 3) {
					if (playData.layer[i].note[j].judgement[k] == 1) {
						combo++;
						perfect++;
						//MAX�R���{�m�F
						if (maxCombo < combo) {
							maxCombo = combo;
						}
						break;
					}
					else if (playData.layer[i].note[j].judgement[k] == 2) {
						combo++;
						great++;
						//MAX�R���{�m�F
						if (maxCombo < combo) {
							maxCombo = combo;
						}
						break;
					}
					else if (playData.layer[i].note[j].judgement[k] == 3) {
						//MISS������R���{�J�E���g��0�ɂ���
						combo = 0;
						miss++;
						break;
					}
				}
			}
		}
	}
}

void Lane::ColumnHit(int layer, int columnNum, int notes, bool trigger, bool push) {
	//�Z����
	if (!autoPlay) {
		//���̈Ⴄ�m�[�c�����d��hit������o���Ȃ��悤��hit��false����Ȃ��Ə�������Ȃ��悤�ɕύX
		if (!playData.layer[layer].note[columnNum].hit[notes]) {
			if (trigger) {
				//TAP�n��̂݁iHOLD�n�_��TAP����
				if (playData.layer[layer].note[columnNum].hitTimer[notes] <= fastJudge + 5) {
					if (playData.layer[layer].note[columnNum].type[notes] < 9) {
						playData.layer[layer].note[columnNum].hit[notes] = true;
					}
				}
			}
			//������
			if (push) {
				//��������Ԃ�HOLD��FAST������Ȃ���
				if (playData.layer[layer].note[columnNum].hitTimer[notes] <= 0) {
					//HOLD�n��
					if (playData.layer[layer].note[columnNum].type[notes] > 8) {
						playData.layer[layer].note[columnNum].hit[notes] = true;
					}
				}
			}
		}
	}
	else if (autoPlay) {
		if (playData.layer[layer].note[columnNum].hitTimer[notes] == 0) {
			playData.layer[layer].note[columnNum].hit[notes] = true;
		}
	}
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
			if (playData.layer[i].note[j].chart[chartNum] > 0 && playData.layer[i].note[j].chart[chartNum] < 17) {	//�m�[�c�̗L���`�F�b�N
				for (int k = 0; k < drawNotes; k++) {	//�󂢂Ă鏇����t���O���I���ɂ���
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						break;											//�󂫂����������瑦�E�o
					}
				}
			}
			//else if (playData.layer[i].note[j].chart[chartNum] == 2 || playData.layer[i].note[j].chart[chartNum] == 5 || playData.layer[i].note[j].chart[chartNum] == 6) {	//�m�[�c�̗L���`�F�b�N
			//	for (int k = 0; k < drawNotes; k++) {	//�󂢂Ă鏇����t���O���I���ɂ���
			//		if (!playData.layer[i].note[j].startMove[k]) {
			//			SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
			//			break;											//�󂫂����������瑦�E�o
			//		}
			//	}
			//}
			else if (playData.layer[i].note[j].chart[chartNum] > 16) {	//�m�[�c�̗L���`�F�b�N
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
						playData.layer[i].note[j].hitTimer[k] = 60;
						playData.layer[i].note[j].type[k] = 0;
						playData.layer[i].note[j].startMove[k] = false;
						playData.layer[i].note[j].judgement[k] = 0;
					}
					if (!playData.layer[i].note[j].hit[k]) {
						playData.layer[i].note[j].hitTimer[k]--;	//�^�C�}�[�����炷
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
	ID000("Resources/musicData/000/testmc.txt", 0);
	IDEntry(1, "Resources/musicData/001/banbado.txt", "musicData/001/banbado.wav",144);
}

void Lane::ID000(string filePass, int musicID) {
	//test�p�f�[�^
	//����
	musicData[musicID].beatDenomonator = 4;
	//���q
	musicData[musicID].beatMolecule = 4;
	//BPM
	musicData[musicID].BPM = 120;
	//���ʑ��x�i�{���j
	musicData[musicID].speed = 1;

	LoadData(musicID, filePass);
}

void Lane::IDEntry(int musicID, std::string filePass, std::string musicPass,int BPM, int beatDenomonator, int beatMolecule, int speed) {
	//����
	musicData[musicID].beatDenomonator = beatDenomonator;
	//���q
	musicData[musicID].beatMolecule = beatMolecule;
	//BPM
	musicData[musicID].BPM = BPM;
	//���ʑ��x�i�{���j
	musicData[musicID].speed = speed;

	LoadData(musicID, filePass);

	//music[musicID] = audioMusic->LoadWave(musicPass);
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

void Lane::SetNote(int i, int j, int k, int typeNum) {
	playData.layer[i].note[j].startMove[k] = true;	//�J�n�t���O�I��
	playData.layer[i].note[j].type[k] = typeNum;			//�m�[�c�̎�ސݒ�
	//playData.layer[i].note[j].hitTimer[k] = 60;		//����J�E���g�̐ݒ�
}

bool Lane::ThickColumn(bool key1, bool key2, bool key3, bool key4) {
	if (key1 || key2 || key3 || key4) {
		return true;
	}
	return false;
}