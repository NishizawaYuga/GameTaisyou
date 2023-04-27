#include"Lane.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include "TextureManager.h"

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
	for (int m = 0; m < 4; m++) {
		for (int i = 0; i < layerNum * shiftMaxNum; i++) {
			for (int j = 0; j < columnNum; j++) {
				for (int k = 0; k < maxNotes; k++) {
					playData.difficulty[m].layer[i].note[j].chart[k] = 0;
					if (k < drawNotes) {
						playData.difficulty[m].layer[i].note[j].hit[k] = false;
						playData.difficulty[m].layer[i].note[j].hitTimer[k] = 60;
						playData.difficulty[m].layer[i].note[j].judgement[k] = 0;
						playData.difficulty[m].layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
						playData.difficulty[m].layer[i].note[j].worldTransform[k].Initialize();
						playData.difficulty[m].layer[i].note[j].startMove[k] = false;
					}
				}
				playData.difficulty[m].layer[i].note[j].speed = 0;
			}
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
	SE[2] = audioSE->LoadWave("se/miss.wav");

	//小節線
	for (int i = 0; i < line.lineNum; i++) {
		line.linePop[i] = false;
		line.lineWorld[i].translation_ = Vector3(0, 0, 0);
		line.lineWorld[i].Initialize();
	}

	//スプライト
	uiCombo = TextureManager::Load("ui/playdata/combo.png");
	comboSprite = Sprite::Create(uiCombo, { comboNumPosX - 60,comboNumPosY - 75 });
	for (int i = 0; i < 10; i++) {
		uiComboNum[i] = TextureManager::Load("font/font_dark" + std::to_string(i) + ".png");
	}
	for (int i = 0; i < 5; i++) {
		uiData[i] = TextureManager::Load("ui/playdata/datafont" + std::to_string(i) + ".png");
		uiDataSprite[i] = Sprite::Create(uiData[i], { 10.0f,150.0f + 45.0f * i });
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 10; j++) {
			comboNumSprite[i][j] = nullptr;
			comboNumSprite[i][j] = Sprite::Create(uiComboNum[j], { comboNumPosX,comboNumPosY });
		}
	}
	for (int i = 0; i < 10; i++) {
		uiDataNum[i] = TextureManager::Load("ui/playdata/playfont" + std::to_string(i) + ".png");
	}
	for (int i = 0; i < 16; i++) {
		int j = i / 4;
		//j = j / 4;
		for (int k = 0; k < 10; k++) {
			uiDataNumbers[i][k] = nullptr;
			uiDataNumbers[i][k] = Sprite::Create(uiDataNum[k], { 210.0f + i % 4 * 35.0f,150.0f + 45.0f * j });
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 11; j++) {
			uiScoreNum[j] = TextureManager::Load("ui/playdata/playfont_s" + std::to_string(j) + ".png");
			uiScore[i][j] = nullptr;
			uiScore[i][j] = Sprite::Create(uiScoreNum[j], { 10.0f + i * 45.0f,375.0f });
		}
	}


	//レーン
	lanePosition.translation_ = Vector3(0, -2.0f, -35.0f);
	lanePosition.Initialize();

	startTimer = resetStartTimer;
	endTimer = resetEndTimer;

	startMusic = false;

	chartNum = 0;

	combo = 0;
	maxCombo = 0;
	perfect = 0;
	great = 0;
	miss = 0;

	fastJudge = 4;
	lateJudge = -4;

	score = 0;

	averageRate = 0;
	rate = 0;
	accuracyCounter = 0;
	rank = "D";

	quota = 0;

	autoPlay = false;

	//LoadMusic(1, difficulty);
}

