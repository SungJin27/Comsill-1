#pragma once
#include <time.h>
#include <cstdlib>
#include <string.h>
#include <ofVec2f.h>

class ladderObj
{
public:
	
	int** arr; //��ٸ� ���μ� ���� ����
	int num; //���� �ο� ��
	char** result; //��÷ �׸�
	int* last; //����������� �������� �ε���
	ofVec2f** v1; //��� ���� ����

	void createLadder(ladderObj temp); //��ٸ��� ���μ� ���� �Լ�
	void resultLadder(ladderObj temp); //�̵� ��θ� ����ϴ� �Լ�
};