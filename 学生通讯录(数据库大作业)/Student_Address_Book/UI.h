#pragma once
#include"StudentManager.h"

class UI
{

public:
	void showLoginPage();
	int getLogin();
	void showAddressBook();
	int getUserChoice();

	void printStudents(vector<student> StuList);
	void printGroups(vector<group> StuList);

};

