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

//一個一個引数で渡すのがめんどくさいのでまとめる
struct OptionsData {
	int speed;
	int style;
	bool autoPlay;
	int wall;
	bool detail;
};

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
	void Initialize(uint32_t SE[],uint32_t BGM[]);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(int &sceneNum,int &musicID,int &difficulty,OptionsData& options);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//曲選択中のデータ表示
	void SelectDrawData(int maxScore,int maxRank,int isFCAP, bool clear);

	//難易度・レベル表示（プレイ中も表示するため別個で用意）
	void DrawDifficulty(int difficulty, int level);

	//曲名・アーティスト・ND表示（リザルト画面に別で表示させるため）
	void DrawMusicString(int difficulty, int ID);

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
	uint32_t textureHandleBG_[5] = { 0,0,0,0,0 };
	Sprite* spriteBG_[5] = {nullptr,nullptr,nullptr,nullptr,nullptr};
	//タイトル
	uint32_t texturehandlTi_ = 0;
	Sprite* spriteTi_ = nullptr;
	uint32_t logoTex = 0;
	Sprite* logoSp = nullptr;

	//数字
	uint32_t textureNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* spriteNum[9][11] = { nullptr };
	//手前のが0かどうか
	bool isZero[7] = { 0,0,0,0,0,0,0 };

	//ランク
	uint32_t rankTexNum[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* rankSprite[10] = { nullptr };

	//CLFCAPチップ
	uint32_t tip[3] = { 0,0,0 };
	Sprite* tipSprite[3] = { nullptr };

	//難易度表記
	uint32_t diffNum[4] = { 0,0,0,0 };
	Sprite* diff[4] = { nullptr };

	//レベル
	uint32_t levelNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* levelS[11] = { nullptr };

	//曲名など
	uint32_t musicName[6] = {0,0,0,0,0};
	Sprite* musicNameSprite[6] = { nullptr };
	uint32_t artist[6] = { 0,0,0,0,0 };
	Sprite* artistSprite[6] = { nullptr };
	uint32_t nd[6][4] = { 0,0,0,0,0 };
	Sprite* ndSprite[6][4] = { nullptr };
	//ノーツデザイナー
	uint32_t noteDesigner = 0;
	Sprite* noteDesignerSprite = nullptr;
	//ID
	int id = 0;

	//タブ番号
	int tab;
	//タブ
	uint32_t tabTex[2] = { 0,0 };
	Sprite* tabSp[2] = { nullptr,nullptr };
	float tabPosX;
	float tabSizeX = 256.0f;
	float tabSizeY = 96.0f;
	bool sizeChange = false;
	int nowTime = 0;
	int maxTime = 60;

	//オプション
	uint32_t options[5][11] = {0,0,0,0,0};
	Sprite* optionsSprite[5][11] = {nullptr,nullptr, nullptr, nullptr, nullptr};
	//オプション各種
	int speed;	//譜面速度
	int style;		//キー配置変更
	int autoPlay;	//オート
	bool detail;	//詳細表示
	int wall;		//ウォール
	int optionNum;	//番号

	//説明
	uint32_t explanationTx[5] = { 0,0,0,0,0 };
	Sprite* explanationSp[5] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr };
	uint32_t figureTex = 0;
	Sprite* figureSp = nullptr;
	uint32_t figImage[7] = { 0,0,0,0,0,0,0 };
	Sprite* figImageSp[7] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
	uint32_t figNote = 0;
	Sprite* figNoteSp = nullptr;

	//画面下部ナビ
	uint32_t controlTx = 0;
	Sprite* controlSp = nullptr;

	Vector2 position;
	Vector2 size;
	//曲の上下移動
	int scene = 0;
	bool JumpFlagPlus_ = TRUE;
	bool JumpFlagNegative_ = FALSE;
	int jumpTimer[5] = { 60,60,60,60,60 };

	//背景変更用変数
	int difficultyColor;

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

	//効果音
	uint32_t SEselect[7] = { 0,0,0,0,0,0,0 };
	Audio* soundSE = nullptr;

	//音楽
	uint32_t BGM[3] = { 0,0,0 };
	Audio* bgm[3] = { nullptr,nullptr,nullptr };
	bool playFlag[3] = { false,false,false };

	//ナビ
	uint32_t nabiTitleTex[2] = { 0,0 };
	Sprite* nabiTitleSp[2] = { nullptr,nullptr };
	uint32_t nabi_selectTex[8] = { 0,0,0,0,0,0,0,0 };
	Sprite* nabi_selectSp[8]= {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};

	//曲の消滅フラグ
	/*int  Songdelete_ = 0;
	int Songdelete2_ = 0;
	int Songdelete3_ = 0;
	int Songdelete4_ = 0;
	int Songdelete5_ = 0;*/

};

