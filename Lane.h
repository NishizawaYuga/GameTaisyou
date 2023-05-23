#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Audio.h"
#include "Sprite.h"

#include <string>

//chartManager�œǂݍ��񂾕��ʂ�Lane�Ɉڂ����Ƃ���Ɨe�ʂ��傫�����ăG���[�f���̂�
//��̃\�[�X�ɕ����Ă�����������ɓZ�߂܂�

//�m�[�c�i1��)
struct Note {
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform[200];
	//�ړ����x
	float speed;
	//�q�b�g����
	bool hit[200] = { false };
	//����`�F�b�N
	int judgement[200] = { 0 };
	//����^�C�}�[
	int hitTimer[200] = { 0 };
	//�m�[�c�̎��
	int type[200] = { 0 };
	//�J�n����
	bool startMove[200] = { false };

	//����
	int chart[2500] = { 0 };
};

//�m�[�c�i4��)
struct Notes {
	Note note[4];
};

//��Փx�i����j
struct Difficulty {
	//���ʁi���C���[4���j
	//�i����񕪂̒�����Z�����邽�ߏc������4���L�΂�)
	Notes layer[32];
	//�ō��X�R�A
	int maxScore = 0;
	//�ō������N
	int maxRankNum = 0;
	std::string maxRank = "D";
	//FC�AAP�̏��
	int isFCAP = 0;
	//�N���A�t���O
	bool clear = false;
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
	//�e��Փx���Ƃ̕���
	Difficulty difficulty[4];
	//��Փx�̃��x���i��Փx�ʁj
	int level[4];
	//�Ȃ̊J�n�^�C�~���O
	int playMusicCount;
	//�X�R�A�f�[�^�̃t�@�C���p�X
	std::string dataPass = "a";
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
	void Update(int& scene);
	//�`��
	void Draw(ViewProjection viewProjection);
	//�`��i�X�v���C�g�j
	void DrawSprite();
	//�ǂݍ���
	void LoadMusic(int ID, int difficulty);
	//���Z�b�g�i�ȏI����Ȃǂɋ���ۂɂ���j
	void ResetMusic();
	//���[���̌����ڕύX
	void ChangeLane(Model* model);
	//�f�t�H���g�ɖ߂�
	void DefaultLane();
	//�I�[�g��
	//true�ŃI�[�gON�Afalse�ŃI�[�gOFF
	void Auto(bool select);
	//���ʑ��x�ύX
	void ChangeSpeed(float addSpeed);
	//�X�^�C���ύX
	void ChangeStyle(int num);
	//�E�H�[���ݒ�ύX
	void ChangeWall(float num);
	//MusicData�擾
	MusicData GetMusic(int ID) { return musicData[ID]; }

	//�f�[�^�擾
	//�n�C�X�R�A�f�[�^�擾
	int GetHiScore(int ID, int difficulty) { return musicData[ID].difficulty[difficulty].maxScore; }
	//�ō������N�f�[�^�擾
	int GetHiRank(int ID, int difficulty) { return musicData[ID].difficulty[difficulty].maxRankNum; }
	//FCAP�`�F�b�N
	int GetFCAPFlag(int ID, int difficulty) { return musicData[ID].difficulty[difficulty].isFCAP; }
	//Clear�t���O�擾
	bool GetClear(int ID, int difficulty) { return musicData[ID].difficulty[difficulty].clear; }
	//���׃��擾
	int GetLevel(int ID, int difficulty) { return musicData[ID].level[difficulty]; }

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
	bool ThickColumn(bool key1 = false, bool key2 = false, bool key3 = false, bool key4 = false);
	//���[�g�v�Z
	void UpdateRate(int RateScore);
	//����
	void ChartInitialize();
	//���ߐ��X�V
	void LineUpdate();
	//�t�@�C���ǂݍ���
	void LoadData(int ID, int difficulty, std::string filePass);
	//�ȃf�[�^�������Q
	void ID000(std::string filePass, int musicID);	//�e�X�g����
	//ID�ƕ��ʃf�[�^�̃p�X�ƋȂ̃p�X�ƃX�R�A�f�[�^�̃p�X�ABPM�Ɠ�Փx�����w�肷���OK�A�f�t�H���g�Ń��x��0�̋ȊJ�n0�b��4/4���q�̕��ʑ��x�{��1
	void IDEntry(int musicID, std::string filePass, const std::string musicPass, const std::string scoreDataPass, int BPM, int difficultyNum, int level = 0,
		int startMusicCount = 0, int beatDenomonator = 4, int beatMolecule = 4, int speed = 1);	//�ėpID�o�^�֐�
	//�Ȃ̏I���֐�
	void FinishMusic(int& scene);
	//4�P�^�pDrawSprite
	void DrawSprite4(int num, Sprite* sprite[25][10], int startNum, bool draw0);
	//�]��Draw
	void SetEvaluation(int spriteNum);
	//�L�[���f�����W�ύX
	void KeyPositionChange(int style);

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

