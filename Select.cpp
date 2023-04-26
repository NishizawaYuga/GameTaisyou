#include "Select.h"
#include "TextureManager.h"
#include <cassert>

Select::Select() {}

Select::~Select()
{
	for (size_t i = 0; i < _countof(spritesong_); i++)
	{
		delete spritesong_[i];
	}
	delete spriteframe_;
	delete spriteBG_;

}

void Select::Initialize() {

	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();


	{//曲の選択中の上下運動のタイマー
		JumpTimer_ = 30;
		JumpTimer2_ = 30;
		JumpTimer3_ = 30;
		JumpTimer4_ = 30;
		JumpTimer5_ = 30;
	}

	//2
	textureHandleSong2_ = TextureManager::Load("sprite/0002.png");
	//3
	textureHandleSong3_ = TextureManager::Load("sprite/0003.png");
	//4
	textureHandleSong4_ = TextureManager::Load("sprite/0004.png");
	//5
	textureHandleSong5_ = TextureManager::Load("sprite/0005.png");
	//6
	textureHandleSong6_ = TextureManager::Load("sprite/0006.png");
	//7
	textureHandleSong7_ = TextureManager::Load("sprite/0007.png");

	//BG
	textureHandleBG_[0] = TextureManager::Load("sprite/moyou000.png");
	textureHandleBG_[1] = TextureManager::Load("sprite/moyou001.png");
	textureHandleBG_[2] = TextureManager::Load("sprite/moyou002.png");
	textureHandleBG_[3] = TextureManager::Load("sprite/moyou003.png");
	spriteBG_[0] = Sprite::Create(textureHandleBG_[0], { 0,0 });
	spriteBG_[1] = Sprite::Create(textureHandleBG_[1], { 0,0 });
	spriteBG_[2] = Sprite::Create(textureHandleBG_[2], { 0,0 });
	spriteBG_[3] = Sprite::Create(textureHandleBG_[3], { 0,0 });

	float posX = 0.0f;
	//数字
	for (int i = 0; i < 11; i++) {
		textureNum[i] = TextureManager::Load("font/font_dark" + std::to_string(i) + ".png");
		levelNum[i] = TextureManager::Load("ui/level/levelNum" + std::to_string(i) + ".png");
		levelS[i] = nullptr;
		levelS[i] = Sprite::Create(levelNum[i], { 1700,158 });
	}
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 9; j++) {
			spriteNum[j][i] = nullptr;
			spriteNum[j][i] = Sprite::Create(textureNum[i], { 0.0f + posX,750 });
			if (j == 0 || j == 1 || j == 4 || j == 5) {
				posX += 55.0f;
			}
			else { posX += 85.0f; }
		}
		posX = 0.0f;
	}

	//ランク
	for (int i = 0; i < 10; i++) {
		rankTexNum[i] = TextureManager::Load("ui/rank" + std::to_string(i) + ".png");
		rankSprite[i] = nullptr;
		rankSprite[i] = Sprite::Create(rankTexNum[i], { 1200,700 });
	}

	//チップ
	for (int i = 0; i < 3; i++) {
		tip[i] = TextureManager::Load("ui/tip" + std::to_string(i) + ".png");
		tipSprite[i] = nullptr;
		if (i == 0) {
			tipSprite[i] = Sprite::Create(tip[i], { 0.0f + 80.0f,900 });
		}
		else {
			tipSprite[i] = Sprite::Create(tip[i], { 0.0f + 240.0f,900 });
		}
	}

	//難易度
	for (int i = 0; i < 4; i++) {
		diffNum[i] = TextureManager::Load("ui/difficulty/difficulty" + std::to_string(i) + ".png");
		diff[i] = nullptr;
		diff[i] = Sprite::Create(diffNum[i], { 1356,0 });
	}

	//タイトル
	texturehandlTi_ = TextureManager::Load("sprite/titol.png");
	spriteTi_ = Sprite::Create(texturehandlTi_, { 0,0 });
	//シーン切り替え用リソース
	//1
	textureHandleSongPl1_ = TextureManager::Load("sprite/play0001.png");
	spriteSongPl1_ = Sprite::Create(textureHandleSongPl1_, { 0,0 });
	//2
	textureHandleSongPl2_ = TextureManager::Load("sprite/play0002.png");
	spriteSongPl2_ = Sprite::Create(textureHandleSongPl2_, { 0,0 });
	//3
	textureHandleSongPl3_ = TextureManager::Load("sprite/play0003.png");
	spriteSongPl3_ = Sprite::Create(textureHandleSongPl3_, { 0,0 });
	//4
	textureHandleSongPl4_ = TextureManager::Load("sprite/play0004.png");
	spriteSongPl4_ = Sprite::Create(textureHandleSongPl4_, { 0,0 });
	//5
	textureHandleSongPl5_ = TextureManager::Load("sprite/play0005.png");
	spriteSongPl5_ = Sprite::Create(textureHandleSongPl5_, { 0,0 });

	textureHandleSong_ = TextureManager::Load("sprite/0001.png");
	for (size_t i = 0; i < _countof(spritesong_); i++)
	{
		spritesong_[0] = Sprite::Create(textureHandleSong_, { 860,360 });//1
		spritesong_[1] = Sprite::Create(textureHandleSong2_, { 1160,360 });//2
		spritesong_[2] = Sprite::Create(textureHandleSong3_, { 1460,360 });//3
		spritesong_[3] = Sprite::Create(textureHandleSong4_, { 260,360 });//4
		spritesong_[4] = Sprite::Create(textureHandleSong5_, { 560,360 });//5
		//spritesong_[5] = Sprite::Create(textureHandleSong6_, { -60,360 });//6
		//spritesong_[6] = Sprite::Create(textureHandleSong7_, { 1760,360 });//7
	}
	worldTransform_.Initialize();
}

