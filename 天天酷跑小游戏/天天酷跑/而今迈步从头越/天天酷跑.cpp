#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include"tools.h"

#define WIN_Width 990
#define WIN_Height 550
#define Win_Score 100
#define OBSTACLE_COUNT 10

//开关
int update;
int down;
int Jump_1;
int Jump_2;
int Status; /*-1下蹲 0奔跑 1一阶段跳跃 2二阶段跳跃*/
//背景
IMAGE Bgs[3];
int BgX[3] = {0};
int BgX_Speed[3] = {3,5,12};
IMAGE Floor;
IMAGE pause;
//角色
IMAGE Heros_run[12];
int HeroX;
int HeroY;
int HeroIndex;
int score;
IMAGE imgSZ[10];

int Jump_high;
int Jump_off;
int Jump_high2;
int r;/*主要是二阶段不好搞*/
int one;

IMAGE Hero_down[2];
IMAGE Hero_up1[2];
IMAGE Hero_up2[12];
IMAGE Hero_up2r[9];

int HeroBlood;
//障碍物
typedef enum
{
	MISSLE,
	HOOK1,
	HOOK2,
	HOOK3,
	HOOK4,
	TALL,
	SHORT1,
	SHORT2,
	OBSTACLE_TYPE_COUNT
}obstacle_type;
struct obstacle
{
	int type;
	int x, y;
	int speed;
	int power;
	int get_score;

	bool exist;
	bool hited;
	bool passed;
};
struct obstacle All_obstacles[OBSTACLE_COUNT];
IMAGE obstacleImgs[OBSTACLE_TYPE_COUNT];
//其他
IMAGE scoreFrame;
IMAGE biaoxian;
IMAGE fen;
IMAGE LOSE;
IMAGE Out;
MOUSEMSG Msg;/*鼠标消息*/


void init()
{
	//创建窗口&&开关
	initgraph(WIN_Width, WIN_Height);
	update = true;

	Status = 0;
	down = false;
	Jump_1 = false;
	Jump_2 = false;

	//特定数值加载
	score = 0;
	Jump_high = 100;
	Jump_off = 10;
	Jump_high2 = Jump_high;
	one = 0;
	r = 0;

	HeroBlood = 100;

	char name[64];/*加载各种图片*/
	//加载背景图片
	for (int i = 0; i < 3; i++)
	{
		sprintf(name, "res/bg/bg00%d.png", i + 1);
		loadimage(&Bgs[i], name);
	}

	loadimage(&Floor, "res/bg/floor.png");
	loadimage(&pause, "res/pause.png");

	//加载角色图片
	HeroX = 0.25 * WIN_Width;
	HeroY = 0.45 * WIN_Height;
	HeroIndex = 0;
	//奔跑
	for (int i = 0; i < 12; i++)
	{
		sprintf(name, "res/hero/run/hero%d.png", i + 1);
		loadimage(&Heros_run[i], name);
	}
	//下蹲
	for (int i = 0; i < 2; i++)
	{
		sprintf(name, "res/hero/down/d%d.png", i + 1);
		loadimage(&Hero_down[i], name);
	}
	//跳跃
	for (int i = 0; i < 2; i++)
	{
		sprintf(name, "res/hero/jump/jump1/j1_%d.png", i + 1);
		loadimage(&Hero_up1[i], name);
	}
	for (int i = 0; i < 12; i++)
	{
		sprintf(name, "res/hero/jump/jump2/g%d.png", i + 1);
		loadimage(&Hero_up2[i], name);
	}
	for (int i = 0; i < 9; i++)
	{
		printf(name, "res/hero/jump/jump2/h%d.png", i + 1);
		loadimage(&Hero_up2r[i], name);
	}

	//加载障碍物图片
	loadimage(&obstacleImgs[MISSLE], "res/obstacle/missile.png", 96, 48, true);
	loadimage(&obstacleImgs[HOOK1], "res/obstacle/h1.png", 80, 290, true);
	loadimage(&obstacleImgs[HOOK2], "res/obstacle/h2.png", 80, 290, true);
	loadimage(&obstacleImgs[HOOK3], "res/obstacle/h3.png", 80, 290, true);
	loadimage(&obstacleImgs[HOOK4], "res/obstacle/h4.png", 80, 290, true);
	loadimage(&obstacleImgs[TALL], "res/obstacle/tall.png", 106, 168, true);
	loadimage(&obstacleImgs[SHORT1], "res/obstacle/short1.png", 106, 75, true);
	loadimage(&obstacleImgs[SHORT2], "res/obstacle/short2.png", 100, 85, true);
	//初始化障碍物池
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		All_obstacles[i].exist = false;
	}

	//加载数字图片
	for (int i = 0; i < 10; i++)
	{
		sprintf(name, "res/score/%d.png", i);
		loadimage(&imgSZ[i], name, 20, 25, true);
	}
	loadimage(&scoreFrame, "res/score/scoreFrame.png");
	loadimage(&biaoxian, "res/score/word1.png");
	loadimage(&fen, "res/score/word2.png");

	//加载lose图片和out
	loadimage(&LOSE, "res/lose_scene/again.png", 400, 400, true);
	loadimage(&Out, "res/lose_scene/word2.png", 100, 50, true);
}

