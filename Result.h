#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<DirectXMath.h>

#include "ResultData.h"

class Result {
public:
	//������
	void Initialize();
	//�X�V
	void Update(int &scene);
	//�`��
	void Draw();
	void Draw3D();
	//���U���g�f�[�^�󂯎��
	void IncludeResultData(ResultData resultdata);
	//���U���g�f�[�^����
	void DeleteResult();
private:
	//4�P�^�f�[�^�\���i���g��Lane�̂ƑS�������j
	void DrawSprite4(int num, Sprite* sprite[16][10], int startNum, bool draw0);
	//�e�X�g�p�f�[�^�i�[
	void TestResult();

private:
	//�L�[����
	Input* input_ = nullptr;
	//�󂯎��p���U���g�f�[�^
	ResultData resultData;
	//���U���g�p���\�[�X
	//�X�R�A
	uint32_t resultScoreTex[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* resultScoreSp[9][11] = { nullptr };
	//��O�̂�0���ǂ���
	bool isZeroResult[7] = { 0,0,0,0,0,0,0 };
	//�n�C�X�R�A
	uint32_t resultHiScoreTex[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* resultHiScoreSp[9][11] = { nullptr };
	//��O�̂�0���ǂ���
	bool isZeroHiScoreResult[7] = { 0,0,0,0,0,0,0 };
	//�X�V����
	bool checkUpdate = false;
	uint32_t newlavel = 0;
	Sprite* newlavelSp = nullptr;
	//�����N
	uint32_t resultRankTex[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* resultRankSp[10] = { nullptr };
	//FCAP&�N���A�i�v���[�g�j
	uint32_t plateTex[4] = { 0,0,0,0 };
	Sprite* plateSp[4] = { nullptr,nullptr,nullptr,nullptr };
	//�R���{�\��
	uint32_t uiCombo = 0;
	Sprite* comboSprite = nullptr;
	//�R���{���\��
	uint32_t uiComboNum[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* comboNumSprite[4][10] = { nullptr,nullptr, nullptr, nullptr };
	//�ڍ׃J�E���g�\��
	uint32_t uiData[6] = { 0,0,0,0,0,0 };
	Sprite* uiDataSprite[6] = { nullptr,nullptr, nullptr, nullptr,nullptr,nullptr };
	//�ڍ׃J�E���g���\���iMAXCOMBO,PERFECT�AGREAT�AMISS�ł��ꂼ��4�P�^�A�X�R�A��,�܂߂�9�P�^�A4 �~ 4 + 9 = 25�j
	uint32_t uiDataNum[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* uiDataNumbers[16][10] = { nullptr };
	uint32_t HiScoreNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* HiScore[9][11] = { nullptr };
	//COMBO���\���̋��ʍ��W
	float comboNumPosX = 1610.0f;
	float comboNumPosY = 380.0f;

	//�w�i
	uint32_t resultBackGroundTex[3] = { 0,0,0 };
	Sprite* resultBackGroundSp[3] = { nullptr,nullptr,nullptr };

	//����s�\�^�C�}�[
	int stopTimer = 180;
};