	//�E�H�[���i�ǁj
	WorldTransform wallPosition;
	Model* wall = nullptr;
	//���l�ɉ����Ă̔{��
	float wallNum;

	//�I�[�g�v���C�t���O
	bool autoPlay;
	//�ȊJ�n�܂ł̎���
	//�����l
	const int resetStartTimer = 300;
	//���ۂɕϓ�������l
	int startTimer;

	//�ȏI���܂ł̎���
	const int resetEndTimer = 300;
	//���ۂɕϓ�������l
	int endTimer;

	//����
	//���y��
	const int musicNum = 10;
	//���C���[��
	const int layerNum = 4;
	//��
	const int columnNum = 4;
	//�u����m�[�c��
	const int maxNotes = 2500;
	//�S�̗̂�ύX�̂��炵���l
	int shift = 0;
	//�S�̂̍s��
	const int shiftMaxNum = 8;
	//�\������m�[�c
	const int drawNotes = 200;
	//����Ɏg�p����^�C�}�[
	const int startMoveTimer = 120;
	//���ʑ��x�i�{���́j
	float speed;
	//�T�C�Y�{���i���ʑ��x�ɉ����ĕύX�j
	float size;
	//�f�t�H���g�q�b�g�^�C�}�[
	int defaultHitTimer;
	//�ȊJ�n
	bool startMusic;
	//�m�[�c�J�E���^�[
	int notesCounter = 0;
	//���ʂ̓ǂݎ��ԍ�
	int chartNum;
	//���胉�C������X�^�[�g�n�_�܂ł̋���
	float distance;

	//��ID
	int musicID = 0;
	//��Փx�ԍ��@0 : Easy, 1 : Normal, 2 : Master, 3 : Prank
	int difficulty = 0;

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

	//�m���}
	//�N���A���C��
	float quota;
	//���U���g��ʗp�̃N���A�t���O
	bool isClear = false;

	//�X�R�A
	int score;
	//�����N���߂Ƃ��Ɏg�������ȕ��σ��[�g
	float averageRate;
	//�������o�����߂̃��[�g
	float rate;
	//���ς�����o�����߂̃J�E���^�[
	int accuracyCounter;
	//�����N
	std::string rank;
	int rankNum = 0;

	//���蒲���p
	int fastJudge;
	int lateJudge;

	//�f�o�b�O�p
	bool moveFlag = true;

	//�T�E���h�f�[�^�n���h��
	uint32_t music[10] = { 0 };
	uint32_t SE[3] = { 0,0,0 };

	//�v���C��ʗpUI
	//�R���{�\��
	uint32_t uiCombo = 0;
	Sprite* comboSprite = nullptr;
	//�R���{���\��
	uint32_t uiComboNum[10] = {0,0,0,0,0,0,0,0,0,0};
	Sprite* comboNumSprite[4][10] = {nullptr,nullptr, nullptr, nullptr};
	//�ڍ׃J�E���g�\��
	uint32_t uiData[5] = {0,0,0,0,0};
	Sprite* uiDataSprite[5] = { nullptr,nullptr, nullptr, nullptr,nullptr };
	//�ڍ׃J�E���g���\���iMAXCOMBO,PERFECT�AGREAT�AMISS�ł��ꂼ��4�P�^�A�X�R�A��,�܂߂�9�P�^�A4 �~ 4 + 9 = 25�j
	uint32_t uiDataNum[10] = { 0,0,0,0,0,0,0,0,0,0};
	Sprite* uiDataNumbers[16][10] = {nullptr};
	uint32_t uiScoreNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* uiScore[9][11] = { nullptr };
	//COMBO���\���̋��ʍ��W
	float comboNumPosX = 1610.0f;
	float comboNumPosY = 380.0f;

	//�]��
	//�e�N�X�`��
	uint32_t evaluationTex[3] = { 0,0,0 };
	//�X�v���C�g
	Sprite* evaluationSprite[3] = { nullptr,nullptr,nullptr };
	//�T�C�Y
	float sizeX = 160.0f;
	float sizeY = 32.0f;
	//���W
	float evaPosX = 960.0f;
	float evaPosY = 400.0f;
	//�\������
	int drawTimer = 0;
	//�X�v���C�g�ԍ�
	int evaluationSpriteNum = 0;

	//�X�^�C���ԍ�
	int style = 0;

	//�L�[���f��
	WorldTransform keyModelPos[3][4];
	Model* keyModel[3][4] = { nullptr };
	//���
	WorldTransform pedestalPos;
	Model* pedestal = nullptr;
};