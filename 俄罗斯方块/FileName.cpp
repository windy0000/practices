#include<iostream>
#include<fstream>
#include<string>
#include <conio.h>
#include <stdlib.h>
#include<windows.h>
#include<algorithm>
using namespace std;
/*
�����棺
		��ʼ��Ϸ: ������Ϸ����
		���а�: �������а����
		������Ա: ���뿪����Ա����
		�̳�: ����̳̽���
		�˳���Ϸ: ��������

��Ϸ���棺

		չʾ��ͼ
		�������ɺ����䡢��ӡ

		���ղ���:
			s������������
			a��������һ��
			d��������һ��
			w����˳ʱ����ת90��


		���������У��ӷ�
		ʧ���жϣ�ʧ�ܺ����������


���а�/������Ա/�̳̽���:
		չʾ�������
		�����������

�˳���exit(0);
*/
//--------------����---------------
void gameView();
void tutorView();
void gotoXY(int x, int y);

/*����ĸ�������*/
class Block
{
public:
	int x; // ���ĺ������ڵ�ͼ�ϵ�ӳ�� 
	int y; // ���ĺ������ڵ�ͼ�ϵ�ӳ�� 
	int type; // �������� 
	int shape[4][4]; // ���Ӵ�С 
	int shapes[8][4][4];//���ַ�������
public:
	/* ���÷������� */
	void set(int _x, int _y, int _shape) {
		x = _x;
		y = _y;
		if (_shape != -1) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					shape[i][j] = shapes[_shape][i][j];
			type = _shape;

		}
	}
	/*�������ɷ�����״*/
	void generate() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					shapes[i][j][k] = 0;
		/* ʯͷ */
		shapes[0][1][1] = 1;
		/* ���� */
		shapes[1][1][0] = shapes[1][1][1] = shapes[1][1][2] = shapes[1][1][3] = 1;
		/* �� (��*/
		shapes[2][0][0] = shapes[2][0][1] = shapes[2][1][1] = shapes[2][2][1] = 1;
		/* �� (��) */
		shapes[3][0][1] = shapes[3][0][2] = shapes[3][1][1] = shapes[3][2][1] = 1;
		/* ͹ */
		shapes[4][0][1] = shapes[4][1][0] = shapes[4][1][1] = shapes[4][2][1] = 1;
		/* �� */
		shapes[5][1][1] = shapes[5][1][2] = shapes[5][2][1] = shapes[5][2][2] = 1;
		/* Z(��) */
		shapes[6][0][0] = shapes[6][0][1] = shapes[6][1][1] = shapes[6][1][2] = 1;
		/* Z(��)*/
		shapes[7][0][2] = shapes[7][0][1] = shapes[7][1][1] = shapes[7][1][0] = 1;
	}
};


int map[25][25];//��ͼ  
//��ͼ����ֵΪ1��Ϊ�߽�Ϊ0��Ϊ���ƶ�����

 //��ҵ�id�͵÷�
	
int score=0;

//----------����--------------

//------------service-------------
/* ��ʼ����ͼ */
void initMap()

/*  
	����: ���õ�ͼ�ĸ��ֲ���
	����: void
	����ֵ: void

	��ͼΪmap[][]
	���ñ߽��ֵΪ1,���ƶ�����Ϊ0
	*/
{
	
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (i == 20 - 1) {
				map[i][j] = 1;
			}
			else if (j == 0 || j == 15 - 1) {
				map[i][j] = 1;
			}
			else {
				map[i][j] = 0;
			}
		}

	}
}


/* ���Ƶ�ͼ */
void drawMap()

/*  
	����:��ӡ��ͼ
	����: void
	����ֵ: void

	map[][]�е�ֵΪ1ʱ��ӡǽ�塮����Ϊ0ʱ��ӡ���ƶ����� ��
	*/
{
	
	for (int i = 0; i < 20; i++) {
		//cout << i;
		for (int j = 0; j < 15; j++) {
			if (map[i][j] == 1) {
				std::cout << "*";	// ǽ�� 
			}
			else {
				std::cout << " "; 	// ���ƶ����� 
			}
		}
		std::cout << endl;
	}
}


/*��ʾ����*/

void drawNumber(int score){
	gotoXY(25, 10);
cout << "�÷֣�" <<score << endl;
}

/*
	����: ��ʾ��ǰ�÷�
	����: void
	����ֵ: void
*/





/* ���Ʒ��� */
void gotoXY(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void drawBlock(Block block)
{
	int x = block.x;
	int y = block.y;
	//cout << x << " " << y;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block.shape[i][j] == 1)
			{
				gotoXY(y + j, x + i);
				//cout << x + i << " " << y + j << endl;
				cout << "*";
			}
		}
	}
};


/*  
	����: �������ַ���
	����: void
	����ֵ: void

	shape������Ϊ1��λ�ô�ӡ������
*/


