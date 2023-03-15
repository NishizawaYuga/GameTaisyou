#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Audio.h"

#include <string>

//chartManager�œǂݍ��񂾕��ʂ�Lane�Ɉڂ����Ƃ���Ɨe�ʂ��傫�����ăG���[�f���̂�
//��̃\�[�X�ɕ����Ă�����������ɓZ�߂܂�

//�m�[�c�i1��)
struct Note {
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform[90];
	//�ړ����x
	float speed;
	//�q�b�g����
	bool hit[90] = { false };
	//����`�F�b�N
	int judgement[90] = { 0 };
	//����^�C�}�[
	int hitTimer[90] = { 0 };
	//�m�[�c�̎��
	int type[90] = { 0 };
	//�J�n����
	bool startMove[90] = { false };

	//����
	int chart[15000] = { 0 };
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
	const int lineNum = 500;
	//���ߐ�
	WorldTransform lineWorld[500];
};

class Lane {
public:
	//������
	void Initialize(Model* laneModel, Model* lineModel, Model* noteModel[12]);
	//�X�V
	void Update();
	//�`��
	void Draw(ViewProjection viewProjection);
	//�ǂݍ���
	void LoadMusic(int ID);
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
	void ReadChart();
	//�X�^�[�g���̐ݒ�
	void SetNote(int i, int j, int k, int typeNum);
	//����
	void Judgement();
	//�����̔���
	void ColumnHit(int layer, int columnNum, int notes, bool trigger, bool push);
	//�����m�[�c�̉����ꂽ�����Ԃ�
	bool ThickColumn(bool key1=false, bool key2=false, bool key3=false, bool key4=false);
	//����
	void ChartInitialize();
	//���ߐ��X�V
	void LineUpdate();
	//�t�@�C���ǂݍ���
	void LoadData(int ID, std::string filePass);
	//�ȃf�[�^�������Q
	void ID000(std::string filePass, int musicID);	//�e�X�g����
	//ID�ƕ��ʃf�[�^�̃p�X�ƋȂ̃p�X�ABPM�����w�肷���OK�A�f�t�H���g��4/4���q�̕��ʑ��x�{��1
	void IDEntry(int musicID, std::string filePass, const std::string musicPass, int BPM,int beatDenomonator = 4,int beatMolecule = 4,int speed = 1);	//�ėpID�o�^�֐�

private:
	//���y�f�[�^
	//�v���C�Ŏ��ۂɎg���f�[�^
	MusicData playData;
	//���Ŋi�[���Ă����f�[�^
	MusicData musicData[10];
	//�I�[�f�B�I
	Audio* audioSE = nullptr;
	Audio* audioMusic = nullptr;

	//�L�[����
	Input* input_ = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText_ = nullptr;

	//���[�����̂��̂̃��[���h���W
	WorldTransform lanePosition;
	//���[�����̂��̂̃��f���f�[�^
	Model* laneModel = nullptr;
	//�f�t�H���g�p�ɕʌŎ���Ă���
	Model* defaultModel = nullptr;
	//���ߐ����f��
	Model* lineModel = nullptr;
	//�m�[�c���f��
	Model* notesModel[12];
	//�m�[�c���f����
	const int modelNum = 12;
	//���ߐ�
	Line line;

	//�I�[�g�v���C�t���O
	bool autoPlay;
	//�ȊJ�n�܂ł̎���
	//�����l
	const int resetStartTimer = 300;
	//���ۂɕϓ�������l
	int startTimer;

	//����
	//���y��
	const int musicNum = 10;
	//���C���[��
	const int layerNum = 4;
	//��
	const int columnNum = 4;
	//�u����m�[�c��
	const int maxNotes = 15000;
	//�\������m�[�c
	const int drawNotes = 90;
	//����Ɏg�p����^�C�}�[
	const int startMoveTimer = 120;
	//���ʑ��x�i�{���́j
	float speed;
	//�ȊJ�n
	bool startMusic;
	//���ʂ̓ǂݎ��ԍ�
	int chartNum;
	//���胉�C������X�^�[�g�n�_�܂ł̋���
	float distance;

	//�R���{��
	int combo;
	//�ő�R���{��
	int maxCombo;
	//PERFECT��
	int perfect;
	//GREAT��
	int great;
	//MISS��
	int miss;

	//���蒲���p
	int fastJudge;
	int lateJudge;

	//�f�o�b�O�p
	bool moveFlag = true;

	//�T�E���h�f�[�^�n���h��
	uint32_t music[10] = { 0 };
	uint32_t SE[3] = { 0,0,0 };
};