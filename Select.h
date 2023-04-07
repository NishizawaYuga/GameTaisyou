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
/// ゲームシーン
/// </summary>
class Select {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	Select();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Select();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	WorldTransform worldTransform_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//枠
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
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;
	//タイトル
	uint32_t texturehandlTi_ = 0;
	Sprite* spriteTi_ = nullptr;

	Vector2 position;
	Vector2 size;
	//曲の上下移動
	int scene = 0;
	bool JumpFlagPlus_ = TRUE;
	bool JumpFlagNegative_ = FALSE;
	int JumpTimer_ = 60;
	int JumpTimer2_ = 60;
	int JumpTimer3_ = 60;
	int JumpTimer4_ = 60;
	int JumpTimer5_ = 60;


	//シーン切り替え用リソース
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


	//曲の消滅フラグ
	/*int  Songdelete_ = 0;
	int Songdelete2_ = 0;
	int Songdelete3_ = 0;
	int Songdelete4_ = 0;
	int Songdelete5_ = 0;*/

};

