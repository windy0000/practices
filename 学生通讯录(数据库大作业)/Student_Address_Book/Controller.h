#pragma once
#include"UI.h"
#include"StudentManager.h"
class Controller
{
public:
	Controller(UI &u,StudentManager &s);
	~Controller();

	void start();
private:
	UI* ui;
	StudentManager* stu;
};

