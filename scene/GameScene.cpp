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

	laneModel = Model::CreateFromOBJ("lane", true);
	lineModel = Model::CreateFromOBJ("line", true);//テクスチャがないのでエラー吐きます

	//レーン初期化
	lane = new Lane();
	//lane->Initialize(laneModel,lineModel);
	lane->Initialize(laneModel, lineModel);

	countFlame = 0;
	countRhythm = 0;
	BPM = 222;

	change = baseBPM / BPM;

	viewProjection.Initialize();
	viewProjection.target.y = -30.0f;
	viewProjection.eye.x += 0.025;
	viewProjection.eye.y = 2.0f;
	viewProjection.eye.z += 1.5f;
}

void GameScene::Update() {
	

	int changeCount = baseBPM * change;
	if (countFlame >= changeCount) {
		countRhythm++;
		countFlame = 0;
	}
	countFlame++;

	debugText_->SetPos(10, 10);
	debugText_->Printf("BPM : %d", BPM);
	debugText_->SetPos(10, 30);
	debugText_->Printf("Rhythm : %d", countRhythm);
	debugText_->SetPos(10, 50);
	debugText_->Printf("countFlame : %d", countFlame);
	debugText_->SetPos(10, 70);
	debugText_->Printf("change : %f", change);

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