/* ������� */
void cleanBlock(Block block) {
	

	int x = block.x;
	int y = block.y;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block.shape[i][j] == 1)
			{
				gotoXY(y + j,x + i);
				cout << " ";
			}
		}
	}
}
/*

����: ����ͼ�з������
����: void
����ֵ: void

shape��ֵΪ1��λ�ô�ӡ�� ��

*/


/* ��ӷ��鵽��ͼ�� */
void addBlock(Block block)

/*

	����: ����������ͼ�У���ֹ�����
	����: void
	����ֵ: void

	�Ȼ�ȡ�������Ͻ��ڵ�ͼ�е�����
	int x = block.x;
	int y = block.y;
	֮���ж�������㴥������
	��shape ��ֵ����map
	map[x+i][y+j] = block.shape[i][j];
*/
{
	int x = block.x;
	int y = block.y;
	// ���ж���û�д���
	/*int flag = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int tx = x + i - 1;
			int ty = y + j - 1;
			if(map[tx+1][ty]==1)
			{
				flag = 1;
				break;
			}
		}
	}
	if (flag == 0)	return;*/

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (!map[x + i][y + j]) {
				map[x + i][y + j] = block.shape[i][j];
			}
		}
	}
}


/*��麯��*/
int check(Block* block)
{

	int x = block->x;
	int y = block->y;
	//cout << x << " "<<y;
	int sign = block->type;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			
				if (block->shape[i][j] == 1&&map[x+i][y+j]==1) {
					//cout << x << " "<<y;
					//cout << i << " " << j;
					return 1;
				}
				if (map[x + i + 1][y + j] == 1 && block->shape[i][j] == 1) {
					return 2;
				}
		}
	}
	return 0;
}

/*

	����: ����ƶ�����ת�Ƿ�Ϸ����Լ��Ƿ�����
	����: int
	����ֵ: int

	������˹�������ͼ���غ�
	����غ����1
	���غ����0


*/

/*������ƶ�*/
int _move(Block* block, int sign)

/*
����: ʵ�ַ�����ƶ�
����: int
����ֵ: int

sign�� 0��ʾ�����ƶ��� 1��ʾ���ƶ���-1��ʾ���ƶ���2��ʾ�����ƶ�
������������block�ķ���
���¶�λblock��λ�ã������µ�block(��������һ�����ӣ���s������������)
*/
{


	cleanBlock(*block);
	if (sign == 1)
		block->set(block->x, block->y + 1, -1);
	else if (sign == -1)
		block->set(block->x, block->y - 1, -1);
	else if (sign == 0)
		block->set(block->x + 1, block->y, -1);
	else if (sign == 2)
		block->set(block->x + 3, block->y, -1);
	if (check(block) == 1) {
		if (sign == 1)
			block->set(block->x, block->y - 1, -1);
		else if (sign == -1)
			block->set(block->x, block->y + 1, -1);
		else if (sign == 0)
			block->set(block->x - 1, block->y, -1);
		
	}
	drawBlock(*block);
	return 0;
}




/* ������ת90�� */
void rotation(Block* block) {
	// ��ת���Ͻ�3*3�ľ��󣬲�����director��ת90��

	/* ������ת */
		/* ��ת�� */
	int value = block->shape[0][0];
	block->shape[0][0] = block->shape[0][2];
	block->shape[0][2] = block->shape[2][2];
	block->shape[2][2] = block->shape[2][0];
	block->shape[2][0] = value;
	/* ʮ��ת�� */
	value = block->shape[0][1];
	block->shape[0][1] = block->shape[1][2];
	block->shape[1][2] = block->shape[2][1];
	block->shape[2][1] = block->shape[1][0];
	block->shape[1][0] = value;



	/* ������ӵ�������� */
	if (block->type == 1) {
		if (block->shape[1][3] == 1) {
			block->shape[1][3] = 0;
			block->shape[3][1] = 1;
		}
		else {
			block->shape[1][3] = 1;
			block->shape[3][1] = 0;
		}
	}
}
/*������ת*/
void _transfer( Block* block) {

	cleanBlock(*block);	 // ������תǰ��block
	int cur[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cur[i][j] = block->shape[i][j];
		}
	}
	rotation(block);
	if (check(block)) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				block->shape[i][j] = cur[i][j];
			}
		}
	}

	/*if (check(block) == 0)
	{
		rotation(block);
	}*/
	drawBlock(*block);  // ���»�����ת���block 
}



/* ��ת */
void transfer(Block* block) {

	int sign = block->type;
	/*
	sign�� ʲô���͵ķ��飬 ��ͬ���͵ķ������ת���Բ�ͬ
	*/
	if (sign == 2 || sign == 3 || sign == 4 || sign == 1 || sign == 6 || sign == 7) { // ��Ҫ�����
		_transfer(block);
	}

	else if (sign == 0 || sign == 5) {	// ����Ҫ����� 

		return;
	}


}