void updateBg()
{
	putimagePNG2(BgX[0], 0, &Bgs[0]);
	putimagePNG2(BgX[2], 300, &Bgs[2]);
	putimagePNG2(BgX[1], 110, &Bgs[1]);

	putimagePNG2(BgX[2], 360, &Floor);
	putimagePNG2(920,3,&pause);
}
void fly()
{
	//实现背景滚动+地板滚动
	for (int i = 0; i < 3; i++)
	{
		BgX[i] -= BgX_Speed[i];
		if (BgX[i] < -WIN_Width)
		{
			BgX[i] = 0;
		}
	}
}

void keyEvent()
{
	char ch;
	int c;

	if (_kbhit())
	{
		ch = _getch();
		if (ch == 'A')
		{
			if (Status == 0)
			{
				down = 1;
				HeroIndex = 0;
			}
		}

		if (ch == ' ')
		{
			if (Status == 0)
			{
				Jump_1 = 1;
			}
			if (Status == 1)
			{
				Jump_2 = 1;
				Jump_high2 = HeroY - (0.45 * WIN_Height - Jump_high);
				Jump_off = 10;
			}
		}
	}
}
void Judge_Status()
{

	if (down == 0 && Jump_1 == 0 && Jump_2 == 0)
		Status = 0;
	if (down == 1 && Jump_1 == 0 && Jump_2 == 0)
		Status = -1;
	if (down == 0 && Jump_1 == 1 && Jump_2 == 0)
		Status = 1;
	if (down == 0 && Jump_1 == 1 && Jump_2 == 1)
		Status = 2;
}
void updateHero()
{
	if(Status==0)
	{
		HeroY = 0.45 * WIN_Height;

		putimagePNG2(HeroX, HeroY, &Heros_run[HeroIndex]);
		HeroIndex++;
		HeroIndex %= 12;
	}
	else if (Status == -1) {
		static int count = 0;
		int delay[2] = { 5,30 };
		count++;
		if (count >= delay[HeroIndex])
		{
			count = 0;
			HeroIndex++;
			if (HeroIndex >= 2)
			{
				HeroIndex = 0;
				down = 0;
			}
		}

		int y = 390 - Hero_down[HeroIndex].getheight();
		putimagePNG2(HeroX,y, &Hero_down[HeroIndex]);
	}else if(Status == 1)
	{
		if(HeroY<Jump_high)
		{
			Jump_off = -10;
		}

		HeroY -= Jump_off;

		if(Jump_off>0)
		{
			putimagePNG2(HeroX, HeroY, &Hero_up1[0]);
		}
		else
		{
			putimagePNG2(HeroX, HeroY, &Hero_up1[1]);
		}

		if(HeroY > 0.45 * WIN_Height)
		{
			Jump_1 = 0;
			Jump_off = 10;
		}

	}else if(Status == 2)
	{
		if(HeroY<Jump_high2)
		{
			Jump_off = -12;
		}

		HeroY -= Jump_off;

		if(one==0)
		{
			putimagePNG2(HeroX, HeroY, &Hero_up2[r]);
			r++;
			if (r == 12)
				one++;
		}else
		{
			putimagePNG2(HeroX, HeroY, &Hero_up1[1]);
		}

		if(HeroY > 0.45 * WIN_Height)
		{
			Jump_1 = 0;		
			Jump_2 = 0;
			Jump_off = 10;
			one = 0;
			r = 0;
		}
	}
}

