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
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(int &sceneNum,int &musicID,int &difficulty);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//�ȑI�𒆂̃f�[�^�\��
	void SelectDrawData(int maxScore,int maxRank,int isFCAP, bool clear, int difficulty, int level);

	//��Փx�E���x���\���i�v���C�����\�����邽�ߕʌŗp�Ӂj
	void DrawDifficulty(int difficulty, int level);

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
	uint32_t textureHandleBG_[4] = { 0,0,0,0 };
	Sprite* spriteBG_[4] = {nullptr,nullptr,nullptr,nullptr};
	//�^�C�g��
	uint32_t texturehandlTi_ = 0;
	Sprite* spriteTi_ = nullptr;

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

	uint32_t controlTx = 0;
	Sprite* controlSp = nullptr;

	Vector2 position;
	Vector2 size;
	//�Ȃ̏㉺�ړ�
	int scene = 0;
	bool JumpFlagPlus_ = TRUE;
	bool JumpFlagNegative_ = FALSE;
	int JumpTimer_ = 60;
	int JumpTimer2_ = 60;
	int JumpTimer3_ = 60;
	int JumpTimer4_ = 60;
	int JumpTimer5_ = 60;

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


	//�Ȃ̏��Ńt���O
	/*int  Songdelete_ = 0;
	int Songdelete2_ = 0;
	int Songdelete3_ = 0;
	int Songdelete4_ = 0;
	int Songdelete5_ = 0;*/

};

