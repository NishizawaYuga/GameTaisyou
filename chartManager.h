#pragma once
#include "Lane.h"

//���y�f�[�^�Ǘ��N���X
class ChartManager {

public:
	//������
	void Initialize();
	//���y�f�[�^�Ăяo��
	MusicData GetData(int idnum) { return musicData[idnum]; }
private:
	//���ʂ��e�Ȃɓ����
	void IncludeChart(int ID,int chart[4][20000]);
	//�ȃf�[�^�������Q
	void ID000();	//�e�X�g����

private:
	//���y�f�[�^
	MusicData musicData[10];
};