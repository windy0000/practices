#include<iostream>
#include<fstream>
#include<string>
#include <conio.h>
#include <stdlib.h>
#include<windows.h>
#include<algorithm>
using namespace std;
/*
主界面：
		开始游戏: 进入游戏界面
		排行榜: 进入排行榜界面
		开发人员: 进入开发人员界面
		教程: 进入教程界面
		退出游戏: 结束程序

游戏界面：

		展示地图
		方块生成和下落、打印

		接收操作:
			s——下落三格
			a——左移一格
			d——右移一格
			w——顺时针旋转90度


		方块消除行，加分
		失败判断，失败后按任意键返回


排行榜/开发人员/教程界面:
		展示相关内容
		按任意键返回

退出：exit(0);
*/
//--------------数据---------------
void gameView();
void tutorView();
void gotoXY(int x, int y);

/*方块的各种属性*/
class Block
{
public:
	int x; // 中心横坐标在地图上的映射 
	int y; // 中心横坐标在地图上的映射 
	int type; // 方块类型 
	int shape[4][4]; // 格子大小 
	int shapes[8][4][4];//八种方块类型
public:
	/* 设置方块属性 */
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
	/*设置生成方块形状*/
	void generate() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					shapes[i][j][k] = 0;
		/* 石头 */
		shapes[0][1][1] = 1;
		/* 棍子 */
		shapes[1][1][0] = shapes[1][1][1] = shapes[1][1][2] = shapes[1][1][3] = 1;
		/* 七 (左）*/
		shapes[2][0][0] = shapes[2][0][1] = shapes[2][1][1] = shapes[2][2][1] = 1;
		/* 七 (右) */
		shapes[3][0][1] = shapes[3][0][2] = shapes[3][1][1] = shapes[3][2][1] = 1;
		/* 凸 */
		shapes[4][0][1] = shapes[4][1][0] = shapes[4][1][1] = shapes[4][2][1] = 1;
		/* 田 */
		shapes[5][1][1] = shapes[5][1][2] = shapes[5][2][1] = shapes[5][2][2] = 1;
		/* Z(左) */
		shapes[6][0][0] = shapes[6][0][1] = shapes[6][1][1] = shapes[6][1][2] = 1;
		/* Z(右)*/
		shapes[7][0][2] = shapes[7][0][1] = shapes[7][1][1] = shapes[7][1][0] = 1;
	}
};


int map[25][25];//地图  
//地图中数值为1的为边界为0的为可移动区域

 //玩家的id和得分
	
int score=0;

//----------数据--------------

//------------service-------------
/* 初始化地图 */
void initMap()

/*  
	功能: 设置地图的各种参数
	参数: void
	返回值: void

	地图为map[][]
	设置边界的值为1,可移动区域为0
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


/* 绘制地图 */
void drawMap()

/*  
	功能:打印地图
	参数: void
	返回值: void

	map[][]中的值为1时打印墙体‘■’为0时打印可移动区域‘ ’
	*/
{
	
	for (int i = 0; i < 20; i++) {
		//cout << i;
		for (int j = 0; j < 15; j++) {
			if (map[i][j] == 1) {
				std::cout << "*";	// 墙体 
			}
			else {
				std::cout << " "; 	// 可移动区域 
			}
		}
		std::cout << endl;
	}
}


/*显示分数*/

void drawNumber(int score){
	gotoXY(25, 10);
cout << "得分：" <<score << endl;
}

/*
	功能: 显示当前得分
	参数: void
	返回值: void
*/





/* 绘制方块 */
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
	功能: 画出各种方块
	参数: void
	返回值: void

	shape数组中为1的位置打印‘■’
*/


/* 清除方块 */
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

功能: 将地图中方块清除
参数: void
返回值: void

shape中值为1的位置打印‘ ’

*/


/* 添加方块到地图中 */
void addBlock(Block block)

