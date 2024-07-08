#include "UI.h"
#include<iostream>
#include <iomanip>
using namespace std;

void UI::showLoginPage()
{

	cout << "***********************" << endl;
	cout << " *     " << "��1����" << "     *" << endl;
	cout << endl << endl << endl << endl;
	cout << " *     " << "��0�˳�" << "     *" << endl;
	cout << "***********************" << endl;
}

int UI::getLogin()
{
	int c;
	cin >> c;
	return c;
}

void UI::showAddressBook()
{
	cout << "Welcome to Student Adress List" << endl;
	cout << endl;
	cout << "***********************" << endl;
	cout << "1. Print Student" << endl;
	cout << "2. Add Student" << endl;
	cout << "3. Delete Student" << endl;
	cout << "4. Update Student" << endl;
	cout << "5. Find a Student through student_id" << endl;
	cout << "***********************" << endl;
	cout << "6. Print Groups" << endl;
	cout << "7. Group Students" << endl;
	cout << "8. Find students through student_id" << endl;
	cout << "***********************" << endl;
	cout << "0. Exit" << endl;
	cout << "***********************" << endl;
}

int UI::getUserChoice()
{
	int choice;
	cin >> choice;

	return choice;
}

// ��ӡǰ10��ѧ������
void UI::printStudents(vector<student> StuList)
{
	cout << "ѧ����Ϣ:" << endl;
	for(auto t:StuList)
	{
		cout << left << setw(6) << t.student_id << "   " 
			<< setw(6) <<  t.name << "   " 
			<< setw(6) << t.class_id << "   "
			<< setw(6) << t.group_id << endl;
	}
}

// ��ӡǰ10��������
void UI::printGroups(vector<group> GList)
{
	cout << "����Ϣ:" << endl;
	for(auto g:GList)
	{
		cout << left << setw(6) << g.group_id << "   "
			<< setw(6) << g.group_name << endl;
	}
}

