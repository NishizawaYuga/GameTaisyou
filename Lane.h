#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <string>

//chartManagerで読み込んだ譜面をLaneに移そうとすると容量が大きすぎてエラー吐くので
//二つのソースに分けていた処理を一つに纏めます

//ノーツ（1列分)
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

//ノーツ（4列分)
struct Notes {
	Note note[4];
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
	//譜面（レイヤー4枚）
	Notes layer[4];
};

//小節線
struct Line {
	//フレームカウント
	int countFlame;
	//拍カウント
	int countRhythm;
	//ベースとなるBPM(BPM60 = 1拍が1秒)
	const float baseBPM = 60.0f;
	//ループタイミングを調整させる倍率
	float magnification;
	//カウント倍率
	float change;
	//1拍をカウントするタイミング
	int switching;
	//小節線タイミング
	int linePop[500];
	//小節線番号
	int lineNum;
	//小節線
	WorldTransform lineWorld[500];
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
	void LoadMusic(int ID);
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
	void ReadChart(int notes,int i,int j);

	//譜面
	void ChartInitialize();
	//ファイル読み込み
	void LoadData(int ID, std::string filePass);
	//曲データ初期化群
	void ID000();	//テスト音源

private:
	//音楽データ
	//プレイで実際に使うデータ
	MusicData playData;
	//裏で格納しておくデータ
	MusicData musicData[10];

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//レーンそのもののワールド座標
	WorldTransform lanePosition;
	//レーンそのもののモデルデータ
	Model* laneModel = nullptr;
	//デフォルト用に別個で取っておく
	Model* defaultModel = nullptr;
	//小節線モデル
	Model* lineModel = nullptr;
	//小節線
	Line line;

	//オートプレイフラグ
	bool autoPlay;
	//曲開始までの時間
	//初期値
	const int resetStartTimer = 300;
	//実際に変動させる値
	int startTimer;

	//譜面
	//音楽数
	const int musicNum = 10;
	//レイヤー数
	const int layerNum = 4;
	//列数
	const int columnNum = 4;
	//置けるノーツ数
	const int maxNotes = 20000;
};