/*

	功能: 将方块加入地图中，防止被清除
	参数: void
	返回值: void

	先获取方块左上角在地图中的坐标
	int x = block.x;
	int y = block.y;
	之后判断如果满足触底条件
	将shape 的值赋给map
	map[x+i][y+j] = block.shape[i][j];
*/
{
	int x = block.x;
	int y = block.y;
	// 先判断有没有触底
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


/*检查函数*/
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

	功能: 检查移动和旋转是否合法，以及是否死亡
	参数: int
	返回值: int

	检查俄罗斯方块与地图有重合
	如果重合输出1
	不重合输出0


*/

/*方块的移动*/
int _move(Block* block, int sign)

/*
功能: 实现方块的移动
参数: int
返回值: int

sign： 0表示向下移动， 1表示右移动，-1表示左移动，2表示加速移动
消除所有属于block的方块
重新定位block的位置，生成新的block(正常下落一个格子，按s下落三个格子)
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




/* 矩阵旋转90度 */
void rotation(Block* block) {
	// 旋转左上角3*3的矩阵，并且向director旋转90度

	/* 向左旋转 */
		/* 角转换 */
	int value = block->shape[0][0];
	block->shape[0][0] = block->shape[0][2];
	block->shape[0][2] = block->shape[2][2];
	block->shape[2][2] = block->shape[2][0];
	block->shape[2][0] = value;
	/* 十字转换 */
	value = block->shape[0][1];
	block->shape[0][1] = block->shape[1][2];
	block->shape[1][2] = block->shape[2][1];
	block->shape[2][1] = block->shape[1][0];
	block->shape[1][0] = value;



	/* 处理棍子的特殊情况 */
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
/*方块旋转*/
void _transfer( Block* block) {

	cleanBlock(*block);	 // 擦除旋转前的block
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
	drawBlock(*block);  // 重新绘制旋转后的block 
}



/* 旋转 */
void transfer(Block* block) {

	int sign = block->type;
	/*
	sign： 什么类型的方块， 不同类型的方块的旋转策略不同
	*/
	if (sign == 2 || sign == 3 || sign == 4 || sign == 1 || sign == 6 || sign == 7) { // 需要处理的
		_transfer(block);
	}

	else if (sign == 0 || sign == 5) {	// 不需要处理的 

		return;
	}


}

/*检查是否成行*/
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
	//temp数组从最后一行依次向上存；
	int index = 18;
	//遍历map数组，找到所有未成行的行，并将这些行信息赋值给temp
	for (int i = 18; i >= 0; i--) {
		int j = 1;
		for (; j <= 13; j++) {
			if (map[i][j] !=1) break;
		}
		//j<14说明并未成行；
		if (j < 14) {
			
			//将该行信息复制到temp中；
			for (int m = 1; m <= 13; m++) {
				temp[index][m] = map[i][m];
			}
			index--;
		}
		else
		{
			//消除后加分；
			score += 10;
		}
		
	}
	//重置map数组；
	initMap();
	//将temp数组信息赋值给map；
	for (int i = 0; i <= 18; i++) {
		for (int j = 1; j <= 13; j++) {
			map[i][j] = temp[i][j];
		}
	}
	delete[] array;
	delete[] temp;
}

/*
	功能: 检查是否成行，成行加分并消除
	参数: void
	返回值: void

	检测一行都为1之后将该行以上的纵坐标都-1覆盖

*/
//---------------service----------------

//---------------view-------------------

/*菜单界面*/
void menuView() {
	int n = 0;
	std::cout << "1.开始游戏";
	std::cout << endl << "2.查看开发人员";
	std::cout << endl << "3.查看教程";
	std::cout << endl << "4.退出游戏" << endl;
	std::cout << "请输入选择：";
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
		std::cout << "     教程      " << endl;
		std::cout << "ad左右移动方块" << endl;
		std::cout << "加速下移方块" << endl;
		std::cout << "w顺时针旋转方块" << endl;
		system("pause");
		system("cls");
		menuView();
		break;
		/*
		功能: 绘制教程菜单
		参数: void
		返回值: void


		按任意键返回 system("pause")
		system("cls")清屏再次调用开始菜单
	*/
	case 4:
		exit(0);
		break;
	}
}
/*

	功能: 绘制开始菜单
	参数: void
	返回值: void

	按相应按键调用相应函数

*/

	/*开发者成员界面*/
void tutorView()
{
	system("cls");
	char a = ' ';
	system("pause");
	system("cls");
	menuView();
}

/*
	功能: 绘制开发者成员界面
	参数: void
	返回值: void

	按任意键返回 system("pause")
	system("cls")清屏再次调用开始菜单
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

/*运行游戏*/
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
			cout << "游戏失败,最终得分："<<score<<"分，请按任意键返回菜单" << endl;
			system("pause");
			system("cls");
			menuView();
			
		}
		//system("pause");
	}
}

/*
	功能: 运行游戏
	参数: void
	返回值: void


	用while(true)调用各种函数进行游戏检测输入调用不同函数
	{
	   void initMap(int** map) {};
	   void drawMap(int** map) {};
	   ....

	  判断失败或退出游戏时 break退出循环


	}

*/
//---------------view-------------------
int main()
{
    menuView();
	
	return 0;
}