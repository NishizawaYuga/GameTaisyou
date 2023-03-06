#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel, Model* noteModel[21]) {
	//nullチェックってやつ
	assert(laneModel);
	assert(lineModel);
	for (int i = 0; i < modelNum; i++) {
		assert(noteModel[i]);
	}

	//ノーツモデルnullptr代入
	for (int i = 0; i < modelNum; i++) {
		notesModel[i] = nullptr;
	}

	//モデルデータを読み込む
	this->laneModel = laneModel;
	defaultModel = laneModel;

	this->lineModel = lineModel;

	for (int i = 0; i < modelNum; i++) {
		notesModel[i] = noteModel[i];
	}

	//譜面データ初期化
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = 0;
				if (k < drawNotes) {
					playData.layer[i].note[j].hit[k] = false;
					playData.layer[i].note[j].hitTimer[k] = 0;
					playData.layer[i].note[j].judgement[k] = 0;
					playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
					playData.layer[i].note[j].worldTransform[k].Initialize();
					playData.layer[i].note[j].startMove[k] = false;
				}
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	//譜面速度（デフォルト）
	speed = 0.3f;

	distance = speed * 60.0f;//(60.0f->1秒)

	//小節線初期化
	line.countFlame = 0;
	line.countRhythm = 1;

	//譜面初期化
	ChartInitialize();

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
	//小節線
	for (int i = 0; i < line.lineNum; i++) {
		line.linePop[i] = false;
		line.lineWorld[i].translation_ = Vector3(0, 0, 0);
		line.lineWorld[i].Initialize();
	}

	//レーン
	lanePosition.translation_ = Vector3(0, -2.0f, -35.0f);
	lanePosition.Initialize();

	startTimer = resetStartTimer;

	startMusic = false;

	chartNum = 0;
}

void Lane::Update() {
	MatSet2 matset;
	//デバッグ用にスペースを押してる間だけ進むように
	if (input_->TriggerKey(DIK_SPACE)) {
		if (!moveFlag) {
			moveFlag = true;
		}
		else if (moveFlag) {
			moveFlag = false;
		}
	}
	//if (input_->PushKey(DIK_SPACE)) {
		//タイマーが0になったらスタート
	if (moveFlag) {
		if (startTimer > 0) {
			startTimer--;
		}
		if (startTimer <= 60) {
			LineUpdate();
		}
		if (startMusic) {
			ReadChart();
		}
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
	debugText_->SetPos(10, 130);
	debugText_->Printf("noteZ : %f", playData.layer[0].note[0].worldTransform[0].translation_.z);

	//小節線更新
	for (int i = 0; i < line.lineNum; i++) {
		matset.MatIdentity(line.lineWorld[i]);
		line.lineWorld[i].TransferMatrix();
	}

	//ノーツ更新
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.layer[i].note[j].startMove[k]) {
					matset.MatIdentity(playData.layer[i].note[j].worldTransform[k]);
					playData.layer[i].note[j].worldTransform[k].TransferMatrix();
				}
			}
		}
	}
	//レーンの更新
	matset.MatIdentity(lanePosition);
	lanePosition.TransferMatrix();
	//}
}

void Lane::Draw(ViewProjection viewProjection) {
	laneModel->Draw(lanePosition, viewProjection);
	for (int i = 0; i < line.lineNum; i++) {
		if (line.linePop[i]) {
			lineModel->Draw(line.lineWorld[i], viewProjection);
		}
	}
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.layer[i].note[j].type[k] == 1) {
					notesModel[0]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
				}
				else if (playData.layer[i].note[j].type[k] == 2 || playData.layer[i].note[j].type[k] == 5 || playData.layer[i].note[j].type[k] == 6) {
					notesModel[1]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
				}
				else if (playData.layer[i].note[j].type[k] == 3 || playData.layer[i].note[j].type[k] == 4) {
					notesModel[2]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
				}
			}
		}
	}
}

void Lane::LoadMusic(int ID) {

	//音楽データをコピーする
	playData.beatDenomonator = musicData[ID].beatDenomonator;
	playData.beatMolecule = musicData[ID].beatMolecule;
	playData.BPM = musicData[ID].BPM;
	playData.speed = musicData[ID].speed;
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.layer[i].note[j].chart[k] = musicData[ID].layer[i].note[j].chart[k];
				if (k < drawNotes) {
					playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.0f - i * 0.01f, -45.5f + distance * playData.speed);
				}
			}
		}
	}
	//取得データを素に読み込み
	line.change = line.baseBPM / playData.BPM;
	//小節線の位置決め
	for (int i = 0; i < line.lineNum; i++) {
		//判定ラインまでの距離を ジャスト座標 + (移動速度 * 1秒のフレーム数) * 曲ごとの速度倍率 で取る ※画面に出現から判定ラインまで1秒で来る想定(デフォ)
		line.lineWorld[i].translation_ = Vector3(0, -2.0f, -45.5f + distance * playData.speed);
		line.lineWorld[i].TransferMatrix();
	}
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
				if (k < drawNotes) {
					playData.layer[i].note[j].hit[k] = false;
					playData.layer[i].note[j].hitTimer[k] = 0;
					playData.layer[i].note[j].judgement[k] = 0;
					playData.layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
					playData.layer[i].note[j].startMove[k] = false;
				}
			}
			playData.layer[i].note[j].speed = 0;
		}
	}

	startMusic = 0;
	startTimer = resetStartTimer;
	chartNum = 0;
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

