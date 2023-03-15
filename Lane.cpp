#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "MatSet.h"

using namespace std;

void Lane::Initialize(Model* laneModel, Model* lineModel, Model* noteModel[12]) {
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
					playData.layer[i].note[j].hitTimer[k] = 60;
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
	audioSE = Audio::GetInstance();
	audioSE->Initialize();
	audioMusic = Audio::GetInstance();
	audioMusic->Initialize();

	//音楽
	music[0] = audioMusic->LoadWave("musicData/000/test.wav");
	music[1] = audioMusic->LoadWave("musicData/001/banbado.wav");

	//効果音
	SE[0] = audioSE->LoadWave("se/perfect.wav");
	SE[1] = audioSE->LoadWave("se/great.wav");

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

	combo = 0;
	maxCombo = 0;
	perfect = 0;
	great = 0;
	miss = 0;

	fastJudge = 4;
	lateJudge = -4;

	autoPlay = true;
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
		if (startTimer > -121) {
			startTimer--;
		}
		if (startTimer <= 60) {
			LineUpdate();
		}
		if (startMusic) {
			ReadChart();
			Judgement();
			if (startTimer == -100) {
				//testmcは開始120
				//if (!audioMusic->IsPlaying(music[0])) {
				audioMusic->PlayWave(music[1]);
			}
			//}
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
	debugText_->SetPos(10, 150);
	debugText_->Printf("Combo : %d", combo);
	debugText_->SetPos(10, 170);
	debugText_->Printf("MAXCombo : %d", maxCombo);
	debugText_->SetPos(10, 190);
	debugText_->Printf("PERFECT : %d", perfect);
	debugText_->SetPos(10, 210);
	debugText_->Printf("GREAT : %d", great);
	debugText_->SetPos(10, 230);
	debugText_->Printf("MISS : %d", miss);

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
				int drawNum = playData.layer[i].note[j].type[k];
				if (drawNum > 20) { drawNum -= 12;}
				else if (drawNum > 16) { drawNum -= 8; }
				else if (drawNum > 12) { drawNum -= 8; }
				else if (drawNum > 8) { drawNum -= 4; }

				if (playData.layer[i].note[j].type[k] != 0) {
					notesModel[drawNum - 1]->Draw(playData.layer[i].note[j].worldTransform[k], viewProjection);
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
					playData.layer[i].note[j].hitTimer[k] = 60;
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

void Lane::Judgement() {
	//各ノーツのhit判定
	for (int i = 0; i < layerNum; i++) {
		for (int k = 0; k < drawNotes; k++) {
			//列ごとにhit判定を取る
			//ノーツの種類によって処理の仕方を変える
			if (playData.layer[i].note[0].type[k] % 4 == 1) { ColumnHit(i, 0, k, input_->TriggerKey(DIK_F), input_->PushKey(DIK_F)); }
			if (playData.layer[i].note[1].type[k] % 4 == 1) { ColumnHit(i, 1, k, input_->TriggerKey(DIK_G), input_->PushKey(DIK_G)); }
			if (playData.layer[i].note[2].type[k] % 4 == 1) { ColumnHit(i, 2, k, input_->TriggerKey(DIK_H), input_->PushKey(DIK_H)); }
			if (playData.layer[i].note[3].type[k] % 4 == 1) { ColumnHit(i, 3, k, input_->TriggerKey(DIK_J), input_->PushKey(DIK_J)); }
			//1/2ノーツ
			if (playData.layer[i].note[0].type[k] % 4 == 2) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G)));
			}
			if (playData.layer[i].note[1].type[k] % 4 == 2) {
				ColumnHit(i, 1, k, ThickColumn(input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H)), ThickColumn(input_->PushKey(DIK_G), input_->PushKey(DIK_H)));
			}
			if (playData.layer[i].note[2].type[k] % 4 == 2) {
				ColumnHit(i, 2, k, ThickColumn(input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
			//3/4ノーツ
			if (playData.layer[i].note[0].type[k] % 4 == 3) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G), input_->PushKey(DIK_H)));
			}
			if (playData.layer[i].note[1].type[k] % 4 == 3) {
				ColumnHit(i, 1, k, ThickColumn(input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_G), input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
			//1/1ノーツ
			if (playData.layer[i].note[0].type[k] % 4 == 0 && playData.layer[i].note[0].type[k] != 0) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G), input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
		}
	}
	//hitTimerを基に評価決め
	for (int i = 0; i < layerNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.layer[i].note[j].hit[k]) {
					//PERFECT判定（6フレーム)
					if (playData.layer[i].note[j].hitTimer[k] >= lateJudge && playData.layer[i].note[j].hitTimer[k] <= fastJudge) {
						playData.layer[i].note[j].judgement[k] = 1;
						if (playData.layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[0]);
						}
					}
					//GREAT判定(FAST)（2フレーム）
					else if (playData.layer[i].note[j].hitTimer[k] > fastJudge && playData.layer[i].note[j].hitTimer[k] <= fastJudge + 2) {
						playData.layer[i].note[j].judgement[k] = 2;
						if (playData.layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[1]);
						}
						if (playData.layer[i].note[j].type[k] > 12 && playData.layer[i].note[j].type[k] < 17) {
							//HOLD終点のみFAST判定無し
							playData.layer[i].note[j].judgement[k] = 1;
						}
					}
					//GREAT判定(LATE)（2フレーム）
					else if (playData.layer[i].note[j].hitTimer[k] < lateJudge && playData.layer[i].note[j].hitTimer[k] >= lateJudge - 2) {
						playData.layer[i].note[j].judgement[k] = 2;
						if (playData.layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[1]);
						}
					}
					//MISS判定(FAST)（1フレーム）
					else if (playData.layer[i].note[j].hitTimer[k] > fastJudge + 2 && playData.layer[i].note[j].hitTimer[k] <= fastJudge + 3) {
						playData.layer[i].note[j].judgement[k] = 3;
						if (playData.layer[i].note[j].type[k] > 12 && playData.layer[i].note[j].type[k] < 17) {
							//HOLD終点のみFAST判定無し
							playData.layer[i].note[j].judgement[k] = 1;
						}
					}
				}
				//MISS判定（スルー判定）
				if (playData.layer[i].note[j].hitTimer[k] < lateJudge - 2) {
					playData.layer[i].note[j].judgement[k] = 3;
					playData.layer[i].note[j].hit[k] = true;
				}
				//評価に応じてカウントする値を変動させる
				//HOLDの中間だけはスルーさせる
				if (playData.layer[i].note[j].type[k] != 3) {
					if (playData.layer[i].note[j].judgement[k] == 1) {
						combo++;
						perfect++;
						//MAXコンボ確認
						if (maxCombo < combo) {
							maxCombo = combo;
						}
						break;
					}
					else if (playData.layer[i].note[j].judgement[k] == 2) {
						combo++;
						great++;
						//MAXコンボ確認
						if (maxCombo < combo) {
							maxCombo = combo;
						}
						break;
					}
					else if (playData.layer[i].note[j].judgement[k] == 3) {
						//MISSしたらコンボカウントを0にする
						combo = 0;
						miss++;
						break;
					}
				}
			}
		}
	}
}

