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

#include "Lane.h"
#include "Select.h"
#include "skydome.h"
#include "Result.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	ViewProjection viewProjection;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//シーン管理番号
	int sceneNum;
	//レーン
	Lane* lane = nullptr;
	//曲選択画面
	Select* select = nullptr;
	//スカイドーム
	Skydome* skydome = nullptr;

	//曲IDと難易度
	int difficulty;
	int musicID;

	//直前の選択してたIDと難易度を保存する変数
	int oldDiff;
	int oldID;

	//レーン用モデル
	Model* laneModel = nullptr;
	Model* lineModel = nullptr;
	Model* notesModel[12];
	//スカイドームモデル
	Model* skydomeModel = nullptr;

	//Options
	OptionsData options;
	//更新確認用直前の入力データ保管オプション構造体
	OptionsData oldOptions;

	//リザルト
	Result* results = nullptr;

	//効果音とBGM
	uint32_t SE[10] = { 0,0,0,0,0,0,0,0,0,0 };
	uint32_t BGM[5] = { 0,0,0,0,0 };
	uint32_t music[5] = { 0,0,0,0,0 };
};
