#pragma once
#include "Lane.h"
#include <string>

//���y�f�[�^�Ǘ��N���X
class ChartManager {

public:
	//������
	void Initialize();
	//���y�f�[�^�Ăяo��
	MusicData GetData(int idnum) { return musicData[idnum]; }
private:
	//�t�@�C���ǂݍ���
	void LoadData(int ID, std::string filePass);

	//�ȃf�[�^�������Q
	void ID000();	//�e�X�g����

private:
	//���y�f�[�^
	MusicData musicData[10];

	//���y��
	const int musicNum = 10;
	//���C���[��
	const int layerNum = 4;
	//��
	const int columnNum = 4;
	//�u����m�[�c��
	const int maxNotes = 20000;
};