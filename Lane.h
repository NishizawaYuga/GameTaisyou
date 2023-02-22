#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h" 

//↑仮でGameSceneのヤツ全部持ってきた、必要ないincludeは後に削除

//ノーツ
struct Note {
	//ワールドトランスフォーム
	WorldTransform worldTransform[20000];
	//移動速度
	float speed;
	//ヒット判定
	bool hit[20000] = {false};
	//判定チェック
	int judgement[20000] = {0};
	//判定タイマー
	int hitTimer[2000] = {0};
	//ノーツの種類
	int type[20000] = {0};
	//開始判定
	bool startMove[20000] = { false };

	//譜面
	int chart[20000] = {0};
};

//曲の基本情報
struct MusicData {
	//BPM
	int BPM;
	//拍数
	//分母
	int beatDenomonator;
	//分子
	int beatMolecule;
	//譜面速度
	int speed;
	//譜面
	Note layer[4];
};

class Lane {
public:
	//初期化
	void Initialize(Model* laneModel, Model*lineModel);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
	//読み込み
	void LoadMusic(MusicData musicData_);
	//リセット（曲終了後などに空っぽにする）
	void ResetMusic();
	//レーンの見た目変更
	void ChangeLane(Model* model);
	//デフォルトに戻す
	void DefaultLane();
	//オート化
	//trueでオートON、falseでオートOFF
	void Auto(bool select);

private:
	//譜面を読む
	void ReadChart(int notes,int i);

private:
	//音楽データ
	MusicData musicData;
	//フレームカウント
	int countFlame;
	//拍カウント
	int countRhythm;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//レーンそのもののワールド座標
	WorldTransform lanePosition;
	//ベースとなるBPM(BPM60 = 1拍が1秒)
	const float baseBPM = 60.0f;
	//ループタイミングを調整させる倍率
	float magnification;
	//レーンそのもののモデルデータ
	Model* laneModel = nullptr;
	//デフォルト用に別個で取っておく
	Model* defaultModel = nullptr;
	//小節線モデル
	Model* line = nullptr;
	//移す用データ
	Note layer[4];
	//ループさせるカウント
	const int maxNotes = 20000;
	//オートプレイフラグ
	bool autoPlay;
	//カウント倍率
	float change;
	//1拍をカウントするタイミング
	int switching;
	//小節線
	WorldTransform lineWorld[500];
	//曲開始までの時間
	//初期値
	const int resetStartTimer = 300;
	//実際に変動させる値
	int startTimer;
};