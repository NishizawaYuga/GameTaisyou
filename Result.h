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
	//初期化
	void Initialize();
	//更新
	void Update(int &scene);
	//描画
	void Draw();
	void Draw3D();
	//リザルトデータ受け取り
	void IncludeResultData(ResultData resultdata);
	//リザルトデータ消去
	void DeleteResult();
private:
	//4ケタデータ表示（中身はLaneのと全く同じ）
	void DrawSprite4(int num, Sprite* sprite[16][10], int startNum, bool draw0);
	//テスト用データ格納
	void TestResult();

private:
	//キー入力
	Input* input_ = nullptr;
	//受け取り用リザルトデータ
	ResultData resultData;
	//リザルト用リソース
	//スコア
	uint32_t resultScoreTex[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* resultScoreSp[9][11] = { nullptr };
	//手前のが0かどうか
	bool isZeroResult[7] = { 0,0,0,0,0,0,0 };
	//ハイスコア
	uint32_t resultHiScoreTex[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* resultHiScoreSp[9][11] = { nullptr };
	//手前のが0かどうか
	bool isZeroHiScoreResult[7] = { 0,0,0,0,0,0,0 };
	//更新差分
	bool checkUpdate = false;
	uint32_t newlavel = 0;
	Sprite* newlavelSp = nullptr;
	//ランク
	uint32_t resultRankTex[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* resultRankSp[10] = { nullptr };
	//FCAP&クリア（プレート）
	uint32_t plateTex[4] = { 0,0,0,0 };
	Sprite* plateSp[4] = { nullptr,nullptr,nullptr,nullptr };
	//コンボ表示
	uint32_t uiCombo = 0;
	Sprite* comboSprite = nullptr;
	//コンボ数表示
	uint32_t uiComboNum[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* comboNumSprite[4][10] = { nullptr,nullptr, nullptr, nullptr };
	//詳細カウント表示
	uint32_t uiData[6] = { 0,0,0,0,0,0 };
	Sprite* uiDataSprite[6] = { nullptr,nullptr, nullptr, nullptr,nullptr,nullptr };
	//詳細カウント数表示（MAXCOMBO,PERFECT、GREAT、MISSでそれぞれ4ケタ、スコアで,含めて9ケタ、4 × 4 + 9 = 25）
	uint32_t uiDataNum[10] = { 0,0,0,0,0,0,0,0,0,0 };
	Sprite* uiDataNumbers[16][10] = { nullptr };
	uint32_t HiScoreNum[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
	Sprite* HiScore[9][11] = { nullptr };
	//COMBO数表示の共通座標
	float comboNumPosX = 1610.0f;
	float comboNumPosY = 380.0f;

	//背景
	uint32_t resultBackGroundTex[3] = { 0,0,0 };
	Sprite* resultBackGroundSp[3] = { nullptr,nullptr,nullptr };

	//操作不能タイマー
	int stopTimer = 180;
};
