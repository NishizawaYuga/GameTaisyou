#include "chartManager.h"

void ChartManager::Initialize() {
	//�S�Ă̋Ȃ̃f�[�^�ǂݍ���
	ID000();
}

void ChartManager::IncludeChart(int ID,int chart[4][20000]) {
	//���艟���ǂݍ���
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 20000; j++) {
			musicData[ID].layer[i].chart[j] = chart[j][i];
		}
	}
}

void ChartManager::ID000() {
	//test�p�f�[�^
	//����
	musicData[0].beatDenomonator = 4;
	//���q
	musicData[0].beatMolecule = 4;
	//BPM
	musicData[0].BPM = 120;
	//���ʑ��x�i�{���j
	musicData->speed = 1;
	//����
	int chart[4][20000] =
	{	//���ʃf�[�^�A���[���E�[�œǂݍ���ł���
		{0,0,0,0, 0,0,0,0, 0,0,0,0,},
		{0,0,0,0, 0,0,0,0, 0,0,0,0,},
		{0,0,0,0, 0,0,0,0, 0,0,0,0,},
		{0,0,0,0, 0,0,0,0, 0,0,0,0,}
	};

	IncludeChart(0, chart);
}