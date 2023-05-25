#pragma once
//Selectに渡す用リザルトデータ
struct ResultData {
	//スコア
	int score;
	//ハイスコア
	int hiScore;
	//ランク
	int rank;
	//FCAP
	int isFCAP;
	//クリアフラグ
	bool isClear;
	//内訳
	int perfect;
	int great;
	int miss;
	//fast/late
	int Pfast;
	int Plate;
	int Gfast;
	int Glate;
	//最高コンボ数
	int maxCombo;
};
