#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel) {
	//nullチェックってやつ
	assert(laneModel);
	assert(lineModel);
	//モデルデータを読み込む
	this->laneModel = laneModel;
	defaultModel = laneModel;

	this->lineModel = lineModel;

	//譜面データ初期化
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = 0;
				playData.layer[i].note[j].hit[k] = false;
				playData.layer[i].note[j].hitTimer[k] = 0;
				playData.layer[i].note[j].judgement[k] = 0;
				playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
				playData.layer[i].note[j].startMove[k] = false;
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	//小節線初期化
	line.lineNum = 1;
	line.countFlame = 0;
	line.countRhythm = 1;

	//譜面初期化
	ChartInitialize();

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	lanePosition.translation_ = Vector3(0, -2.0f, -35.0f);
	lanePosition.Initialize();

	startTimer = resetStartTimer;
}

void Lane::Update() {
	MatSet2 matset;

	//タイマーが0になったらスタート
	if (startTimer > 0) {
		startTimer--;
	}
	else if (startTimer <= 0) {
		line.switching = line.baseBPM * line.change;
		if (line.countFlame >= line.switching) {
			line.countRhythm++;
			line.countFlame = 0;
			//曲の拍数（分子）に合わせて小節線タイミング始動
			if (line.countRhythm >= playData.beatMolecule + 1) {
				line.countRhythm = 1;
				line.linePop[line.lineNum] = true;
				line.lineNum++;
			}
		}
		line.countFlame++;
	}

	debugText_->SetPos(10, 10);
	debugText_->Printf("BPM : %d", playData.BPM);
	debugText_->SetPos(10, 30);
	debugText_->Printf("Rhythm : %d", line.countRhythm);
	debugText_->SetPos(10, 50);
	debugText_->Printf("countFlame : %d", line.countFlame);
	debugText_->SetPos(10, 70);
	debugText_->Printf("change : %f", line.change);
	debugText_->SetPos(10, 90);
	debugText_->Printf("lineNum : %d", line.lineNum);
	debugText_->SetPos(10, 110);
	debugText_->Printf("Timer : %d", startTimer);

	//レーンの更新
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition, viewProjection);
	lineModel->Draw(lanePosition, viewProjection);
}

void Lane::LoadMusic(int ID) {
	//音楽データをコピーする
	playData.beatDenomonator = musicData[ID].beatDenomonator;
	playData.beatMolecule = musicData[ID].beatMolecule;
	playData.BPM = musicData[ID].BPM;
	playData.speed = musicData[ID].speed;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = musicData[ID].layer[i].note[j].chart[k];
			}
		}
	}
	//取得データを素に読み込み
	line.change = line.baseBPM / playData.BPM;
}

void Lane::ResetMusic() {
	//音楽データ削除
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = 0;
				playData.layer[i].note[j].hit[k] = false;
				playData.layer[i].note[j].hitTimer[k] = 0;
				playData.layer[i].note[j].judgement[k] = 0;
				playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
				playData.layer[i].note[j].startMove[k] = false;
			}
			playData.layer[i].note[j].speed = 0;
		}
	}
}

void Lane::ChangeLane(Model* model) {
	//nullチェック
	assert(model);
	//一回モデルデータを消す
	laneModel = nullptr;
	//その後入れる
	laneModel = model;
}

void Lane::DefaultLane() {
	//一回モデルデータを消す
	laneModel = nullptr;
	//その後入れる
	laneModel = defaultModel;
}

void Lane::Auto(bool select) {
	if (select) {
		autoPlay = true;
	}
	else {
		autoPlay = false;
	}
}

void Lane::ReadChart(int notes, int i, int j) {
	//ノーツの種類に合わせて設定する
	//ノーツの動きだし開始判定
	playData.layer[i].note[j].startMove[notes] = true;
	if (playData.layer[i].note[j].chart[notes] == 1) {

	}
}

void Lane::ChartInitialize() {
	//全ての曲データ初期化（四重for文…）
	for (int M = 0; M < musicNum; M++) {								//曲数
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

void Lane::ID000() {
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

void Lane::LoadData(int ID, string filePass) {

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