void CreatObstacle()
{	
	//先找池子里是否缺障碍物
	int i;
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (All_obstacles[i].exist == false)
		{
			break;
		}
	}

	/*分支1，都有障碍物*/
	if (i >= OBSTACLE_COUNT)
	{
		return;
	}
	/*分支2找到了不存在的并加载*/
	All_obstacles[i].exist = true;
	All_obstacles[i].hited = false;
	All_obstacles[i].passed = false;
	All_obstacles[i].type = (obstacle_type)(rand() % OBSTACLE_TYPE_COUNT);
	All_obstacles[i].x = WIN_Width;
	All_obstacles[i].y = 310;

	//speed
	if (All_obstacles[i].type == MISSLE)
	{
		All_obstacles[i].speed = 8;
		All_obstacles[i].power = 40;
		All_obstacles[i].get_score = 4;
	}
	else if (All_obstacles[i].type >= HOOK1 && All_obstacles[i].type <= HOOK4)
	{
		All_obstacles[i].speed = 0;
		All_obstacles[i].y = 0;
		All_obstacles[i].power = 20;
		All_obstacles[i].get_score = 2;
	}
	else if (All_obstacles[i].type == TALL)
	{
		All_obstacles[i].speed = 0;
		All_obstacles[i].y = 220;
		All_obstacles[i].power = 30;
		All_obstacles[i].get_score = 3;
	}
	else if (All_obstacles[i].type >= SHORT1 && All_obstacles[i].type <= SHORT2)
	{
		All_obstacles[i].speed = 0;
		All_obstacles[i].power = 10;
		All_obstacles[i].get_score = 1;
	}
}
void Count_ObstacleTime()
{
	static int frameCount = 0;
	static int enemyFre = 40;
	frameCount++;
	if(frameCount>enemyFre)
	{
		frameCount = 0;
		enemyFre = 40 + rand() % 40;
	
		CreatObstacle();
	}
}
void updateEnemy()
{
	//2.更新所有障碍物坐标 以及 图片帧
	for(int i=0;i<OBSTACLE_COUNT;i++)
	{
		if(All_obstacles[i].exist)
		{
			All_obstacles[i].x -= All_obstacles[i].speed + BgX_Speed[2];
			if(All_obstacles[i].x < -obstacleImgs[All_obstacles[i].type].getheight()*2)
			/*如果跑出去了*/
			{
				All_obstacles[i].exist = false;
			}
		}
	}

	//1.渲染障碍物图片
	for(int i=0;i<OBSTACLE_COUNT;i++)
	{
		if(All_obstacles[i].exist)
		{
			putimagePNG2(All_obstacles[i].x,All_obstacles[i].y,WIN_Width
				,&obstacleImgs[All_obstacles[i].type]);
		}
	}
}

void checkhit()
{
	for(int i=0;i<OBSTACLE_COUNT;i++)
	{
		if(All_obstacles[i].exist&&All_obstacles[i].hited==false)
		{
			//先找角色的点&&要和putimage时相互对应
			int a1x, a1y, a2x, a2y;
			int frame = 30;
			a1x = HeroX + frame;
			a1y = HeroY + frame;
			if(Status == 0)
			{
				a2x = HeroX + Heros_run[HeroIndex].getwidth() - frame;
				a2y = HeroY + Heros_run[HeroIndex].getheight();
			}else if(Status == -1)
			{
				a1y = 390 - Hero_down[HeroIndex].getheight();
				a2x = HeroX + Hero_down[HeroIndex].getheight() - frame;
				a2y = 390;
			}else if(Status == 1)
			{
				if(Jump_off>0)
				{
					a2x = HeroX + Hero_up1[0].getwidth() - frame;
					a2y = HeroY + Hero_up1[0].getheight();
				}else
				{
					a2x = HeroX + Hero_up1[1].getwidth() - frame;
					a2y = HeroY + Hero_up1[1].getheight();
				}
			}else if(Status == 2)
			{
				if(one == 0)
				{
					a2x = HeroX + Hero_up2[r].getwidth()-frame;
					a2y = HeroY + Hero_up2[r].getwidth();

				}else
				{
					a2x = HeroX + Hero_up1[1].getwidth() - frame;
					a2y = HeroY + Hero_up1[1].getwidth();
				}
			}
			//再找障碍物的点
			int b1x, b1y, b2x, b2y;
			IMAGE img = obstacleImgs[All_obstacles[i].type];

			b1x = All_obstacles[i].x + frame;
			b1y = All_obstacles[i].y + frame;
			b2x = All_obstacles[i].x + img.getwidth() - frame;
			b2y = All_obstacles[i].y + img.getheight() - 10;
			//判断碰撞
			if(rectIntersect(a1x,a1y,a2x,a2y,b1x,b1y,b2x,b2y))
			{
				mciSendString("play res/music/hit.mp3", 0, 0, 0);
				HeroBlood -= All_obstacles[i].power;
				printf("血量剩余%d", HeroBlood);
				All_obstacles[i].hited = true;
			}
		}
	}
}
void updateBloodBar()
{
	drawBloodBar(10,520,200,10,2,BLUE,DARKGRAY,RED,HeroBlood/100.0);
}

