#include "chartManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void ChartManager::Initialize() {
	//�S�Ă̋ȃf�[�^�������i�l�dfor���c�j
	for(int M = 0;M < musicNum;M++){								//�Ȑ�
		for (int i = 0; i < layerNum; i++) {						//���C���[��
			for (int j = 0; j < columnNum; j++) {					//��
				for (int k = 0; k < maxNotes; k++) {				//�u����m�[�c��
					musicData[M].layer[i].note[j].chart[k] = 0;
				}
			}
		}
		musicData[M].BPM = 0;
		musicData[M].beatDenomonator = 0;
		musicData[M].beatMolecule = 0;
		musicData[M].speed = 0;
	}

	//�S�Ă̋Ȃ̃f�[�^�ǂݍ���
	ID000();
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

	LoadData(0, "musicData/testmc.txt");
}

void ChartManager::LoadData(int ID, string filePass) {

	//���͂��ꂽ�t�@�C���p�X����t�@�C���I�u�W�F�N�g�쐬
	ifstream ifs(filePass);

	//�J���Ȃ�������G���[
	if (!ifs) {
		cout << "�t�@�C�����J���܂���ł���" << endl;
		return;
	}

	string str = "";

	//���C���[
	int i = 0;
	//��
	int j = 0;
	//�u����m�[�c��
	int k = 0;
	 
	//�t�@�C���̒��g����s���ǂݍ���
	while (getline(ifs, str)) {
		string tmp = "";
		istringstream stream(str);

		//��؂蕶�����Ȃ��Ȃ�܂ŕ�������؂�
		while (getline(stream, tmp, ',')) {
			musicData[ID].layer[i].note[j].chart[k] = atoi(tmp.c_str());
			k++;
		}
		//�m�[�c�ǂݍ��݂��ŏ������
		k = 0;
		//���̗��
		j++;
		//j���S�Ă̍s��ǂݍ��񂾂烊�Z�b�g���ă��C���[�ύX
		if (j > 3) {
			j = 0;
			i++;
		}
	}
}