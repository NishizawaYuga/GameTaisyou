#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h" 

//������GameScene�̃��c�S�������Ă����A�K�v�Ȃ�include�͌�ɍ폜

//�m�[�c�i1��)
struct Note {
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform[20000];
	//�ړ����x
	float speed;
	//�q�b�g����
	bool hit[20000] = {false};
	//����`�F�b�N
	int judgement[20000] = {0};
	//����^�C�}�[
	int hitTimer[2000] = {0};
	//�m�[�c�̎��
	int type[20000] = {0};
	//�J�n����
	bool startMove[20000] = { false };

	//����
	int chart[20000] = {0};
};

//�m�[�c�i4��)
struct Notes {
	Note note[4];
};

//�Ȃ̊�{���
struct MusicData {
	//BPM
	int BPM;
	//����
	//����
	int beatDenomonator;
	//���q
	int beatMolecule;
	//���ʑ��x
	int speed;
	//���ʁi���C���[4���j
	Notes layer[4];
};

//���ߐ�
struct Line {
	//�t���[���J�E���g
	int countFlame;
	//���J�E���g
	int countRhythm;
	//�x�[�X�ƂȂ�BPM(BPM60 = 1����1�b)
	const float baseBPM = 60.0f;
	//���[�v�^�C�~���O�𒲐�������{��
	float magnification;
	//�J�E���g�{��
	float change;
	//1�����J�E���g����^�C�~���O
	int switching;
	//���ߐ��^�C�~���O
	int linePop[500];
	//���ߐ��ԍ�
	int lineNum;
	//���ߐ�
	WorldTransform lineWorld[500];
};

class Lane {
public:
	//������
	void Initialize(Model* laneModel, Model*lineModel);
	//�X�V
	void Update();
	//�`��
	void Draw(ViewProjection viewProjection);
	//�ǂݍ���
	void LoadMusic(MusicData musicData_);
	//���Z�b�g�i�ȏI����Ȃǂɋ���ۂɂ���j
	void ResetMusic();
	//���[���̌����ڕύX
	void ChangeLane(Model* model);
	//�f�t�H���g�ɖ߂�
	void DefaultLane();
	//�I�[�g��
	//true�ŃI�[�gON�Afalse�ŃI�[�gOFF
	void Auto(bool select);

private:
	//���ʂ�ǂ�
	void ReadChart(int notes,int i,int j);

private:
	//���y�f�[�^
	MusicData musicData;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//���[�����̂��̂̃��[���h���W
	WorldTransform lanePosition;
	//���[�����̂��̂̃��f���f�[�^
	Model* laneModel = nullptr;
	//�f�t�H���g�p�ɕʌŎ���Ă���
	Model* defaultModel = nullptr;
	//���ߐ����f��
	Model* lineModel = nullptr;
	//���ߐ�
	Line line;
	//���[�v������J�E���g
	const int maxNotes = 20000;
	//�I�[�g�v���C�t���O
	bool autoPlay;
	//�ȊJ�n�܂ł̎���
	//�����l
	const int resetStartTimer = 300;
	//���ۂɕϓ�������l
	int startTimer;
};