void checkScore()
{
	for(int i=0;i<OBSTACLE_COUNT;i++)
	{
		if(All_obstacles[i].exist&&
			All_obstacles[i].passed==false&&
			All_obstacles[i].hited==false&&
			All_obstacles[i].x + obstacleImgs[All_obstacles[i].type].getheight() < HeroX)
		{
			score = score + All_obstacles[i].get_score;
			All_obstacles[i].passed = true;
			printf("score:%d\n", score);
		}
	}
}
void updateScore()
{
	putimagePNG2(10,10,&scoreFrame);
	putimagePNG2(28,23,&biaoxian);
	putimagePNG2(210,23,&fen);

	//50->"50"	"5"	"5"-"0"==5
	char str[8];
	sprintf(str, "%d", score);

	int x = 185;
	int y = 24;
	int len = strlen(str);
	for(int i=len-1;i>=0;i--)
	{
		int sz = str[i] - '0';
		putimagePNG(x, y, &imgSZ[sz]);
		x -= imgSZ[sz].getwidth() + 5;
	}
}

void choice()
{
	FlushMouseMsgBuffer();
	while (1)
	{
		if(_kbhit())
		{
			_getch();
			break;
		}
	
		if(MouseHit())
		{
			Msg = GetMouseMsg();
			if(Msg.uMsg == WM_LBUTTONDOWN
				&& Msg.x >= 835 && Msg.x <= 980
				&& Msg.y >= 30 && Msg.y <= 110)
			{
				exit(0);
			}
		}
	}
}
void checkover() 
{
	if(HeroBlood<=0)
	{
		putimagePNG2(300,100,&LOSE);
		putimagePNG2(835,30,&Out);
		FlushBatchDraw();
		mciSendString("stop res/music/bg.mp3", 0, 0, 0);

		choice();
		
		mciSendString("play res/music/bg.mp3 repeat", 0, 0, 0);
		HeroBlood = 100;
		score = 0;
	}
}
void checkWin() 
{
	if(score>=Win_Score)
	{
		FlushBatchDraw();
		Sleep(1000);
		mciSendString("stop res/music/bg.mp3",0,0,0);
		mciSendString("play res/music/win.mp3",0,0,0);
		loadimage(0, "res/win_scene/win.png");
		FlushBatchDraw();

		choice();

		mciSendString("play res/music/bg.mp3 repeat", 0, 0, 0);
		HeroBlood = 100;
		score = 0;
	}
}


int main(void)
{
	//初始化及登录界面
	init();
	loadimage(0, "res/loading/load.png");

	while(1)
	{
		Msg = GetMouseMsg();
		//开始
		if(Msg.uMsg==WM_LBUTTONDOWN
			&&Msg.x>=110&&Msg.x<=270
			&&Msg.y>=280&&Msg.y<=360)
		{

			//开始加载背景音乐及正文部分
			mciSendString("play res/music/bg.mp3 repeat", 0, 0, 0);
			int timber = 0;
			while (1)
			{
				timber += getDelay();
				if (timber > 30)
				{
					update = true;
					timber = 0;
				}
				keyEvent();

				if (update)
				{
					update = false;
					Judge_Status();

					BeginBatchDraw();
					updateBg();
					fly();
					Count_ObstacleTime();
					updateHero();
					checkhit();
					updateBloodBar();
					checkScore();
					updateScore();
					updateEnemy();
					EndBatchDraw();

					checkover();
					checkWin();

					Judge_Status();
				}
			}
		}
		//退出
		if(Msg.uMsg==WM_LBUTTONDOWN
			&&Msg.x >= 110 && Msg.x <= 270
			&& Msg.y >= 400 && Msg.y <= 480)
		{
			exit(0);
		}
	}
}