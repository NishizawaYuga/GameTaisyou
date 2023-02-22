#include "chartManager.h"

void ChartManager::Initialize() {
	//全ての曲のデータ読み込み
	ID000();
}

void ChartManager::IncludeChart(int ID,int chart[4][20000]) {
	//ごり押し読み込み
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 20000; j++) {
			musicData[ID].layer[i].chart[j] = chart[j][i];
		}
	}
}

void ChartManager::ID000() {
	//test用データ
	//分母
	musicData[0].beatDenomonator = 4;
	//分子
	musicData[0].beatMolecule = 4;
	//BPM
	musicData[0].BPM = 120;
	//譜面速度（倍率）
	musicData->speed = 1;
	//譜面
	int chart[4][20000] =
	{	//譜面データ、左端→右端で読み込んでいく
		{0,0,0,0, 0,0,0,0, 0,0,0,0,},
		{0,0,0,0, 0,0,0,0, 0,0,0,0,},
		{0,0,0,0, 0,0,0,0, 0,0,0,0,},
		{0,0,0,0, 0,0,0,0, 0,0,0,0,}
	};

	IncludeChart(0, chart);
}