void Lane::Update(int& scene) {
	MatSet2 matset;
	//デバッグ用にスペースを押してる間だけ進むように
	if (!moveFlag) {
		if (input_->TriggerKey(DIK_SPACE)) {
			moveFlag = true;
		}
		if (input_->TriggerKey(DIK_Q)) {
			if (autoPlay) { autoPlay = false; }
			else if (!autoPlay) { autoPlay = true; }
		}

		if (input_->TriggerKey(DIK_UP)) {
			difficulty = 3;
			LoadMusic(1, difficulty);
		}
		if (input_->TriggerKey(DIK_DOWN)) {
			difficulty = 2;
			LoadMusic(1, difficulty);
		}
	}
	//if (input_->PushKey(DIK_SPACE)) {
		//タイマーが0になったらスタート
	else if (moveFlag) {
		if (startTimer > -121) {
			startTimer--;
		}
		if (startTimer <= 60) {
			LineUpdate();
		}
		if (startMusic) {
			ReadChart();
			Judgement();
			if (startTimer == -musicData[musicID].playMusicCount) {
				//testmcは開始120
				//if (!audioMusic->IsPlaying(music[0])) {
				audioMusic->PlayWave(music[musicID]);
			}
			//}
		}
		if (notesCounter < 1) {
			endTimer--;
			if (endTimer < 0) {
				FinishMusic(scene);
			}
		}
	}

	/*debugText_->SetPos(10, 90);
	debugText_->Printf("all + Counter : %d", perfect + great + miss + notesCounter);
	debugText_->SetPos(10, 110);
	debugText_->Printf("Counter : %d", notesCounter);
	debugText_->SetPos(10, 130);
	debugText_->Printf("BPM : %d", playData.BPM);
	debugText_->SetPos(10, 150);
	debugText_->Printf("Difficulty : %d", difficulty);
	debugText_->SetPos(10, 170);
	debugText_->Printf("Level : %d", playData.level[0]);
	debugText_->SetPos(10, 190);
	debugText_->Printf("Combo : %d", combo);
	debugText_->SetPos(10, 210);
	debugText_->Printf("MAXCombo : %d", maxCombo);
	debugText_->SetPos(10, 230);
	debugText_->Printf("PERFECT : %d", perfect);
	debugText_->SetPos(10, 160);
	debugText_->Printf("GREAT : %d", great);
	debugText_->SetPos(10, 270);
	debugText_->Printf("MISS : %d", miss);
	debugText_->SetPos(10, 290);
	debugText_->Printf("HIGHSCORE : %d", musicData[musicID].difficulty[difficulty].maxScore);
	debugText_->SetPos(10, 310);
	debugText_->Printf("HIGHRANK : %d", musicData[musicID].difficulty[difficulty].maxRankNum);
	debugText_->SetPos(10, 330);
	debugText_->Printf("FCAP : %d", musicData[musicID].difficulty[difficulty].isFCAP);
	debugText_->SetPos(10, 350);
	debugText_->Printf("SCORE : %d", score);
	debugText_->SetPos(10, 370);
	debugText_->Printf("RATE : %5.2f%%(%5.2f%%)", averageRate, quota);
	debugText_->SetPos(10, 390);
	debugText_->Printf("aaa : %d", moveFlag);
	debugText_->SetPos(10, 410);
	debugText_->Printf("Control : FGHJ");
	debugText_->SetPos(10, 430);
	if (autoPlay) {
		debugText_->Printf("Press Q to auto : true");
	}
	else if (!autoPlay) {
		debugText_->Printf("Press Q to auto : false");
	}*/


	//小節線更新
	for (int i = 0; i < line.lineNum; i++) {
		matset.MatIdentity(line.lineWorld[i]);
		line.lineWorld[i].TransferMatrix();
	}

	//ノーツ更新
	for (int i = 0; i < layerNum * shiftMaxNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.difficulty[0].layer[i].note[j].startMove[k]) {
					matset.MatIdentity(playData.difficulty[0].layer[i].note[j].worldTransform[k]);
					playData.difficulty[0].layer[i].note[j].worldTransform[k].TransferMatrix();
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
	for (int i = 0; i < layerNum * shiftMaxNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				int drawNum = playData.difficulty[0].layer[i].note[j].type[k];
				if (drawNum > 20) { drawNum -= 12; }
				else if (drawNum > 16) { drawNum -= 8; }
				else if (drawNum > 12) { drawNum -= 8; }
				else if (drawNum > 8) { drawNum -= 4; }

				if (playData.difficulty[0].layer[i].note[j].type[k] != 0) {
					notesModel[drawNum - 1]->Draw(playData.difficulty[0].layer[i].note[j].worldTransform[k], viewProjection);
				}

			}
		}
	}
}

