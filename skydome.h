#pragma once
#include "DirectXCommon.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Skydome {
public:
	//初期化
	void Initialize(Model* model);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
private:
	//ワールド座標
	WorldTransform worldTransform_;
	//モデル
	Model* model = nullptr;

};
