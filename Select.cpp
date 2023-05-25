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


	//曲の選択中の上下運動のタイマー(配列使って圧縮)
	for (int i = 0; i < 5; i++) {
		jumpTimer[i] = 30;
	}

	//2
	textureHandleSong2_ = TextureManager::Load("sprite/0002.png");
	//3
	textureHandleSong3_ = TextureManager::Load("sprite/0002.png");
	//4
	textureHandleSong4_ = TextureManager::Load("sprite/0002.png");
	//5
	textureHandleSong5_ = TextureManager::Load("sprite/0002.png");
	//6
	textureHandleSong6_ = TextureManager::Load("sprite/0006.png");
	//7
	textureHandleSong7_ = TextureManager::Load("sprite/0007.png");

	//BG
	for (int i = 0; i < 5; i++) {
		textureHandleBG_[i] = TextureManager::Load("sprite/moyou00" + std::to_string(i) + ".png");
		spriteBG_[i] = Sprite::Create(textureHandleBG_[i], { 0,0 });
	}

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
			spriteNum[j][i] = Sprite::Create(textureNum[i], { 0.0f + posX,700 });
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
			tipSprite[i] = Sprite::Create(tip[i], { 0.0f + 80.0f,850 });
		}
		else {
			tipSprite[i] = Sprite::Create(tip[i], { 0.0f + 240.0f,850 });
		}
	}

	//難易度
	for (int i = 0; i < 4; i++) {
		diffNum[i] = TextureManager::Load("ui/difficulty/difficulty" + std::to_string(i) + ".png");
		diff[i] = nullptr;
		diff[i] = Sprite::Create(diffNum[i], { 1356,0 });
	}

	//曲名等
	noteDesigner = TextureManager::Load("ui/musicdatafont/nd.png");
	noteDesignerSprite = Sprite::Create(noteDesigner, { 10,260 });
	for (int i = 0; i < 6; i++) {
		//0とnull代入
		musicName[i] = 0;
		musicNameSprite[i] = nullptr;
		artist[i] = 0;
		artistSprite[i] = nullptr;
		for (int j = 0; j < 4; j++) {
			nd[i][j] = 0;
			ndSprite[i][j] = nullptr;
		}
	}
	//バンバード（仮で全ての配列に読み込み）
	for (int j = 1; j < _countof(musicName); j++) {
		musicName[j] = TextureManager::Load("ui/musicdatafont/banbad_name0.png");
		artist[j] = TextureManager::Load("ui/musicdatafont/banbad_name1.png");
		for (int i = 0; i < 4; i++) {
			if (i < 3) {
				nd[j][i] = TextureManager::Load("ui/musicdatafont/banbad_name2.png");
			}
			else {
				nd[j][i] = TextureManager::Load("ui/musicdatafont/banbad_name3.png");
			}

		}
	}
	//オプション（使用しない番号0に登録
	musicName[0] = TextureManager::Load("ui/musicdatafont/option.png");
	musicNameSprite[0] = Sprite::Create(musicName[0], { 10,20 });
	//まとめてCreate
	for (int i = 1; i < _countof(musicNameSprite); i++) {
		musicNameSprite[i] = Sprite::Create(musicName[i], { 10,20 });
		artistSprite[i] = Sprite::Create(artist[i], { 10,180 });
		for (int j = 0; j < 4; j++) {
			ndSprite[i][j] = Sprite::Create(nd[i][j], { 10 + 76,260 });
		}
	}

	//オプション
	tab = 1;
	speed = 2;
	style = 0;
	autoPlay = 0;
	detail = 0;
	wall = 0;
	optionNum = 0;
	for (int i = 0; i < 11; i++) {
		options[0][i] = 0;
		options[0][i] = TextureManager::Load("ui/options/speed/speed" + std::to_string(i) + ".png");
		optionsSprite[0][i] = nullptr;
		optionsSprite[0][i] = Sprite::Create(options[0][i], { 860,360 });
		options[1][i] = 0;
		options[1][i] = TextureManager::Load("ui/options/style/style" + std::to_string(i) + ".png");
		optionsSprite[1][i] = nullptr;
		optionsSprite[1][i] = Sprite::Create(options[1][i], { 1160,360 });
		options[2][i] = 0;
		options[2][i] = TextureManager::Load("ui/options/auto/auto" + std::to_string(i) + ".png");
		optionsSprite[2][i] = nullptr;
		optionsSprite[2][i] = Sprite::Create(options[2][i], { 1460,360 });
		options[3][i] = 0;
		options[3][i] = TextureManager::Load("ui/options/wall/wall" + std::to_string(i) + ".png");
		optionsSprite[3][i] = nullptr;
		optionsSprite[3][i] = Sprite::Create(options[3][i], { 260,360 });
		options[4][i] = 0;
		options[4][i] = TextureManager::Load("ui/options/detail/detail" + std::to_string(i) + ".png");
		optionsSprite[4][i] = nullptr;
		optionsSprite[4][i] = Sprite::Create(options[4][i], { 560,360 });
	}

	//画面下部ナビ
	controlTx = TextureManager::Load("ui/contorol.png");
	controlSp = Sprite::Create(controlTx, { 0,0 });

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

	//リザルト

}