void Lane::LineUpdate() {
	line.switching = line.baseBPM * line.change;
	if (line.countFlame >= line.switching) {
		line.countRhythm++;
		line.countFlame = 0;
		//曲の拍数（分子）に合わせて小節線タイミング始動
		if (line.countRhythm >= playData.beatMolecule + 1) {
			line.countRhythm = 1;
			//使われていない小節線をオンにする
			for (int i = 0; i < line.lineNum; i++) {
				if (!line.linePop[i]) {
					line.linePop[i] = true;
					break;
				}
			}
			if (!startMusic) {
				startMusic = true;
			}
		}
	}
	line.countFlame++;
	//小節線移動
	for (int i = 0; i < line.lineNum; i++) {
		if (line.linePop[i]) {
			line.lineWorld[i].translation_.z -= speed;
			if (line.lineWorld[i].translation_.z < -50.0f) {
				//判定ラインを通り過ぎて画面外まで行ったらfalseにして初期位置に戻してリサイクル
				float distance = speed * 60.0f;//(60.0f->1秒)
				line.lineWorld[i].translation_ = Vector3(0, -2.0f, -45.5f + distance * playData.speed);
				line.linePop[i] = false;
			}
		}
	}
}

void Lane::ReadChart() {
	//1フレームごとに譜面を読む
	for (int i = 0; i < layerNum; i++) {	//レイヤー数
		for (int j = 0; j < columnNum; j++) {	//列数
			if (playData.layer[i].note[j].chart[chartNum] == 1) {	//ノーツの有無チェック
				for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						break;											//空きが見つかったら即脱出
					}
				}
			}
			else if (playData.layer[i].note[j].chart[chartNum] == 2 || playData.layer[i].note[j].chart[chartNum] == 5 || playData.layer[i].note[j].chart[chartNum] == 6) {	//ノーツの有無チェック
				for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						break;											//空きが見つかったら即脱出
					}
				}
			}
			else if (playData.layer[i].note[j].chart[chartNum] == 3 || playData.layer[i].note[j].chart[chartNum] == 4) {	//ノーツの有無チェック
				for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.01f - i * 0.01f, -45.5f + distance * playData.speed);
						break;											//空きが見つかったら即脱出
					}
				}
			}
		}
	}

	for (int i = 0; i < layerNum; i++) {	//レイヤー数
		for (int j = 0; j < columnNum; j++) {	//列数
			for (int k = 0; k < drawNotes; k++) {	//表示可能ノーツ数
				if (playData.layer[i].note[j].startMove[k]) {	//スタートフラグオンなら
					playData.layer[i].note[j].worldTransform[k].translation_.z -= speed;	//譜面速度に合わせて移動
					//ノーツが一定ラインを通り過ぎるかヒット判定がオンでリセット
					if (playData.layer[i].note[j].worldTransform[k].translation_.z < -50.0f || playData.layer[i].note[j].hit[k]) {
						playData.layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.0f - i * 0.01f, -45.5f + distance * playData.speed);
						playData.layer[i].note[j].hit[k] = false;
						playData.layer[i].note[j].hitTimer[k] = 0;
						playData.layer[i].note[j].type[k] = 0;
						playData.layer[i].note[j].startMove[k] = false;
						playData.layer[i].note[j].judgement[k] = 0;
					}
				}
			}
		}
	}

	//カウントを進める
	chartNum++;
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
	//譜面データがあるファイルの場所と格納したい配列の番号を指定する
	ID000("Resources/musicData/testmc.txt",0);
}

void Lane::ID000(string filePass,int musicID) {
	//test用データ
	//分母
	musicData[musicID].beatDenomonator = 4;
	//分子
	musicData[musicID].beatMolecule = 4;
	//BPM
	musicData[musicID].BPM = 175;
	//譜面速度（倍率）
	musicData[musicID].speed = 1;

	LoadData(musicID, filePass);
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

void Lane::SetNote(int i, int j,int k, int typeNum) {
	playData.layer[i].note[j].startMove[k] = true;	//開始フラグオン
	playData.layer[i].note[j].type[k] = typeNum;			//ノーツの種類設定
	playData.layer[i].note[j].hitTimer[k] = 60;		//判定カウントの設定
}