void Lane::LoadMusic(int ID, int difficulty) {
	//一度リセット
	accuracyCounter = 0;
	rate = 0;
	score = 0;
	notesCounter = 0;
	isClear = false;
	//曲IDを別で格納する（データ保存の際に使用）
	musicID = ID;
	this->difficulty = difficulty;
	//音楽データをコピーする
	playData.beatDenomonator = musicData[ID].beatDenomonator;
	playData.beatMolecule = musicData[ID].beatMolecule;
	playData.BPM = musicData[ID].BPM;
	playData.speed = musicData[ID].speed;
	playData.level[0] = musicData[ID].level[difficulty];
	for (int i = 0; i < layerNum * shiftMaxNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.difficulty[0].layer[i].note[j].chart[k] = musicData[ID].difficulty[difficulty].layer[i].note[j].chart[k];
				//許容の決定
				if (playData.difficulty[0].layer[i].note[j].chart[k] > 0 && playData.difficulty[0].layer[i].note[j].chart[k] < 21) {
					accuracyCounter += 10;
					notesCounter++;
					//rate += 10;
				}
				if (k < drawNotes) {
					playData.difficulty[0].layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.0f - i % 4 * 0.01f, -45.5f + distance * playData.speed);
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
	//難易度に応じてノルマライン変更
	quota = 50 + 10 * difficulty;
	if (difficulty == 3) {	//PRANKだけやりたい放題なのでノルマを別に設定
		quota = 40.0f;
	}

	moveFlag = true;
}

void Lane::ResetMusic() {
	//音楽データ削除
	playData.beatDenomonator = 0;
	playData.beatMolecule = 0;
	playData.BPM = 0;
	playData.speed = 0;
	for (int i = 0; i < layerNum * shiftMaxNum; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < maxNotes; k++) {
				playData.difficulty[0].layer[i].note[j].chart[k] = 0;
				if (k < drawNotes) {
					playData.difficulty[0].layer[i].note[j].hit[k] = false;
					playData.difficulty[0].layer[i].note[j].hitTimer[k] = 60;
					playData.difficulty[0].layer[i].note[j].judgement[k] = 0;
					playData.difficulty[0].layer[i].note[j].worldTransform[k].translation_ = { 0,0,0 };
					playData.difficulty[0].layer[i].note[j].startMove[k] = false;
				}
			}
			playData.difficulty[0].layer[i].note[j].speed = 0;
		}
	}

	//小節線初期化
	line.countFlame = 0;
	line.countRhythm = 1;

	startMusic = 0;
	startTimer = resetStartTimer;
	chartNum = 0;
	shift = 0;

	//小節線
	for (int i = 0; i < line.lineNum; i++) {
		line.linePop[i] = false;
		line.lineWorld[i].translation_ = Vector3(0, 0, 0);
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

void Lane::Judgement() {
	//各ノーツのhit判定
	for (int i = 0; i < layerNum + shift; i++) {
		for (int k = 0; k < drawNotes; k++) {
			//列ごとにhit判定を取る
			//ノーツの種類によって処理の仕方を変える
			if (playData.difficulty[0].layer[i].note[0].type[k] % 4 == 1) { ColumnHit(i, 0, k, input_->TriggerKey(DIK_F), input_->PushKey(DIK_F)); }
			if (playData.difficulty[0].layer[i].note[1].type[k] % 4 == 1) { ColumnHit(i, 1, k, input_->TriggerKey(DIK_G), input_->PushKey(DIK_G)); }
			if (playData.difficulty[0].layer[i].note[2].type[k] % 4 == 1) { ColumnHit(i, 2, k, input_->TriggerKey(DIK_H), input_->PushKey(DIK_H)); }
			if (playData.difficulty[0].layer[i].note[3].type[k] % 4 == 1) { ColumnHit(i, 3, k, input_->TriggerKey(DIK_J), input_->PushKey(DIK_J)); }
			//1/2ノーツ
			if (playData.difficulty[0].layer[i].note[0].type[k] % 4 == 2) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G)));
			}
			if (playData.difficulty[0].layer[i].note[1].type[k] % 4 == 2) {
				ColumnHit(i, 1, k, ThickColumn(input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H)), ThickColumn(input_->PushKey(DIK_G), input_->PushKey(DIK_H)));
			}
			if (playData.difficulty[0].layer[i].note[2].type[k] % 4 == 2) {
				ColumnHit(i, 2, k, ThickColumn(input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
			//3/4ノーツ
			if (playData.difficulty[0].layer[i].note[0].type[k] % 4 == 3) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G), input_->PushKey(DIK_H)));
			}
			if (playData.difficulty[0].layer[i].note[1].type[k] % 4 == 3) {
				ColumnHit(i, 1, k, ThickColumn(input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_G), input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
			//1/1ノーツ
			if (playData.difficulty[0].layer[i].note[0].type[k] % 4 == 0 && playData.difficulty[0].layer[i].note[0].type[k] != 0) {
				ColumnHit(i, 0, k, ThickColumn(input_->TriggerKey(DIK_F), input_->TriggerKey(DIK_G), input_->TriggerKey(DIK_H), input_->TriggerKey(DIK_J)), ThickColumn(input_->PushKey(DIK_F), input_->PushKey(DIK_G), input_->PushKey(DIK_H), input_->PushKey(DIK_J)));
			}
		}
	}
	//hitTimerを基に評価決め
	for (int i = 0; i < layerNum + shift; i++) {
		for (int j = 0; j < columnNum; j++) {
			for (int k = 0; k < drawNotes; k++) {
				if (playData.difficulty[0].layer[i].note[j].hit[k]) {
					//PERFECT判定（6フレーム)
					if (playData.difficulty[0].layer[i].note[j].hitTimer[k] >= lateJudge && playData.difficulty[0].layer[i].note[j].hitTimer[k] <= fastJudge) {
						playData.difficulty[0].layer[i].note[j].judgement[k] = 1;
						if (playData.difficulty[0].layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[0]);
						}
						if (playData.difficulty[0].layer[i].note[j].type[k] < 21) {
							UpdateRate(10);
						}
					}
					//GREAT判定(FAST)（2フレーム）
					else if (playData.difficulty[0].layer[i].note[j].hitTimer[k] > fastJudge && playData.difficulty[0].layer[i].note[j].hitTimer[k] <= fastJudge + 2) {
						playData.difficulty[0].layer[i].note[j].judgement[k] = 2;
						if (playData.difficulty[0].layer[i].note[j].type[k] < 9) {
							audioSE->PlayWave(SE[1]);
						}
						if (playData.difficulty[0].layer[i].note[j].type[k] > 8 && playData.difficulty[0].layer[i].note[j].type[k] < 21) {
							//HOLDのみFAST判定無し（始点除く）
							playData.difficulty[0].layer[i].note[j].judgement[k] = 1;
							UpdateRate(10);
							if (playData.difficulty[0].layer[i].note[j].type[k] < 17) {
								audioSE->PlayWave(SE[0]);
							}
						}
						else if (playData.difficulty[0].layer[i].note[j].type[k] < 21) { UpdateRate(5); }
					}
					//GREAT判定(LATE)（2フレーム）
					else if (playData.difficulty[0].layer[i].note[j].hitTimer[k] < lateJudge && playData.difficulty[0].layer[i].note[j].hitTimer[k] >= lateJudge - 2) {
						playData.difficulty[0].layer[i].note[j].judgement[k] = 2;
						if (playData.difficulty[0].layer[i].note[j].type[k] < 17) {
							audioSE->PlayWave(SE[1]);
						}
						if (playData.difficulty[0].layer[i].note[j].type[k] < 21) {
							UpdateRate(5);
						}
					}
					//MISS判定(FAST)（1フレーム）
					else if (playData.difficulty[0].layer[i].note[j].hitTimer[k] > fastJudge + 2 && playData.difficulty[0].layer[i].note[j].hitTimer[k] <= fastJudge + 3) {
						playData.difficulty[0].layer[i].note[j].judgement[k] = 3;
						if (playData.difficulty[0].layer[i].note[j].type[k] > 8 && playData.difficulty[0].layer[i].note[j].type[k] < 21) {
							if (playData.difficulty[0].layer[i].note[j].type[k] < 17) {
								audioSE->PlayWave(SE[0]);
							}
							//HOLDのみFAST判定無し（始点除く）
							playData.difficulty[0].layer[i].note[j].judgement[k] = 1;
							UpdateRate(10);
						}
						else if (playData.difficulty[0].layer[i].note[j].type[k] < 21) {
							UpdateRate(0);
							audioSE->PlayWave(SE[2]);
						}
					}
				}
				//MISS判定（スルー判定）
				if (playData.difficulty[0].layer[i].note[j].hitTimer[k] < lateJudge - 2) {
					playData.difficulty[0].layer[i].note[j].judgement[k] = 3;
					playData.difficulty[0].layer[i].note[j].hit[k] = true;
					if (playData.difficulty[0].layer[i].note[j].type[k] < 21) {
						UpdateRate(0);
					}
				}
				//評価に応じてカウントする値を変動させる
				//HOLDの中間だけはスルーさせる
				if (playData.difficulty[0].layer[i].note[j].type[k] < 21) {
					if (playData.difficulty[0].layer[i].note[j].judgement[k] == 1) {
						combo++;
						perfect++;
						notesCounter--;
						//MAXコンボ確認
						if (maxCombo < combo) {
							maxCombo = combo;
						}
						//break;
					}
					else if (playData.difficulty[0].layer[i].note[j].judgement[k] == 2) {
						combo++;
						great++;
						notesCounter--;
						//MAXコンボ確認
						if (maxCombo < combo) {
							maxCombo = combo;
						}
						//break;
					}
					else if (playData.difficulty[0].layer[i].note[j].judgement[k] == 3) {
						//MISSしたらコンボカウントを0にする
						combo = 0;
						miss++;
						notesCounter--;
						//break;
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
		if (!playData.difficulty[0].layer[layer].note[columnNum].hit[notes] && playData.difficulty[0].layer[layer].note[columnNum].type[notes] > 0) {
			if (trigger) {
				//TAP系列のみ（HOLD始点はTAP判定
				if (playData.difficulty[0].layer[layer].note[columnNum].hitTimer[notes] <= fastJudge + 3) {
					if (playData.difficulty[0].layer[layer].note[columnNum].type[notes] < 9) {
						playData.difficulty[0].layer[layer].note[columnNum].hit[notes] = true;
					}
				}
			}
			//長押し
			if (push) {
				//長押し状態のHOLDのFAST判定をなくす
				if (playData.difficulty[0].layer[layer].note[columnNum].hitTimer[notes] <= 0) {
					//HOLD系列
					if (playData.difficulty[0].layer[layer].note[columnNum].type[notes] > 8) {
						playData.difficulty[0].layer[layer].note[columnNum].hit[notes] = true;
					}
				}
			}
		}
	}
	else if (autoPlay) {
		if (playData.difficulty[0].layer[layer].note[columnNum].hitTimer[notes] == 0) {
			playData.difficulty[0].layer[layer].note[columnNum].hit[notes] = true;
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
			//使われていない小節線をオンにする（全てのノーツが通過し終えた場合生成しない）
			for (int i = 0; i < line.lineNum; i++) {
				if (!line.linePop[i] && notesCounter > 0) {
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
	for (int i = 0 + shift; i < layerNum + shift; i++) {	//レイヤー数
		for (int j = 0; j < columnNum; j++) {	//列数
			if (playData.difficulty[0].layer[i].note[j].chart[chartNum] > 0 && playData.difficulty[0].layer[i].note[j].chart[chartNum] < 17) {	//ノーツの有無チェック
				for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
					if (!playData.difficulty[0].layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.difficulty[0].layer[i].note[j].chart[chartNum]);
						break;											//空きが見つかったら即脱出
					}
				}
			}
			else if (playData.difficulty[0].layer[i].note[j].chart[chartNum] > 16 && playData.difficulty[0].layer[i].note[j].chart[chartNum] != 99) {	//ノーツの有無チェック
				for (int k = 0; k < drawNotes; k++) {	//空いてる順からフラグをオンにする
					if (!playData.difficulty[0].layer[i].note[j].startMove[k]) {
						SetNote(i, j, k, playData.difficulty[0].layer[i].note[j].chart[chartNum]);
						playData.difficulty[0].layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.01f - 4 * 0.01f, -45.5f + distance * playData.speed);
						break;											//空きが見つかったら即脱出
					}
				}
			}
		}
	}

	for (int i = 0; i < layerNum + shift; i++) {	//レイヤー数
		for (int j = 0; j < columnNum; j++) {	//列数
			for (int k = 0; k < drawNotes; k++) {	//表示可能ノーツ数
				if (playData.difficulty[0].layer[i].note[j].startMove[k]) {	//スタートフラグオンなら
					playData.difficulty[0].layer[i].note[j].worldTransform[k].translation_.z -= speed;	//譜面速度に合わせて移動
					//ノーツが一定ラインを通り過ぎるかヒット判定がオンでリセット
					if (playData.difficulty[0].layer[i].note[j].worldTransform[k].translation_.z < -50.0f || playData.difficulty[0].layer[i].note[j].hit[k]) {
						playData.difficulty[0].layer[i].note[j].worldTransform[k].translation_ = Vector3(0.05f + j * 1.135f, -2.0f - i % 4 * 0.01f, -45.5f + distance * playData.speed);
						playData.difficulty[0].layer[i].note[j].hit[k] = false;
						playData.difficulty[0].layer[i].note[j].hitTimer[k] = 60;
						playData.difficulty[0].layer[i].note[j].type[k] = 0;
						playData.difficulty[0].layer[i].note[j].startMove[k] = false;
						playData.difficulty[0].layer[i].note[j].judgement[k] = 0;
					}
					if (!playData.difficulty[0].layer[i].note[j].hit[k]) {
						playData.difficulty[0].layer[i].note[j].hitTimer[k]--;	//タイマーを減らす
					}
				}
			}
		}
	}

	//カウントを進める
	chartNum++;
	//maxまで行ったらリセットして次の全体の行へ
	if (chartNum >= maxNotes) {
		chartNum = 0;
		shift += 4;
	}
}

void Lane::ChartInitialize() {
	//全ての曲データ初期化（四重for文…）
	for (int M = 0; M < musicNum; M++) {								//曲数
		for (int D = 0; D < 4; D++) {									//難易度
			for (int i = 0; i < layerNum * shiftMaxNum; i++) {			//レイヤー数
				for (int j = 0; j < columnNum; j++) {					//列数
					for (int k = 0; k < maxNotes; k++) {				//置けるノーツ数
						musicData[M].difficulty[D].layer[i].note[j].chart[k] = 0;
					}
				}
			}
			musicData[M].BPM = 0;
			musicData[M].beatDenomonator = 0;
			musicData[M].beatMolecule = 0;
			musicData[M].speed = 0;
			musicData[M].level[D] = 0;
			musicData[M].playMusicCount = 0;
		}
	}

	//全ての曲のデータ読み込み
	//譜面データがあるファイルの場所と格納したい配列の番号を指定する
	ID000("Resources/musicData/000/testmc.txt", 0);
	IDEntry(1, "Resources/musicData/001/banbado.txt", "musicData/001/banbado.wav", "Resources/musicData/001/scoredata.txt", 144, 3, 0, 100);
	IDEntry(1, "Resources/musicData/001/banbadoM.txt", "musicData/001/banbado.wav", "Resources/musicData/001/scoredata.txt", 144, 2, 7, 100);
	IDEntry(1, "Resources/musicData/001/banbadoN.txt", "musicData/001/banbado.wav", "Resources/musicData/001/scoredata.txt", 144, 1, 5, 100);
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

	LoadData(musicID, 0, filePass);
}

void Lane::IDEntry(int musicID, std::string filePass, std::string musicPass, std::string scoreDataPass, int BPM, int difficultyNum, int level, int startMusicCount, int beatDenomonator, int beatMolecule, int speed) {
	//分母
	musicData[musicID].beatDenomonator = beatDenomonator;
	//分子
	musicData[musicID].beatMolecule = beatMolecule;
	//BPM
	musicData[musicID].BPM = BPM;
	//譜面速度（倍率）
	musicData[musicID].speed = speed;
	//レベル
	musicData[musicID].level[difficultyNum] = level;
	//開始タイミング
	musicData[musicID].playMusicCount = startMusicCount;
	//スコアデータパス
	musicData[musicID].dataPass = scoreDataPass;

	LoadData(musicID, difficultyNum, filePass);

	//music[musicID] = audioMusic->LoadWave(musicPass);
}

void Lane::LoadData(int ID, int difficulty, string filePass) {

	//入力されたファイルパスからファイルオブジェクト作成
	ifstream ifs(filePass);

	//開かなかったらエラー
	if (!ifs) {
		cout << "ファイルを開けませんでした" << endl;
		return;
	}
	/*if (!ifs2) {
		cout << "ファイルを開けませんでした" << endl;
		return;
	}*/

	string str = "";

	//レイヤー
	int i = 0;
	//列
	int j = 0;
	//置けるノーツ数
	int k = 0;
	//この関数専用のずらし
	int shift_ = 0;

	//ファイルの中身を一行ずつ読み込む
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		//区切り文字がなくなるまで文字を区切る
		while (getline(stream, tmp, ',')) {
			musicData[ID].difficulty[difficulty].layer[i + shift_].note[j].chart[k] = atoi(tmp.c_str());
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
		//一個分全て読み込んだらずらして別の列に移動
		if (i > 3) {
			shift_ += 4;
			i = 0;
		}
	}
	ifstream ifs2(musicData[ID].dataPass);
	//格納先の変数
	int saveNum = 0;

	string str2 = "";

	while (getline(ifs2, str2)) {
		string tmp = "";
		istringstream stream(str2);
		int i = 0;

		while (getline(stream, tmp, ',')) {
			if (i == 0) { musicData[ID].difficulty[saveNum].maxScore = atoi(tmp.c_str()); }
			else if (i == 1) { musicData[ID].difficulty[saveNum].maxRankNum = atoi(tmp.c_str()); }
			else if (i == 2) { musicData[ID].difficulty[saveNum].isFCAP = atoi(tmp.c_str()); }
			else if (i == 3) { musicData[ID].difficulty[saveNum].clear = atoi(tmp.c_str()); }
			i++;
		}
		saveNum++;
	}
}

void Lane::SetNote(int i, int j, int k, int typeNum) {
	playData.difficulty[0].layer[i].note[j].startMove[k] = true;	//開始フラグオン
	playData.difficulty[0].layer[i].note[j].type[k] = typeNum;			//ノーツの種類設定
	//playData.difficulty[0].layer[i].note[j].hitTimer[k] = 60;		//判定カウントの設定
}

bool Lane::ThickColumn(bool key1, bool key2, bool key3, bool key4) {
	if (key1 || key2 || key3 || key4) {
		return true;
	}
	return false;
}

void Lane::UpdateRate(int RateScore) {
	//評価ごとに応じたスコアを足す
	//rate += RateScore - 10;
	rate += RateScore;
	//割合計算
	averageRate = rate / accuracyCounter * 100;
	score = averageRate * 10000;
	//平均値に応じてランク変動
	if (averageRate >= 99.80) {
		rank = "V";
		rankNum = 9;
	}
	else if (averageRate >= 99.60) {
		rank = "S";
		rankNum = 8;
	}
	else if (averageRate >= 99.30) {
		rank = "AAA";
		rankNum = 7;
	}
	else if (averageRate >= 99.00) {
		rank = "AA";
		rankNum = 6;
	}
	else if (averageRate >= 98.00) {
		rank = "A";
		rankNum = 5;
	}
	else if (averageRate >= 95.00) {
		rank = "BBB";
		rankNum = 4;
	}
	else if (averageRate >= 90.00) {
		rank = "BB";
		rankNum = 3;
	}
	else if (averageRate >= 70.00) {
		rank = "B";
		rankNum = 2;
	}
	else if (averageRate >= 45.00) {
		rank = "C";
		rankNum = 1;
	}
	else {
		rank = "D";
		rankNum = 0;
	}
}

void Lane::FinishMusic(int& scene) {
	//小節線がこれ以上生成されないようにする
	moveFlag = false;
	if (!autoPlay) {
		//クリアの確認
		if (quota <= averageRate) {
			isClear = true;
			if (!musicData[musicID].difficulty[difficulty].clear) {
				musicData[musicID].difficulty[difficulty].clear = true;
			}
		}

		//スコア更新の確認
		if (musicData[musicID].difficulty[difficulty].maxScore < score) {
			musicData[musicID].difficulty[difficulty].maxScore = score;
		}
		//ランク更新の確認
		if (musicData[musicID].difficulty[difficulty].maxRankNum < rankNum) {
			musicData[musicID].difficulty[difficulty].maxRankNum = rankNum;
		}
		//FCAPチェック
		if (miss == 0) {
			if (musicData[musicID].difficulty[difficulty].isFCAP < 1) {
				if (great == 0) {
					if (musicData[musicID].difficulty[difficulty].isFCAP < 2) {
						musicData[musicID].difficulty[difficulty].isFCAP = 2;
					}
				}
				else {
					musicData[musicID].difficulty[difficulty].isFCAP = 1;
				}
			}
		}


		ofstream ofs(musicData[musicID].dataPass);
		for (int i = 0; i < 4; i++) {
			ofs << musicData[musicID].difficulty[i].maxScore << ",";
			ofs << musicData[musicID].difficulty[i].maxRankNum << ",";
			ofs << musicData[musicID].difficulty[i].isFCAP << ",";
			ofs << musicData[musicID].difficulty[i].clear << endl;
		}
	}

	ResetMusic();
	miss = 0;
	great = 0;
	perfect = 0;
	endTimer = resetEndTimer;
	combo = 0;
	maxCombo = 0;
	averageRate = 0;
	audioMusic->StopWave(music[musicID]);
	quota = 0;
	LoadMusic(musicID, difficulty);
	scene = 1;
}

//スプライト描画※長くなりそうなのでこっちに
void Lane::DrawSprite() {
	//上からPERFECT数、GREAT数、MISS数、MAXCOMBO数
	uiDataSprite[0]->Draw();
	DrawSprite4(maxCombo, uiDataNumbers, 0, true);
	uiDataSprite[1]->Draw();
	DrawSprite4(perfect, uiDataNumbers, 4, true);
	uiDataSprite[2]->Draw();
	DrawSprite4(great, uiDataNumbers, 8, true);
	uiDataSprite[3]->Draw();
	DrawSprite4(miss, uiDataNumbers, 12, true);
	//スコア
	uiDataSprite[4]->Draw();
	int getNum = score;
	bool isZero[7] = { 0,0,0,0,0,0,0 };
	if (getNum / 1000000 > 0) {
		uiScore[0][getNum / 1000000]->Draw();
		uiScore[1][10]->Draw();
		isZero[0] = true;
	}
	else { isZero[0] = false; }
	getNum = getNum % 1000000;
	if (isZero[0] || getNum / 100000 > 0) {
		uiScore[2][getNum / 100000]->Draw();
		isZero[1] = true;
	}
	else { isZero[1] = false; }
	getNum = getNum % 100000;
	if (isZero[1] || getNum / 10000 > 0) {
		uiScore[3][getNum / 10000]->Draw();
		isZero[2] = true;
	}
	else { isZero[2] = false; }
	getNum = getNum % 10000;
	if (isZero[2] || getNum / 1000 > 0) {
		uiScore[4][getNum / 1000]->Draw();
		uiScore[5][10]->Draw();
		isZero[3] = true;
	}
	else { isZero[3] = false; }
	getNum = getNum % 1000;
	if (isZero[3] || getNum / 100 > 0) {
		uiScore[6][getNum / 100]->Draw();
		isZero[4] = true;
	}
	else { isZero[4] = false; }
	getNum = getNum % 100;
	if (isZero[4] || getNum / 10 > 0) {
		uiScore[7][getNum / 10]->Draw();
		isZero[5] = true;
	}
	else { isZero[5] = false; }
	getNum = getNum % 10;
	uiScore[8][getNum / 1]->Draw();
	isZero[6] = true;

	//コンボ
	bool comboIsZero[4] = { false,false,false,false };
	int getCombo = combo;
	if(combo > 9){
		comboSprite->Draw();
		if (getCombo / 1000 > 0) {
			comboIsZero[0] = true;
			comboNumSprite[0][getCombo / 1000]->SetPosition({ comboNumPosX - 120,comboNumPosY });
			comboNumSprite[0][getCombo / 1000]->Draw();
		}
		else { comboIsZero[0] = false; }
		getCombo = getCombo % 1000;
		if (comboIsZero[0] || getCombo / 100 > 0) {
			if (comboIsZero[0]) {
				comboNumSprite[1][getCombo / 100]->SetPosition({ comboNumPosX - 40,comboNumPosY });
			}
			else {
				comboNumSprite[1][getCombo / 100]->SetPosition({ comboNumPosX - 80,comboNumPosY });
			}
			comboIsZero[1] = true;
			comboNumSprite[1][getCombo / 100]->Draw();
		}
		else { comboIsZero[1] = false; }
		getCombo = getCombo % 100;
		if (comboIsZero[1] || getCombo / 10 > 0) {
			if (comboIsZero[0]) {
				comboNumSprite[2][getCombo / 10]->SetPosition({ comboNumPosX + 40,comboNumPosY });
			}
			else if (comboIsZero[1]) {
				comboNumSprite[2][getCombo / 10]->SetPosition({ comboNumPosX,comboNumPosY });
			}
			else {
				comboNumSprite[2][getCombo / 10]->SetPosition({ comboNumPosX - 40,comboNumPosY });
			}
			comboIsZero[2] = true;
			comboNumSprite[2][getCombo / 10]->Draw();
		}
		getCombo = getCombo % 10;
		if (comboIsZero[2] || getCombo / 1 > 0) {
			if (comboIsZero[0]) {
				comboNumSprite[3][getCombo / 1]->SetPosition({ comboNumPosX + 120,comboNumPosY });
			}
			else if (comboIsZero[1]) {
				comboNumSprite[3][getCombo / 1]->SetPosition({ comboNumPosX + 80,comboNumPosY });
			}
			else {
				comboNumSprite[3][getCombo / 1]->SetPosition({ comboNumPosX + 40,comboNumPosY });
			}
			comboIsZero[3] = true;
			comboNumSprite[3][getCombo / 1]->Draw();
		}
	}
}

void Lane::DrawSprite4(int num, Sprite* sprite[16][10], int startNum, bool draw0) {
	bool isZero[4] = { 0,0,0,0 };
	int getNum = num;
	if (getNum / 1000 > 0) {
		sprite[0 + startNum][getNum / 1000]->Draw();
		isZero[0] = true;
	}
	else { isZero[0] = false; }
	getNum = getNum % 1000;
	if (isZero[0] || getNum / 100 > 0) {
		sprite[1 + startNum][getNum / 100]->Draw();
		isZero[1] = true;
	}
	else { isZero[1] = false; }
	getNum = getNum % 100;
	if (isZero[1] || getNum / 10 > 0) {
		sprite[2 + startNum][getNum / 10]->Draw();
		isZero[2] = true;
	}
	else { isZero[2] = false; }
	getNum = getNum % 10;
	if (draw0 || getNum / 1 > 0) {
		sprite[3 + startNum][getNum / 1]->Draw();
		isZero[3] = true;
	}
}