void Select::Update(int& sceneNum, int& musicID, int& difficulty, OptionsData& options)
{
	switch (scene)
	{
	case 0:
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene = 1;
		}
		break;
	case 1:
		//tabで切り替え
		if (input_->PushKey(DIK_TAB)) {
			if (input_->TriggerKey(DIK_LEFT)) {
				tab--;
				if (tab < 0) {
					tab = 2;
				}
			}
			else if (input_->TriggerKey(DIK_RIGHT)) {
				tab++;
				if (tab > 2) {
					tab = 0;
				}
			}
		}
		if (tab == 1) {
			if (difficulty == 3) {
				difficulty = 0;
				difficultyColor = difficulty;
			}
			if (!input_->PushKey(DIK_TAB)) {
				// ステージセレクト　入力処理
				if (input_->TriggerKey(DIK_LEFT))
				{

					// ステージの番号を300ずらす
					for (size_t i = 0; i < _countof(spritesong_); i++)
					{
						position = spritesong_[i]->GetPosition();
						position.x += 300;
						position.y = 360;
						jumpTimer[i] = 30;
						// 1460が右端なのでこれより大きかったら最小値の260の位置にする
						if (position.x > 1460)
						{
							position.x = 260;

						}

						spritesong_[i]->SetPosition(position);
					}
					musicID++;
					if (musicID > 5) {
						musicID = 1;
					}
				}

				if (input_->TriggerKey(DIK_RIGHT))
				{

					// ステージの番号を-300ずらす
					for (size_t i = 0; i < _countof(spritesong_); i++)
					{
						position = spritesong_[i]->GetPosition();
						position.x -= 300;
						position.y = 360;
						jumpTimer[i] = 30;
						// 260が左端なのでこれより小さかったら最大値の1460の位置にする
						if (position.x <= 250)
						{
							position.x = 1460;
						}

						spritesong_[i]->SetPosition(position);


					}
					musicID--;
					if (musicID < 1) {
						musicID = 5;
					}
				}
				//難易度変更
				if (input_->TriggerKey(DIK_UP)) {
					if (difficulty < 2) {
						difficulty++;
						difficultyColor = difficulty;
					}
				}
				else if (input_->TriggerKey(DIK_DOWN)) {
					if (difficulty > 0) {
						difficulty--;
						difficultyColor = difficulty;
					}
				}
				if (input_->TriggerKey(DIK_SPACE))
				{
					sceneNum = 2;
				}
			}
			if (input_->TriggerKey(DIK_BACKSPACE))
			{
				scene = 0;
			}
			if (input_->TriggerKey(DIK_1))
			{
				sceneNum = 3;
			}

			for (int i = 0; i < _countof(spritesong_); i++) {
				if (spritesong_[i]->GetPosition().x == 860)
				{

					position = spritesong_[i]->GetPosition();

					if (JumpFlagPlus_)
					{
						jumpTimer[i]--;
						position.y += 1.0f;
					}
					else if (JumpFlagNegative_)
					{
						jumpTimer[i]--;
						position.y -= 1.0f;
					}
					if (jumpTimer[i] <= 0 && JumpFlagPlus_)
					{
						JumpFlagPlus_ = false;
						JumpFlagNegative_ = true;
						jumpTimer[i] = 60;
					}
					else if (jumpTimer[i] <= 0 && JumpFlagNegative_)
					{
						JumpFlagPlus_ = true;
						JumpFlagNegative_ = false;
						jumpTimer[i] = 60;
					}
					spritesong_[i]->SetPosition(position);
					if (input_->TriggerKey(DIK_2))
					{
						scene = i;
					}
				}
			}
		}
		else if (tab == 0) {
			if (difficulty != 3) {
				difficulty = 3;
				difficultyColor = difficulty;
			}
			if (!input_->PushKey(DIK_TAB)) {
				// ステージセレクト　入力処理
				if (input_->TriggerKey(DIK_LEFT))
				{

					// ステージの番号を300ずらす
					for (size_t i = 0; i < _countof(spritesong_); i++)
					{
						position = spritesong_[i]->GetPosition();
						position.x += 300;
						position.y = 360;
						jumpTimer[i] = 30;
						// 1460が右端なのでこれより大きかったら最小値の260の位置にする
						if (position.x > 1460)
						{
							position.x = 260;

						}

						spritesong_[i]->SetPosition(position);
					}
					musicID++;
					if (musicID > 5) {
						musicID = 1;
					}
				}

				if (input_->TriggerKey(DIK_RIGHT))
				{

					// ステージの番号を-300ずらす
					for (size_t i = 0; i < _countof(spritesong_); i++)
					{
						position = spritesong_[i]->GetPosition();
						position.x -= 300;
						position.y = 360;
						jumpTimer[i] = 30;
						// 260が左端なのでこれより小さかったら最大値の1460の位置にする
						if (position.x <= 250)
						{
							position.x = 1460;
						}

						spritesong_[i]->SetPosition(position);


					}
					musicID--;
					if (musicID < 1) {
						musicID = 5;
					}
				}
				if (input_->TriggerKey(DIK_SPACE))
				{
					sceneNum = 2;
				}
			}
			if (input_->TriggerKey(DIK_BACKSPACE))
			{
				scene = 0;
			}

			for (int i = 0; i < _countof(spritesong_); i++) {
				if (spritesong_[i]->GetPosition().x == 860)
				{

					position = spritesong_[i]->GetPosition();

					if (JumpFlagPlus_)
					{
						jumpTimer[i]--;
						position.y += 1.0f;
					}
					else if (JumpFlagNegative_)
					{
						jumpTimer[i]--;
						position.y -= 1.0f;
					}
					if (jumpTimer[i] <= 0 && JumpFlagPlus_)
					{
						JumpFlagPlus_ = false;
						JumpFlagNegative_ = true;
						jumpTimer[i] = 60;
					}
					else if (jumpTimer[i] <= 0 && JumpFlagNegative_)
					{
						JumpFlagPlus_ = true;
						JumpFlagNegative_ = false;
						jumpTimer[i] = 60;
					}
					spritesong_[i]->SetPosition(position);
					if (input_->TriggerKey(DIK_2))
					{
						scene = i;
					}
				}
			}
		}
		else if (tab == 2) {
			if (!input_->PushKey(DIK_TAB)) {
				// オプションセレクト　入力処理
				if (input_->TriggerKey(DIK_LEFT))
				{
					// ステージの番号を300ずらす
					for (size_t i = 0; i < 5; i++)
					{
						for (int j = 0; j < 11; j++) {
							position = optionsSprite[i][j]->GetPosition();
							position.x += 300;
							position.y = 360;
							// 1460が右端なのでこれより大きかったら最小値の260の位置にする
							if (position.x > 1460)
							{
								position.x = 260;

							}

							optionsSprite[i][j]->SetPosition(position);
						}
					}
					optionNum--;
					if (optionNum < 0) {
						optionNum = 4;
					}
				}

				if (input_->TriggerKey(DIK_RIGHT))
				{
					// ステージの番号を-300ずらす
					for (size_t i = 0; i < 5; i++)
					{
						for (int j = 0; j < 11; j++) {
							position = optionsSprite[i][j]->GetPosition();
							position.x -= 300;
							position.y = 360;
							jumpTimer[i] = 30;
							// 260が左端なのでこれより小さかったら最大値の1460の位置にする
							if (position.x <= 250)
							{
								position.x = 1460;
							}
							optionsSprite[i][j]->SetPosition(position);
						}
					}
					optionNum++;
					if (optionNum > 4) {
						optionNum = 0;
					}
				}
				//各項目変更
				if (input_->TriggerKey(DIK_UP)) {
					//譜面速度
					if (optionNum == 0) {
						if (speed < 9) {
							speed++;
							options.speed++;
						}
						
					}
					//キー配置
					else if (optionNum == 1) {
						style++;
						options.style++;
						if (style > 2) {
							style = 0;
							options.style = 0;
						}
					}
					//オートプレイ
					else if (optionNum == 2) {
						autoPlay = 1;
						options.autoPlay = true;
					}
					//ウォール
					else if (optionNum == 3) {
						if (wall < 10) {
							wall++;
							options.wall++;
						}
					}
					//詳細表示
					else if (optionNum == 4) {
						detail = true;
						options.detail = true;
					}
				}
				else if (input_->TriggerKey(DIK_DOWN)) {
					if (optionNum == 0) {
						if (speed > 0) {
							speed--;
							options.speed--;
						}
					}
					else if (optionNum == 1) {
						style--;
						options.style--;
						if (style < 0) {
							style = 2;
							options.style = 2;
						}
					}
					else if (optionNum == 2) {
						autoPlay = 0;
						options.autoPlay = false;
					}
					else if (optionNum == 3) {
						if (wall > 0) {
							wall--;
							options.wall--;
						}
					}
					else if (optionNum == 4) {
						detail = false;
						options.detail = false;
					}
				}
			}
		}
		break;

	case 2:
		if (input_->TriggerKey(DIK_1))
		{
			sceneNum = 3;
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
		if (tab != 2) {
			if (difficultyColor == 0) { spriteBG_[0]->Draw(); }
			else if (difficultyColor == 1) { spriteBG_[1]->Draw(); }
			else if (difficultyColor == 2) { spriteBG_[2]->Draw(); }
			else if (difficultyColor == 3) { spriteBG_[3]->Draw(); }
			//spriteframe_->Draw();
			for (size_t i = 0; i < _countof(spritesong_); i++)
			{
				spritesong_[i]->Draw();
			}
		}
		else if (tab == 2) {
			spriteBG_[4]->Draw();
			optionsSprite[0][speed]->Draw();
			optionsSprite[1][style]->Draw();
			optionsSprite[2][autoPlay]->Draw();
			optionsSprite[3][wall]->Draw();
			optionsSprite[4][detail]->Draw();
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
	if (scene == 1 && tab != 2) {
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
		controlSp->Draw();
	}
}

void Select::DrawDifficulty(int difficulty, int level) {
	if (scene != 0) {
		if (tab != 2) {
			diff[difficulty]->Draw();
			levelS[level]->Draw();
		}
	}
}

void Select::DrawMusicString(int difficulty, int ID) {
	//曲名等表示
	if (tab != 2) {
		musicNameSprite[ID]->Draw();
		artistSprite[ID]->Draw();
		noteDesignerSprite->Draw();
		ndSprite[ID][difficulty]->Draw();
	}
	else if (tab == 2) {
		musicNameSprite[0]->Draw();
	}
}