void Select::Update(int& sceneNum, int& musicID, int& difficulty)
{
	std::string test = "あ";
	char test2[] = "a";
	switch (scene)
	{
	case 0:
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene = 1;
		}
		break;
	case 1:
		// ステージセレクト　入力処理
		if (input_->TriggerKey(DIK_F))
		{

			// ステージの番号を300ずらす
			for (size_t i = 0; i < _countof(spritesong_); i++)
			{
				position = spritesong_[i]->GetPosition();
				position.x += 300;
				position.y = 360;
				JumpTimer_ = 30;
				JumpTimer2_ = 30;
				JumpTimer3_ = 30;
				JumpTimer4_ = 30;
				JumpTimer5_ = 30;
				// 1460が右端なのでこれより大きかったら最小値の260の位置にする
				if (position.x > 1460)
				{
					position.x = 260;

				}
				musicID++;
				if (musicID > 5) {
					musicID = 1;
				}

				spritesong_[i]->SetPosition(position);
			}
		}

		if (input_->TriggerKey(DIK_J))
		{

			// ステージの番号を-300ずらす
			for (size_t i = 0; i < _countof(spritesong_); i++)
			{
				position = spritesong_[i]->GetPosition();
				position.x -= 300;
				position.y = 360;
				JumpTimer_ = 30;
				JumpTimer2_ = 30;
				JumpTimer3_ = 30;
				JumpTimer4_ = 30;
				JumpTimer5_ = 30;
				// 260が左端なのでこれより小さかったら最大値の1460の位置にする
				if (position.x <= 250)
				{
					position.x = 1460;
				}
				musicID--;
				if (musicID < 1) {
					musicID = 5;
				}

				spritesong_[i]->SetPosition(position);


			}
		}
		//難易度変更
		if (input_->TriggerKey(DIK_G)) {
			if (difficulty < 3) {
				difficulty++;
				difficultyColor = difficulty;
			}
		}
		else if (input_->TriggerKey(DIK_H)) {
			if (difficulty > 0) {
				difficulty--;
				difficultyColor = difficulty;
			}
		}


		if (input_->TriggerKey(DIK_BACKSPACE))
		{
			scene = 0;
		}

		if (spritesong_[0]->GetPosition().x == 860)
		{

			position = spritesong_[0]->GetPosition();

			if (JumpFlagPlus_)
			{
				JumpTimer_--;
				position.y += 1.0f;
			}
			else if (JumpFlagNegative_)
			{
				JumpTimer_--;
				position.y -= 1.0f;
			}
			if (JumpTimer_ <= 0 && JumpFlagPlus_)
			{
				JumpFlagPlus_ = false;
				JumpFlagNegative_ = true;
				JumpTimer_ = 60;
			}
			else if (JumpTimer_ <= 0 && JumpFlagNegative_)
			{
				JumpFlagPlus_ = true;
				JumpFlagNegative_ = false;
				JumpTimer_ = 60;
			}
			spritesong_[0]->SetPosition(position);


			if (input_->TriggerKey(DIK_SPACE))
			{
				sceneNum = 2;
			}


		}
		if (spritesong_[1]->GetPosition().x == 860)
		{
			position = spritesong_[1]->GetPosition();

			if (JumpFlagPlus_)
			{
				JumpTimer2_--;
				position.y += 1.0f;
			}
			else if (JumpFlagNegative_)
			{
				JumpTimer2_--;
				position.y -= 1.0f;
			}
			if (JumpTimer2_ <= 0 && JumpFlagPlus_)
			{
				JumpFlagPlus_ = false;
				JumpFlagNegative_ = true;
				JumpTimer2_ = 60;
			}
			else if (JumpTimer2_ <= 0 && JumpFlagNegative_)
			{
				JumpFlagPlus_ = true;
				JumpFlagNegative_ = false;
				JumpTimer2_ = 60;
			}
			spritesong_[1]->SetPosition(position);

			if (input_->TriggerKey(DIK_2))
			{
				scene = 3;
			}
		}
		if (spritesong_[2]->GetPosition().x == 860)
		{
			position = spritesong_[2]->GetPosition();

			if (JumpFlagPlus_)
			{
				JumpTimer3_--;
				position.y += 1.0f;
			}
			else if (JumpFlagNegative_)
			{
				JumpTimer3_--;
				position.y -= 1.0f;
			}
			if (JumpTimer3_ <= 0 && JumpFlagPlus_)
			{
				JumpFlagPlus_ = false;
				JumpFlagNegative_ = true;
				JumpTimer3_ = 60;
			}
			else if (JumpTimer3_ <= 0 && JumpFlagNegative_)
			{
				JumpFlagPlus_ = true;
				JumpFlagNegative_ = false;
				JumpTimer3_ = 60;
			}
			spritesong_[2]->SetPosition(position);
			if (input_->TriggerKey(DIK_2))
			{
				scene = 4;
			}
		}
		if (spritesong_[3]->GetPosition().x == 860)
		{
			position = spritesong_[3]->GetPosition();

			if (JumpFlagPlus_)
			{
				JumpTimer4_--;
				position.y += 1.0f;
			}
			else if (JumpFlagNegative_)
			{
				JumpTimer4_--;
				position.y -= 1.0f;
			}
			if (JumpTimer4_ <= 0 && JumpFlagPlus_)
			{
				JumpFlagPlus_ = false;
				JumpFlagNegative_ = true;
				JumpTimer4_ = 60;
			}
			else if (JumpTimer4_ <= 0 && JumpFlagNegative_)
			{
				JumpFlagPlus_ = true;
				JumpFlagNegative_ = false;
				JumpTimer4_ = 60;
			}
			spritesong_[3]->SetPosition(position);
			if (input_->TriggerKey(DIK_2))
			{
				scene = 5;
			}
		}
		if (spritesong_[4]->GetPosition().x == 860)
		{
			position = spritesong_[4]->GetPosition();

			if (JumpFlagPlus_)
			{
				JumpTimer5_--;
				position.y += 1.0f;
			}
			else if (JumpFlagNegative_)
			{
				JumpTimer5_--;
				position.y -= 1.0f;
			}
			if (JumpTimer5_ <= 0 && JumpFlagPlus_)
			{
				JumpFlagPlus_ = false;
				JumpFlagNegative_ = true;
				JumpTimer5_ = 60;
			}
			else if (JumpTimer5_ <= 0 && JumpFlagNegative_)
			{
				JumpFlagPlus_ = true;
				JumpFlagNegative_ = false;
				JumpTimer5_ = 60;
			}
			spritesong_[4]->SetPosition(position);
			if (input_->TriggerKey(DIK_2))
			{
				scene = 6;
			}
		}

		debugText_->Printf("%f", position.x);
		debugText_->SetPos(0, 15);
		debugText_->Printf("%f", position.y);
		debugText_->SetPos(0, 30);
		if (test == "い") {
			debugText_->Printf("hanteitoretemasu");
		}
		debugText_->SetPos(0, 45);
		debugText_->Printf("%x", test2);
		debugText_->SetPos(0, 60);
		break;

	case 2:
		if (input_->TriggerKey(DIK_1))
		{
			scene = 1;
		}
		break;
	case 3:
		if (input_->TriggerKey(DIK_1))
		{
			scene = 1;
		}
		break;

	case 4:
		if (input_->TriggerKey(DIK_1))
		{
			scene = 1;
		}
		break;

	case 5:
		if (input_->TriggerKey(DIK_1))
		{
			scene = 1;
		}
		break;

	case 6:
		if (input_->TriggerKey(DIK_1))
		{
			scene = 1;
		}
		break;
	}
}