void Lane::ColumnHit(int layer, int columnNum, int notes, bool trigger, bool push) {
	//短押し
	if (!autoPlay) {
		//幅の違うノーツが多重でhit判定を出さないようにhitがfalseじゃないと処理されないように変更
		if (!playData.layer[layer].note[columnNum].hit[notes]) {
			if (trigger) {
				//TAP系列のみ（HOLD始点はTAP判定
				if (playData.layer[layer].note[columnNum].hitTimer[notes] <= fastJudge + 5) {
					if (playData.layer[layer].note[columnNum].type[notes] < 9) {
						playData.layer[layer].note[columnNum].hit[notes] = true;
					}
				}
			}
			//長押し
			if (push) {
				//長押し状態のHOLDのFAST判定をなくす
				if (playData.layer[layer].note[columnNum].hitTimer[notes] <= 0) {
					//HOLD系列
					if (playData.layer[layer].note[columnNum].type[notes] > 8) {
						playData.layer[layer].note[columnNum].hit[notes] = true;
					}
				}
			}
		}
	}
	else if (autoPlay) {
		if (playData.layer[layer].note[columnNum].hitTimer[notes] == 0) {
			playData.layer[layer].note[columnNum].hit[notes] = true;
		}
	}
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
			if (playData.layer[i].note[j].chart[chartNum] > 0 && playData.layer[i].note[j].chart[chartNum] < 17) {	//ノーツの有無チェック
				for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
					if (!playData.layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
						break;											//空きが見つかったら即脱出
					}
				}
			}
			//else if (playData.layer[i].note[j].chart[chartNum] == 2 || playData.layer[i].note[j].chart[chartNum] == 5 || playData.layer[i].note[j].chart[chartNum] == 6) {	//ノーツの有無チェック
			//	for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
			//		if (!playData.layer[i].note[j].startMove[k]) {
			//			SetNote(i, j, k, playData.layer[i].note[j].chart[chartNum]);
			//			break;											//空きが見つかったら即脱出
			//		}
			//	}
			//}
			else if (playData.layer[i].note[j].chart[chartNum] > 16) {	//ノーツの有無チェック
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
						playData.layer[i].note[j].hitTimer[k] = 60;
						playData.layer[i].note[j].type[k] = 0;
						playData.layer[i].note[j].startMove[k] = false;
						playData.layer[i].note[j].judgement[k] = 0;
					}
					if (!playData.layer[i].note[j].hit[k]) {
						playData.layer[i].note[j].hitTimer[k]--;	//タイマーを減らす
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
	ID000("Resources/musicData/000/testmc.txt", 0);
	IDEntry(1, "Resources/musicData/001/banbado.txt", "musicData/001/banbado.wav",144);
}

void Lane::ID000(string filePass, int musicID) {
	//test用データ
	//分母
	musicData[musicID].beatDenomonator = 4;
	//分子
	musicData[musicID].beatMolecule = 4;
	//BPM
	musicData[musicID].BPM = 120;
	//譜面速度（倍率）
	musicData[musicID].speed = 1;

	LoadData(musicID, filePass);
}

void Lane::IDEntry(int musicID, std::string filePass, std::string musicPass,int BPM, int beatDenomonator, int beatMolecule, int speed) {
	//分母
	musicData[musicID].beatDenomonator = beatDenomonator;
	//分子
	musicData[musicID].beatMolecule = beatMolecule;
	//BPM
	musicData[musicID].BPM = BPM;
	//譜面速度（倍率）
	musicData[musicID].speed = speed;

	LoadData(musicID, filePass);

	//music[musicID] = audioMusic->LoadWave(musicPass);
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

void Lane::SetNote(int i, int j, int k, int typeNum) {
	playData.layer[i].note[j].startMove[k] = true;	//開始フラグオン
	playData.layer[i].note[j].type[k] = typeNum;			//ノーツの種類設定
	//playData.layer[i].note[j].hitTimer[k] = 60;		//判定カウントの設定
}

bool Lane::ThickColumn(bool key1, bool key2, bool key3, bool key4) {
	if (key1 || key2 || key3 || key4) {
		return true;
	}
	return false;
}