/*����Ƿ����*/
int _checkLine(int line) {
	for (int i = 0; i < 15; i++) {
		if (map[line][i] == 0)
			return 0;
	}
	
	return 1;
}
void checkLine() {
	int* array = new int[20 * 15];
	int** temp = new int* [20];
	for (int i = 0; i < 20; i++) temp[i] = array + i * 15;
	//temp��������һ���������ϴ棻
	int index = 18;
	//����map���飬�ҵ�����δ���е��У�������Щ����Ϣ��ֵ��temp
	for (int i = 18; i >= 0; i--) {
		int j = 1;
		for (; j <= 13; j++) {
			if (map[i][j] !=1) break;
		}
		//j<14˵����δ���У�
		if (j < 14) {
			
			//��������Ϣ���Ƶ�temp�У�
			for (int m = 1; m <= 13; m++) {
				temp[index][m] = map[i][m];
			}
			index--;
		}
		else
		{
			//������ӷ֣�
			score += 10;
		}
		
	}
	//����map���飻
	initMap();
	//��temp������Ϣ��ֵ��map��
	for (int i = 0; i <= 18; i++) {
		for (int j = 1; j <= 13; j++) {
			map[i][j] = temp[i][j];
		}
	}
	delete[] array;
	delete[] temp;
}

/*
	����: ����Ƿ���У����мӷֲ�����
	����: void
	����ֵ: void

	���һ�ж�Ϊ1֮�󽫸������ϵ������궼-1����

*/
//---------------service----------------

//---------------view-------------------

/*�˵�����*/
void menuView() {
	int n = 0;
	std::cout << "1.��ʼ��Ϸ";
	std::cout << endl << "2.�鿴������Ա";
	std::cout << endl << "3.�鿴�̳�";
	std::cout << endl << "4.�˳���Ϸ" << endl;
	std::cout << "������ѡ��";
	cin >> n;
	switch (n) {
	case 1:
		gameView();
		break;
	
	case 2:
		tutorView();
		break;
	case 3:
		system("cls");
		std::cout << "     �̳�      " << endl;
		std::cout << "ad�����ƶ�����" << endl;
		std::cout << "�������Ʒ���" << endl;
		std::cout << "w˳ʱ����ת����" << endl;
		system("pause");
		system("cls");
		menuView();
		break;
		/*
		����: ���ƽ̳̲˵�
		����: void
		����ֵ: void


		����������� system("pause")
		system("cls")�����ٴε��ÿ�ʼ�˵�
	*/
	case 4:
		exit(0);
		break;
	}
}
/*

	����: ���ƿ�ʼ�˵�
	����: void
	����ֵ: void

	����Ӧ����������Ӧ����

*/

	/*�����߳�Ա����*/
void tutorView()
{
	system("cls");
	char a = ' ';
	system("pause");
	system("cls");
	menuView();
}

/*
	����: ���ƿ����߳�Ա����
	����: void
	����ֵ: void

	����������� system("pause")
	system("cls")�����ٴε��ÿ�ʼ�˵�
*/

int FPS(int f) {
	static long long oldTime = 0;
	long long nowTime = clock();
	if (nowTime - oldTime >= f) {
		oldTime = nowTime;
		return 1;
	}
	return 0;
}

/*������Ϸ*/
void gameView() {
	system("cls");
	initMap();
	drawMap();
	
	while (true) {
		Block* block = new Block();
		block->generate();
		block->set(0, 4, rand() % 8);
		drawBlock(*block);
		while (true) {
			if (FPS(555))
			{
				drawNumber(score);
				//cout << 1;
				if (_kbhit())
				{
					char key = _getch();
					if (key == 'w' || key == 'W') {
						transfer(block);
					}
					else if (key == 'a' || key == 'A') {
						_move(block, -1);
					}
					else if (key == 'd' || key == 'D') {
						_move(block, 1);
					}
					else if (key == 's' || key == 'S') {
						for (int i = 0; i < 3; i++)
						{
							if (check(block) != 2)
								_move(block, 0);
							else
								break;
						}

					}
				}
				else if (!_kbhit()) {
					_move(block, 0);
				}
			}
			else
				Sleep(10);
			if (check(block) == 2) {
				addBlock(*block);
				//drawMap();
				checkLine();
				system("cls");
				drawMap();
				break;
			}
		}
		//cout << 1;

		if (block->x < 1)
		{
			
			system("cls");
			cout << "��Ϸʧ��,���յ÷֣�"<<score<<"�֣��밴��������ز˵�" << endl;
			system("pause");
			system("cls");
			menuView();
			
		}
		//system("pause");
	}
}

/*
	����: ������Ϸ
	����: void
	����ֵ: void


	��while(true)���ø��ֺ���������Ϸ���������ò�ͬ����
	{
	   void initMap(int** map) {};
	   void drawMap(int** map) {};
	   ....

	  �ж�ʧ�ܻ��˳���Ϸʱ break�˳�ѭ��


	}

*/
//---------------view-------------------
int main()
{
    menuView();
	
	return 0;
}