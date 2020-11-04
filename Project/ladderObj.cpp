#include "ladderObj.h"

void ladderObj::createLadder(ladderObj temp) {
	int i, j;
	int random;

	//������ �����Ͽ� �迭�� ����
	for (i = 0; i < temp.num; i++) {
		for (j = 0; j < temp.num; j++) {
			random = rand() % 2;

			//������ ���� ��� 0���� �ʱ�ȭ(������ �������� ���μ��� ����)
			if (j == temp.num - 1) {
				temp.arr[i][j] = 0;
				continue;
			}

			//1���� ���
			if (j == 0) temp.arr[i][j] = random;
			
			//1���� �ƴ� ���
			if (j != 0) {
				if (random == 1) {
					//���� ������ ���ÿ� �̾��� ���� �ʴٸ� ���� 1�� ����
					if (temp.arr[i][j - 1] != random) temp.arr[i][j] = random;
					
					//�׷��� �ʴٸ� 0�� ����
					else temp.arr[i][j] = 0;
				}
				
				if (random == 0) {
					temp.arr[i][j] = random;
				}
			}
		}
	}
}

void ladderObj::resultLadder(ladderObj temp) {
	int i, j, k;
	int idx = 0; //��� ���� �ε���
	int term = 80; //��ٸ� �� ������ ����
	int radius = 30; //������� ������ ����
	int rec_y = 20; //������ ���� ������ ����

	for (k = 0; k < temp.num; k++) { //��� ������� ���Ͽ� �ݺ�
		j = k;
		i = 0;
		idx = 0;

		temp.v1[k][idx].set((j + 1) * term, (i + 1) * term + radius); //���� ��ư�� �Ʒ� �� ��ǥ ����
		
		idx++;
		temp.v1[k][idx].set((j + 1) * term, (i + 2) * term); //��ٸ��� ���� ��ǥ ����

		while (i <= temp.num - 1) {	//��������� �������� �����Ҷ� ����
			if (j != 0) { //y��ǥ�� 0�� �ƴ� ���
				if (temp.arr[i][j - 1] == 1) { //�������� �̵��� �ڿ� �Ʒ��� �̵�
					idx++;
					temp.v1[k][idx].set(j * term, (i + 2) * term); //������ ��ǥ ����
					
					j--; //�������� �̵�
					
					if (i != temp.num - 1) {
						idx++;
						temp.v1[k][idx].set((j + 1) * term, (i + 3) * term); //�Ʒ� ��ǥ ����
					}
					
					i++; //�Ʒ��� �̵�
					continue;
				}
			}

			if (temp.arr[i][j] == 1) { //���������� �̵��� �ڿ� �Ʒ��� �̵�
				idx++;
				temp.v1[k][idx].set((j + 2) * term, (i + 2) * term); //������ ��ǥ ����
				
				j++; //���������� �̵�

				if (i != temp.num - 1) {
					idx++;
					temp.v1[k][idx].set((j + 1) * term, (i + 3) * term); //�Ʒ� ��ǥ ����
				}
				
				i++; //�Ʒ��� �̵�
				continue;
			}

			if (temp.arr[i][j] == 0) { //�Ʒ��� �̵�
				if (i != temp.num - 1) {
					idx++;
					temp.v1[k][idx].set((j + 1) * term, (i + 3) * term); //�Ʒ� ��ǥ ����
				}
				
				i++; //�Ʒ��� �̵�
			}
		}

		i--;
		idx++;
		temp.v1[k][idx].set((j + 1) * term, (i + 3) * term - rec_y); //���� ��ư�� �� �� ��ǥ ����

		idx++;
		temp.v1[k][idx].set(-1, -1); //����� �������� (-1, -1)�� �����Ѵ�. (���� �˱� ���Ͽ�)

		temp.last[k] = j; //�� ������� �������� ����
	}
}