void Select::Draw() {

	//Sprite::PreDraw(commandList);

	switch (scene)
	{
	case 0:
		spriteTi_->Draw();
		break;
	case 1:
		if (difficultyColor == 0) { spriteBG_[0]->Draw(); }
		else if (difficultyColor == 1) { spriteBG_[1]->Draw(); }
		else if (difficultyColor == 2) { spriteBG_[2]->Draw(); }
		else if (difficultyColor == 3) { spriteBG_[3]->Draw(); }
		//spriteframe_->Draw();
		for (size_t i = 0; i < _countof(spritesong_); i++)
		{
			spritesong_[i]->Draw();
		}
		break;
	case 2:
		spriteSongPl1_->Draw();
		break;
	case 3:
		spriteSongPl2_->Draw();
		break;

	case 4:
		spriteSongPl3_->Draw();
		break;

	case 5:
		spriteSongPl4_->Draw();
		break;

	case 6:
		spriteSongPl5_->Draw();
		break;
	}

	//
	// スプライト描画後処理
	//Sprite::PostDraw();

#pragma endregion
}

void Select::SelectDrawData(int maxScore, int maxRank, int isFCAP, bool clear) {
	if (scene == 1) {
		int getNum = maxScore;
		//1桁ずつ表示
		if (getNum / 1000000 > 0) {
			spriteNum[0][getNum / 1000000]->Draw();
			spriteNum[1][10]->Draw();
			isZero[0] = true;
		}
		else { isZero[0] = false; }
		getNum = getNum % 1000000;
		if (isZero[0] || getNum / 100000 > 0) {
			spriteNum[2][getNum / 100000]->Draw();
			isZero[1] = true;
		}
		else { isZero[1] = false; }
		getNum = getNum % 100000;
		if (isZero[1] || getNum / 10000 > 0) {
			spriteNum[3][getNum / 10000]->Draw();
			isZero[2] = true;
		}
		else { isZero[2] = false; }
		getNum = getNum % 10000;
		if (isZero[2] || getNum / 1000 > 0) {
			spriteNum[4][getNum / 1000]->Draw();
			spriteNum[5][10]->Draw();
			isZero[3] = true;
		}
		else { isZero[3] = false; }
		getNum = getNum % 1000;
		if (isZero[3] || getNum / 100 > 0) {
			spriteNum[6][getNum / 100]->Draw();
			isZero[4] = true;
		}
		else { isZero[4] = false; }
		getNum = getNum % 100;
		if (isZero[4] || getNum / 10 > 0) {
			spriteNum[7][getNum / 10]->Draw();
			isZero[5] = true;
		}
		else { isZero[5] = false; }
		getNum = getNum % 10;
		spriteNum[8][getNum / 1]->Draw();
		isZero[6] = true;
		//ランク表示
		rankSprite[maxRank]->Draw();
		//チップ表示
		if (clear) {
			tipSprite[0]->Draw();
		}
		if (isFCAP > 0) {
			tipSprite[isFCAP]->Draw();
		}
	}
}

void Select::DrawDifficulty(int difficulty, int level) {
	if (scene != 0) {
		diff[difficulty]->Draw();
		levelS[level]->Draw();
	}
}