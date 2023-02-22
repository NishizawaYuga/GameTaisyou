#pragma once
#include "Lane.h"

//音楽データ管理クラス
class ChartManager {

public:
	//初期化
	void Initialize();
	//音楽データ呼び出し
	MusicData GetData(int idnum) { return musicData[idnum]; }
private:
	//譜面を各曲に入れる
	void IncludeChart(int ID,int chart[4][20000]);
	//曲データ初期化群
	void ID000();	//テスト音源

private:
	//音楽データ
	MusicData musicData[10];
};