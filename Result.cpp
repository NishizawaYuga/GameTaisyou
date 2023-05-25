#include "Result.h"

void Result::Initialize() {
	//リザルトデータ
	resultData.score = 0;
	resultData.hiScore = 0;
	resultData.rank = 0;
	resultData.isFCAP = 0;
	resultData.isClear = false;
	resultData.perfect = 0;
	resultData.great = 0;
	resultData.miss = 0;
	resultData.Pfast = 0;
	resultData.Plate = 0;
	resultData.Gfast = 0;
	resultData.Glate = 0;
	resultData.maxCombo = 0;

	//スコア&ハイスコア
	float posX = 0.0f;
	for (int i = 0; i < 11; i++) {
		resultScoreTex[i] = TextureManager::Load("font/font_dark" + std::to_string(i) + ".png");
		resultHiScoreTex[i] = TextureManager::Load("ui/playdata/playfont_s" + std::to_string(i) + ".png");
	}
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 9; j++) {
			resultScoreSp[j][i] = nullptr;
			resultScoreSp[j][i] = Sprite::Create(resultScoreTex[i], { 20.0f + posX,460 });
			if (j == 0 || j == 1 || j == 4 || j == 5) {
				posX += 55.0f;
			}
			else { posX += 85.0f; }
		}
		posX = 0.0f;
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 11; j++) {
			resultHiScoreSp[i][j] = nullptr;
			resultHiScoreSp[i][j] = Sprite::Create(resultHiScoreTex[j], { 80.0f + i * 45.0f,400.0f });
		}
	}
	//更新差分
	newlavel = TextureManager::Load("ui/plate/new.png");
	newlavelSp = Sprite::Create(newlavel, { 400.0f,390.0f });
	//ランク
	for (int i = 0; i < 10; i++) {
		resultRankTex[i] = TextureManager::Load("ui/rank" + std::to_string(i) + ".png");
		resultRankSp[i] = nullptr;
		resultRankSp[i] = Sprite::Create(resultRankTex[i], { 1200,374 });
	}
	//プレート
	for (int i = 0; i < 4; i++) {
		plateTex[i] = TextureManager::Load("ui/plate/plate" + std::to_string(i) + ".png");
		plateSp[i] = Sprite::Create(plateTex[i], { 80.0f + i / 2 * 501.0f,630.0f });
	}
	//内訳（PERFECT/GREAT/MISS/MAXCOMBO）
	for (int i = 0; i < 6; i++) {
		uiData[i] = TextureManager::Load("ui/playdata/datafont" + std::to_string(i) + ".png");
		uiDataSprite[i] = Sprite::Create(uiData[i], { 730.0f,400.0f + 55.0f * i });
		uiDataSprite[i]->SetSize({ 300.0f,60.0f });
	}
	uiDataSprite[4]->SetPosition({ 80.0f,340.0f });
	uiDataSprite[5]->SetPosition({ 1280.0f,340.0f });
	for (int i = 0; i < 10; i++) {
		uiDataNum[i] = TextureManager::Load("ui/playdata/playfont_s" + std::to_string(i) + ".png");
	}
	for (int i = 0; i < 16; i++) {
		int j = i / 4;
		for (int k = 0; k < 10; k++) {
			uiDataNumbers[i][k] = nullptr;
			uiDataNumbers[i][k] = Sprite::Create(uiDataNum[k], { 1030.0f + i % 4 * 55.0f,400.0f + 55.0f * j });
		}
	}
	//背景
	resultBackGroundTex[0] = TextureManager::Load("background/result_clear.png");
	resultBackGroundTex[1] = TextureManager::Load("background/result_failed.png");
	resultBackGroundTex[2] = TextureManager::Load("background/lavel.png");
	resultBackGroundSp[0] = Sprite::Create(resultBackGroundTex[0], { 0,0 });
	resultBackGroundSp[1] = Sprite::Create(resultBackGroundTex[1], { 0,0 });
	resultBackGroundSp[2] = Sprite::Create(resultBackGroundTex[2], { 0,-30.0f });

	input_ = Input::GetInstance();

	//TestResult();
}

void Result::Update(int &scene) {
	//ハイスコア更新をしていた時の処理
	if (resultData.score > resultData.hiScore) {
		checkUpdate = true;
		resultData.hiScore = resultData.score;
	}
	if (stopTimer > 0) {
		stopTimer--;
	}
	else {
		if (input_->TriggerKey(DIK_SPACE)) {
			stopTimer = 180;
			scene = 1;
		}
	}
}

