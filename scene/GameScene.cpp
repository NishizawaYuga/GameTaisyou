#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete laneModel;
	delete lane;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//モデル
	for (int i = 0; i < 12; i++) {
		notesModel[i] = nullptr;
	}
	laneModel = Model::CreateFromOBJ("lane", true);					//レーン
	lineModel = Model::CreateFromOBJ("line", true);					//小節線
	notesModel[0] = Model::CreateFromOBJ("tap", true);			//TAP
	notesModel[1] = Model::CreateFromOBJ("tap_wide01", true);	//TAP（2分の1サイズ）
	notesModel[2] = Model::CreateFromOBJ("tap_wide02", true);	//TAP（4分の3サイズ）
	notesModel[3] = Model::CreateFromOBJ("tap_wide03", true);	//TAP（1/1サイズ）
	notesModel[4] = Model::CreateFromOBJ("hold", true);			//HOLD（始点）
	notesModel[5] = Model::CreateFromOBJ("hold_wide01", true);	//HOLD（始点/2分の1サイズ）
	notesModel[6] = Model::CreateFromOBJ("hold_wide02", true);	//HOLD（始点/4分の3サイズ）
	notesModel[7] = Model::CreateFromOBJ("hold_wide03", true);	//HOLD（始点/1/1サイズ）
	notesModel[8] = Model::CreateFromOBJ("hold_while00", true);	//HOLD（間）
	notesModel[9] = Model::CreateFromOBJ("hold_while01", true);	//HOLD（間/2分の1サイズ）
	notesModel[10] = Model::CreateFromOBJ("hold_while02", true);//HOLD（間/4分の3サイズ）
	notesModel[11] = Model::CreateFromOBJ("hold_while03", true);//HOLD（間/1/1サイズ）

	skydomeModel = Model::CreateFromOBJ("skydome", true);

	//レーン初期化
	lane = new Lane();
	//lane->Initialize(laneModel,lineModel);
	lane->Initialize(laneModel, lineModel, notesModel);

	//select画面初期化
	select = new Select();
	select->Initialize();

	//skydome初期化
	skydome = new Skydome();
	skydome->Initialize(skydomeModel);

	options.autoPlay = false;
	options.border = 0;
	options.speed = 3;
	options.style = 0;
	options.wall = 0;

	//シーン番号
	//(0 : タイトル、1 : 選曲画面、2 : プレイ画面、ととりあえず仮定)
	sceneNum = 1;

	//曲IDと難易度
	difficulty = 0;
	musicID = 1;
	oldDiff = difficulty;
	oldID = musicID;

	//オプションデータ
	options.autoPlay = false;
	options.border = 0;
	options.speed = 0.3f;
	options.style = 0;
	options.wall = 0;
	oldOptions = options;

	viewProjection.Initialize();
	viewProjection.target.y = -30.0f;
	viewProjection.eye.x += 0.025;
	viewProjection.eye.y = 2.0f;
	viewProjection.eye.z += 1.0f;
}

void GameScene::Update() {
	if (sceneNum == 1) {
		//変更前のID番号と難易度番号を格納
		oldID = musicID;
		oldDiff = difficulty;
		//変更前のオプション設定格納
		oldOptions = options;
		select->Update(sceneNum, musicID, difficulty,options);
		//どちらか一致していなかったら譜面更新
		if (oldID != musicID || oldDiff != difficulty) {
			lane->LoadMusic(musicID, difficulty);
		}
		//何かしら一致していなかったら更新
		if (oldOptions.autoPlay != options.autoPlay) {
			lane->Auto(options.autoPlay);
		}
		if (oldOptions.speed < options.speed) {
			lane->ChangeSpeed(0.1f);
			lane->LoadMusic(musicID, difficulty);
		}
		else if (oldOptions.speed > options.speed) {
			lane->ChangeSpeed(-0.1f);
			lane->LoadMusic(musicID, difficulty);
		}
		if (oldOptions.style != options.style) {
			lane->ChangeStyle(options.style);
		}
	}
	else if (sceneNum == 2) {
		lane->Update(sceneNum);
		skydome->Update();
	}

	viewProjection.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	if (sceneNum == 2) {
		lane->Draw(viewProjection);
		skydome->Draw(viewProjection);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	if (sceneNum == 1) {
		select->Draw();
		select->SelectDrawData(lane->GetHiScore(musicID, difficulty), lane->GetHiRank(musicID, difficulty), lane->GetFCAPFlag(musicID, difficulty), lane->GetClear(musicID, difficulty),
			difficulty, lane->GetLevel(musicID, difficulty),musicID);
	}
	if (sceneNum == 2) {
		lane->DrawSprite();
	}
	if (sceneNum != 0) {
		select->DrawDifficulty(difficulty, lane->GetLevel(musicID, difficulty));
	}


	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
