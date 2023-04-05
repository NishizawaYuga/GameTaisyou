#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Audio.h"

#include <string>

//chartManagerで読み込んだ譜面をLaneに移そうとすると容量が大きすぎてエラー吐くので
//二つのソースに分けていた処理を一つに纏めます

//ノーツ（1列分)
struct Note {
	//ワールドトランスフォーム
	WorldTransform worldTransform[90];
	//移動速度
	float speed;
	//ヒット判定
	bool hit[90] = { false };
	//判定チェック
	int judgement[90] = { 0 };
	//判定タイマー
	int hitTimer[90] = { 0 };
	//ノーツの種類
	int type[90] = { 0 };
	//開始判定
	bool startMove[90] = { false };

	//譜面
	int chart[2500] = { 0 };
};

//ノーツ（4列分)
struct Notes {
	Note note[4];
};

//難易度（一つ分）
struct Difficulty {
	//譜面（レイヤー4枚）
	//（横一列分の長さを短くするため縦方向に4つ分伸ばす)
	Notes layer[32];
	//最高スコア
	int maxScore = 0;
	//最高ランク
	int maxRankNum = 0;
	std::string maxRank = "D";
	//FC、APの情報
	int isFCAP = 0;
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
	//各難易度ごとの譜面
	Difficulty difficulty[4];
	//難易度のレベル（難易度別）
	int level[4];
	//曲の開始タイミング
	int playMusicCount;
	//スコアデータのファイルパス
	std::string dataPass = "a";
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
	const int lineNum = 500;
	//小節線
	WorldTransform lineWorld[500];
};

class Lane {
public:
	//初期化
	void Initialize(Model* laneModel, Model* lineModel, Model* noteModel[12]);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
	//読み込み
	void LoadMusic(int ID,int difficulty);
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
	void ReadChart();
	//スタート時の設定
	void SetNote(int i, int j, int k, int typeNum);
	//判定
	void Judgement();
	//列一つ分の判定
	void ColumnHit(int layer, int columnNum, int notes, bool trigger, bool push);
	//太いノーツの押された判定を返す
	bool ThickColumn(bool key1=false, bool key2=false, bool key3=false, bool key4=false);
	//レート計算
	void UpdateRate(int RateScore);
	//譜面
	void ChartInitialize();
	//小節線更新
	void LineUpdate();
	//ファイル読み込み
	void LoadData(int ID,int difficulty, std::string filePass);
	//曲データ初期化群
	void ID000(std::string filePass, int musicID);	//テスト音源
	//IDと譜面データのパスと曲のパスとスコアデータのパス、BPMと難易度さえ指定すればOK、デフォルトでレベル0の曲開始0秒の4/4拍子の譜面速度倍率1
	void IDEntry(int musicID, std::string filePass, const std::string musicPass,const std::string scoreDataPass, int BPM, int difficultyNum, int level = 0,
		int startMusicCount = 0, int beatDenomonator = 4, int beatMolecule = 4, int speed = 1);	//汎用ID登録関数
	//曲の終了関数
	void FinishMusic();

private:
	//音楽データ
	//プレイで実際に使うデータ
	MusicData playData;
	//裏で格納しておくデータ
	MusicData musicData[10];
	//オーディオ
	Audio* audioSE = nullptr;
	Audio* audioMusic = nullptr;

	//キー入力
	Input* input_ = nullptr;
	//デバッグテキスト
	DebugText* debugText_ = nullptr;

	//レーンそのもののワールド座標
	WorldTransform lanePosition;
	//レーンそのもののモデルデータ
	Model* laneModel = nullptr;
	//デフォルト用に別個で取っておく
	Model* defaultModel = nullptr;
	//小節線モデル
	Model* lineModel = nullptr;
	//ノーツモデル
	Model* notesModel[12];
	//ノーツモデル数
	const int modelNum = 12;
	//小節線
	Line line;

	//オートプレイフラグ
	bool autoPlay;
	//曲開始までの時間
	//初期値
	const int resetStartTimer = 300;
	//実際に変動させる値
	int startTimer;

	//曲終了までの時間
	const int resetEndTimer = 300;
	//実際に変動させる値
	int endTimer;

	//譜面
	//音楽数
	const int musicNum = 10;
	//レイヤー数
	const int layerNum = 4;
	//列数
	const int columnNum = 4;
	//置けるノーツ数
	const int maxNotes = 2500;
	//全体の列変更のずらし数値
	int shift = 0;
	//全体の行数
	const int shiftMaxNum = 8;
	//表示するノーツ
	const int drawNotes = 90;
	//判定に使用するタイマー
	const int startMoveTimer = 120;
	//譜面速度（本来の）
	float speed;
	//曲開始
	bool startMusic;
	//ノーツカウンター
	int notesCounter = 0;
	//譜面の読み取り番号
	int chartNum;
	//判定ラインからスタート地点までの距離
	float distance;

	//曲ID
	int musicID = 0;
	//難易度番号　0 : Easy, 1 : Normal, 2 : Master, 3 : Prank
	int difficulty = 2;

	//コンボ数
	int combo;
	//最大コンボ数
	int maxCombo;
	//PERFECT数
	int perfect;
	//GREAT数
	int great;
	//MISS数
	int miss;

	//スコア
	int score;
	//ランク決めとかに使いそうな平均レート
	float averageRate;
	//割合を出すためのレート
	float rate;
	//平均を割り出すためのカウンター
	int accuracyCounter;
	//ランク
	std::string rank;
	int rankNum = 0;

	//判定調整用
	int fastJudge;
	int lateJudge;

	//デバッグ用
	bool moveFlag = false;

	//サウンドデータハンドル
	uint32_t music[10] = { 0 };
	uint32_t SE[3] = { 0,0,0 };
};