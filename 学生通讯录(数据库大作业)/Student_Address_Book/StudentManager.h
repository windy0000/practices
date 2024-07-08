#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<mysql.h>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

typedef struct student
{
	int student_id;	// ����
	string name;
	int class_id;

	int group_id;
	
}student;

typedef struct group
{
	int group_id;
	string group_name;
}group;

class StudentManager
{
public:
	StudentManager();
	~StudentManager();

	static StudentManager* GetInstance()	// ����ģʽ
	{
		static StudentManager DBManager;
		return &DBManager;
	}

	bool InsertStudent_info(student& t);
	bool InsertGroup_info(string group_name);
	bool Update(student& t);
	bool Delete(int student_id);
	student FindStudent(int student_id);		// ͨ��id����1��ѧ��
	vector<student> get_GroupStudent(int group_id);

	void Group(vector<int> student_ids,string student_name);		// ����

	// ......
	vector<student> get_AllStudents();
	vector<group> get_AllGroups();
private:
	MYSQL* con;

	const char* host = "127.0.0.1";
	const char* user = "root";
	const char* pw = "root123";
	const char* database_name = "studentadresslist";
	const int port = 3306;
};
