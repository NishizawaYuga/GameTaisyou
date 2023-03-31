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

	//レーン初期化
	lane = new Lane();
	//lane->Initialize(laneModel,lineModel);
	lane->Initialize(laneModel, lineModel,notesModel);

	//lane->LoadMusic(1);


	countFlame = 0;
	countRhythm = 0;
	BPM = 222;

	change = baseBPM / BPM;

	viewProjection.Initialize();
	viewProjection.target.y = -30.0f;
	viewProjection.eye.x += 0.025;
	viewProjection.eye.y = 2.0f;
	viewProjection.eye.z += 1.0f;
}

void GameScene::Update() {

	lane->Update();

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
	
	lane->Draw(viewProjection);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
