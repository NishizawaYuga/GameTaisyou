#pragma once

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

//�������œn���̂��߂�ǂ������̂ł܂Ƃ߂�
struct OptionsData {
	int speed;
	int style;
	bool autoPlay;
	int wall;
	bool detail;
};

/// <summary>
/// �Q�[���V�[��
/// </summary>
class Select {

public: // �����o�֐�
  /// <summary>
  /// �R���X�g�N���^
  /// </summary>
	Select();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Select();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(uint32_t SE[],uint32_t BGM[]);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(int &sceneNum,int &musicID,int &difficulty,OptionsData& options);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//�ȑI�𒆂̃f�[�^�\��
	void SelectDrawData(int maxScore,int maxRank,int isFCAP, bool clear);

	//��Փx�E���x���\���i�v���C�����\�����邽�ߕʌŗp�Ӂj
	void DrawDifficulty(int difficulty, int level);

	//�Ȗ��E�A�[�e�B�X�g�END�\���i���U���g��ʂɕʂŕ\�������邽�߁j
	void DrawMusicString(int difficulty, int ID);

private: // �����o�ϐ�
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransform_;

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>

	//�g
	uint32_t textureHandleFrame_ = 0;
	Sprite* spriteframe_ = nullptr;
	//1
	uint32_t textureHandleSong_ = 0;
	Sprite* spritesong_[5];
	//2
	uint32_t textureHandleSong2_ = 0;
	//3
	uint32_t textureHandleSong3_ = 0;
	//4
	uint32_t textureHandleSong4_ = 0;
	//5
	uint32_t textureHandleSong5_ = 0;
	//6
	uint32_t textureHandleSong6_ = 0;
	//7
	uint32_t textureHandleSong7_ = 0;
	//BG
	uint32_t textureHandleBG_[5] = { 0,0,0,0,0 };
	Sprite* spriteBG_[5] = {nullptr,nullptr,nullptr,nullptr,nullptr};
	//�^�C�g��
	uint32_t texturehandlTi_ = 0;
	Sprite* spriteTi_ = nullptr;
	uint32_t logoTex = 0;
	Sprite* logoSp = nullptr;

	//����
	uint32_t textureNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* spriteNum[9][11] = { nullptr };
	//��O�̂�0���ǂ���
	bool isZero[7] = { 0,0,0,0,0,0,0 };

	//�����N
	uint32_t rankTexNum[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* rankSprite[10] = { nullptr };

	//CLFCAP�`�b�v
	uint32_t tip[3] = { 0,0,0 };
	Sprite* tipSprite[3] = { nullptr };

	//��Փx�\�L
	uint32_t diffNum[4] = { 0,0,0,0 };
	Sprite* diff[4] = { nullptr };

	//���x��
	uint32_t levelNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* levelS[11] = { nullptr };

	//�Ȗ��Ȃ�
	uint32_t musicName[6] = {0,0,0,0,0};
	Sprite* musicNameSprite[6] = { nullptr };
	uint32_t artist[6] = { 0,0,0,0,0 };
	Sprite* artistSprite[6] = { nullptr };
	uint32_t nd[6][4] = { 0,0,0,0,0 };
	Sprite* ndSprite[6][4] = { nullptr };
	//�m�[�c�f�U�C�i�[
	uint32_t noteDesigner = 0;
	Sprite* noteDesignerSprite = nullptr;
	//ID
	int id = 0;

	//�^�u�ԍ�
	int tab;
	//�^�u
	uint32_t tabTex[2] = { 0,0 };
	Sprite* tabSp[2] = { nullptr,nullptr };
	float tabPosX;
	float tabSizeX = 256.0f;
	float tabSizeY = 96.0f;
	bool sizeChange = false;
	int nowTime = 0;
	int maxTime = 60;

	//�I�v�V����
	uint32_t options[5][11] = {0,0,0,0,0};
	Sprite* optionsSprite[5][11] = {nullptr,nullptr, nullptr, nullptr, nullptr};
	//�I�v�V�����e��
	int speed;	//���ʑ��x
	int style;		//�L�[�z�u�ύX
	int autoPlay;	//�I�[�g
	bool detail;	//�ڍו\��
	int wall;		//�E�H�[��
	int optionNum;	//�ԍ�

	//����
	uint32_t explanationTx[5] = { 0,0,0,0,0 };
	Sprite* explanationSp[5] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr };
	uint32_t figureTex = 0;
	Sprite* figureSp = nullptr;
	uint32_t figImage[7] = { 0,0,0,0,0,0,0 };
	Sprite* figImageSp[7] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
	uint32_t figNote = 0;
	Sprite* figNoteSp = nullptr;

	//��ʉ����i�r
	uint32_t controlTx = 0;
	Sprite* controlSp = nullptr;

	Vector2 position;
	Vector2 size;
	//�Ȃ̏㉺�ړ�
	int scene = 0;
	bool JumpFlagPlus_ = TRUE;
	bool JumpFlagNegative_ = FALSE;
	int jumpTimer[5] = { 60,60,60,60,60 };

	//�w�i�ύX�p�ϐ�
	int difficultyColor;

	//�V�[���؂�ւ��p���\�[�X
	//1
	uint32_t textureHandleSongPl1_ = 0;
	Sprite* spriteSongPl1_ = nullptr;
	//2
	uint32_t textureHandleSongPl2_ = 0;
	Sprite* spriteSongPl2_ = nullptr;
	//3
	uint32_t textureHandleSongPl3_ = 0;
	Sprite* spriteSongPl3_ = nullptr;
	//4
	uint32_t textureHandleSongPl4_ = 0;
	Sprite* spriteSongPl4_ = nullptr;
	//5
	uint32_t textureHandleSongPl5_ = 0;
	Sprite* spriteSongPl5_ = nullptr;

	//���ʉ�
	uint32_t SEselect[7] = { 0,0,0,0,0,0,0 };
	Audio* soundSE = nullptr;

	//���y
	uint32_t BGM[3] = { 0,0,0 };
	Audio* bgm[3] = { nullptr,nullptr,nullptr };
	bool playFlag[3] = { false,false,false };

	//�i�r
	uint32_t nabiTitleTex[2] = { 0,0 };
	Sprite* nabiTitleSp[2] = { nullptr,nullptr };
	uint32_t nabi_selectTex[8] = { 0,0,0,0,0,0,0,0 };
	Sprite* nabi_selectSp[8]= {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

	//�Ȃ̏��Ńt���O
	/*int  Songdelete_ = 0;
	int Songdelete2_ = 0;
	int Songdelete3_ = 0;
	int Songdelete4_ = 0;
	int Songdelete5_ = 0;*/

};

