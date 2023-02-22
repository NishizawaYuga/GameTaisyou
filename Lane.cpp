#include"Lane.h"
#include <cassert>
#include "MatSet.h"

void Lane::Initialize(Model* laneModel, Model* lineModel) {
	//nullチェックってやつ
	assert(laneModel);
	assert(lineModel);
	//モデルデータを読み込む
	this->laneModel = laneModel;
	defaultModel = laneModel;

	line = lineModel;

	musicData.beatDenomonator = 0;
	musicData.beatMolecule = 0;
	musicData.BPM = 0;
	musicData.speed = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < maxNotes; j++) {
			musicData.layer[i].chart[j] = 0;
			musicData.layer[i].hit[j] = false;
			musicData.layer[i].hitTimer[j] = 0;
			musicData.layer[i].judgement[j] = 0;
			musicData.layer[i].worldTransform[j].translation_ = { 0,0,0 };
			musicData.layer[i].startMove[j] = false;
		}
		musicData.layer[i].speed = 0;
	}

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	lanePosition.translation_ = Vector3(0, -2.0f, -35.0f);
	lanePosition.Initialize();

	countFlame = 0;
	countRhythm = 0;

	startTimer = resetStartTimer;
}

void Lane::Update() {
	MatSet2 matset;

	//タイマーが0になったらスタート
	if (startTimer > 0) {
		startTimer--;
	}
	else if (startTimer < 0) {
		switching = baseBPM * change;
		if (countFlame >= switching) {
			countRhythm++;
			countFlame = 0;
		}
		countFlame++;
	}

	//レーンの更新
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition,viewProjection);
	line->Draw(lanePosition, viewProjection);
}

void Lane::LoadMusic(MusicData musicData) {
	//音楽データをコピーする
	this->musicData.beatDenomonator = musicData.beatDenomonator;
	this->musicData.beatMolecule = musicData.beatMolecule;
	this->musicData.BPM = musicData.BPM;
	this->musicData.speed = musicData.speed;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < maxNotes; j++) {
			this->musicData.layer[i].chart[j] = musicData.layer[i].chart[j];
		}
	}
	//取得データを素に読み込み
	change = baseBPM / this->musicData.BPM;
}

void Lane::ResetMusic() {
	//音楽データ削除
	musicData.beatDenomonator = 0;
	musicData.beatMolecule = 0;
	musicData.BPM = 0;
	musicData.speed = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < maxNotes; j++) {
			musicData.layer[i].chart[j] = 0;
			musicData.layer[i].hit[j] = false;
			musicData.layer[i].hitTimer[j] = 0;
			musicData.layer[i].judgement[j] = 0;
			musicData.layer[i].worldTransform[j].translation_ = {0,0,0};
			musicData.layer[i].startMove[j] = false;
		}
		musicData.layer[i].speed = 0;
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

void Lane::ReadChart(int notes,int i) {
	//ノーツの種類に合わせて設定する
	//ノーツの動きだし開始判定
	musicData.layer[i].startMove[notes] = true;
	if (musicData.layer[i].chart[notes] == 1) {

	}
}