void Result::Draw() {
	//背景
	if (resultData.isClear) {
		resultBackGroundSp[0]->Draw();
	}
	else {
		resultBackGroundSp[1]->Draw();
	}
	resultBackGroundSp[2]->Draw();
	//スコア
	int getNum = resultData.score;
	//1桁ずつ表示
	if (getNum / 1000000 > 0) {
		resultScoreSp[0][getNum / 1000000]->Draw();
		resultScoreSp[1][10]->Draw();
		isZeroResult[0] = true;
	}
	else { isZeroResult[0] = false; }
	getNum = getNum % 1000000;
	if (isZeroResult[0] || getNum / 100000 > 0) {
		resultScoreSp[2][getNum / 100000]->Draw();
		isZeroResult[1] = true;
	}
	else { isZeroResult[1] = false; }
	getNum = getNum % 100000;
	if (isZeroResult[1] || getNum / 10000 > 0) {
		resultScoreSp[3][getNum / 10000]->Draw();
		isZeroResult[2] = true;
	}
	else { isZeroResult[2] = false; }
	getNum = getNum % 10000;
	if (isZeroResult[2] || getNum / 1000 > 0) {
		resultScoreSp[4][getNum / 1000]->Draw();
		resultScoreSp[5][10]->Draw();
		isZeroResult[3] = true;
	}
	else { isZeroResult[3] = false; }
	getNum = getNum % 1000;
	if (isZeroResult[3] || getNum / 100 > 0) {
		resultScoreSp[6][getNum / 100]->Draw();
		isZeroResult[4] = true;
	}
	else { isZeroResult[4] = false; }
	getNum = getNum % 100;
	if (isZeroResult[4] || getNum / 10 > 0) {
		resultScoreSp[7][getNum / 10]->Draw();
		isZeroResult[5] = true;
	}
	else { isZeroResult[5] = false; }
	getNum = getNum % 10;
	resultScoreSp[8][getNum / 1]->Draw();
	isZeroResult[6] = true;
	//ハイスコア	
	int getHiNum = resultData.hiScore;
	bool isZero[7] = { 0,0,0,0,0,0,0 };
	if (getHiNum / 1000000 > 0) {
		resultHiScoreSp[0][getHiNum / 1000000]->Draw();
		resultHiScoreSp[1][10]->Draw();
		isZero[0] = true;
	}
	else { isZero[0] = false; }
	getHiNum = getHiNum % 1000000;
	if (isZero[0] || getHiNum / 100000 > 0) {
		resultHiScoreSp[2][getHiNum / 100000]->Draw();
		isZero[1] = true;
	}
	else { isZero[1] = false; }
	getHiNum = getHiNum % 100000;
	if (isZero[1] || getHiNum / 10000 > 0) {
		resultHiScoreSp[3][getHiNum / 10000]->Draw();
		isZero[2] = true;
	}
	else { isZero[2] = false; }
	getHiNum = getHiNum % 10000;
	if (isZero[2] || getHiNum / 1000 > 0) {
		resultHiScoreSp[4][getHiNum / 1000]->Draw();
		resultHiScoreSp[5][10]->Draw();
		isZero[3] = true;
	}
	else { isZero[3] = false; }
	getHiNum = getHiNum % 1000;
	if (isZero[3] || getHiNum / 100 > 0) {
		resultHiScoreSp[6][getHiNum / 100]->Draw();
		isZero[4] = true;
	}
	else { isZero[4] = false; }
	getHiNum = getHiNum % 100;
	if (isZero[4] || getHiNum / 10 > 0) {
		resultHiScoreSp[7][getHiNum / 10]->Draw();
		isZero[5] = true;
	}
	else { isZero[5] = false; }
	getHiNum = getHiNum % 10;
	resultHiScoreSp[8][getHiNum / 1]->Draw();
	isZero[6] = true;
	//更新差分
	if (checkUpdate) {
		newlavelSp->Draw();
	}

	//ランク
	resultRankSp[resultData.rank]->Draw();
	//プレート
	if (resultData.isClear) {
		plateSp[1]->Draw();	//クリア
	}
	else {
		plateSp[0]->Draw();	//ノルマクリア失敗
	}
	if (resultData.isFCAP == 1) {
		plateSp[2]->Draw();
	}
	else if (resultData.isFCAP == 2) {
		plateSp[3]->Draw();
	}
	//内訳
	//上からPERFECT数、GREAT数、MISS数、MAXCOMBO数
	uiDataSprite[0]->Draw();
	DrawSprite4(resultData.maxCombo, uiDataNumbers, 0, true);
	uiDataSprite[1]->Draw();
	DrawSprite4(resultData.perfect, uiDataNumbers, 4, true);
	uiDataSprite[2]->Draw();
	DrawSprite4(resultData.great, uiDataNumbers, 8, true);
	uiDataSprite[3]->Draw();
	DrawSprite4(resultData.miss, uiDataNumbers, 12, true);
	uiDataSprite[4]->Draw();
	uiDataSprite[5]->Draw();
}

void Result::IncludeResultData(ResultData resultdata) {
	//受け取ったデータを格納
	resultData = resultdata;
}

void Result::TestResult() {
	resultData.score = 1000000;
	resultData.hiScore = 1000000;
	resultData.rank = 4;
	resultData.isFCAP = 1;
	resultData.isClear = false;
	resultData.perfect = 999;
	resultData.great = 999;
	resultData.miss = 999;
	resultData.Pfast = 999;
	resultData.Plate = 999;
	resultData.Gfast = 999;
	resultData.Glate = 999;
	resultData.maxCombo = 9999;
	checkUpdate = true;
}

void Result::DrawSprite4(int num, Sprite* sprite[16][10], int startNum, bool draw0) {
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

void Result::DeleteResult() {
	resultData.score = 0;
	resultData.hiScore = 0;
	resultData.rank = 0;
	resultData.isFCAP = 0;
	resultData.isClear = false;
	resultData.perfect = 0;
	resultData.great = 0;
	resultData.miss = 0;
	resultData.Pfast = 0;
	resultData.Plate = 0;
	resultData.Gfast = 0;
	resultData.Glate = 0;
	resultData.maxCombo = 0;
	checkUpdate = false;
}