#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<mysql.h>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

typedef struct student
{
	int student_id;	// 主键
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

	static StudentManager* GetInstance()	// 单例模式
	{
		static StudentManager DBManager;
		return &DBManager;
	}

	bool InsertStudent_info(student& t);
	bool InsertGroup_info(string group_name);
	bool Update(student& t);
	bool Delete(int student_id);
	student FindStudent(int student_id);		// 通过id搜索1个学生
	vector<student> get_GroupStudent(int group_id);

	void Group(vector<int> student_ids,string student_name);		// 分组

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
