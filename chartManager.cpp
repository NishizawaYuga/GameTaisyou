#include "chartManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void ChartManager::Initialize() {
	//全ての曲データ初期化（四重for文…）
	for(int M = 0;M < musicNum;M++){								//曲数
		for (int i = 0; i < layerNum; i++) {						//レイヤー数
			for (int j = 0; j < columnNum; j++) {					//列数
				for (int k = 0; k < maxNotes; k++) {				//置けるノーツ数
					musicData[M].layer[i].note[j].chart[k] = 0;
				}
			}
		}
		musicData[M].BPM = 0;
		musicData[M].beatDenomonator = 0;
		musicData[M].beatMolecule = 0;
		musicData[M].speed = 0;
	}

	//全ての曲のデータ読み込み
	ID000();
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

	LoadData(0, "musicData/testmc.txt");
}

void ChartManager::LoadData(int ID, string filePass) {

	//入力されたファイルパスからファイルオブジェクト作成
	ifstream ifs(filePass);

	//開かなかったらエラー
	if (!ifs) {
		cout << "ファイルを開けませんでした" << endl;
		return;
	}

	string str = "";

	//レイヤー
	int i = 0;
	//列
	int j = 0;
	//置けるノーツ数
	int k = 0;
	 
	//ファイルの中身を一行ずつ読み込む
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		//区切り文字がなくなるまで文字を区切る
		while (getline(stream, tmp, ',')) {
			musicData[ID].layer[i].note[j].chart[k] = atoi(tmp.c_str());
			k++;
		}
		//ノーツ読み込みを最初からに
		k = 0;
		//次の列へ
		j++;
		//jが全ての行を読み込んだらリセットしてレイヤー変更
		if (j > 3) {
			j = 0;
			i++;
		}
	}
}