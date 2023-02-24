#pragma once
#include "Lane.h"
#include <string>

//音楽データ管理クラス
class ChartManager {

public:
	//初期化
	void Initialize();
	//音楽データ呼び出し
	MusicData GetData(int idnum) { return musicData[idnum]; }
private:
	//ファイル読み込み
	void LoadData(int ID, std::string filePass);

	//曲データ初期化群
	void ID000();	//テスト音源

private:
	//音楽データ
	MusicData musicData[10];

	//音楽数
	const int musicNum = 10;
	//レイヤー数
	const int layerNum = 4;
	//列数
	const int columnNum = 4;
	//置けるノーツ数
	const int maxNotes = 20000;
};