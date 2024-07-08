#include "Controller.h"
#include <windows.h>


Controller::Controller(UI& u, StudentManager& s)
{
	ui = &u;
	stu = &s;
}

Controller::~Controller()
{
	ui = NULL;
	stu = NULL;
}

void Controller::start()
{
	ui->showLoginPage();
	int login = ui->getLogin();
	if (login == 0)	return;

	system("cls");
	bool flag = false;
	while(true)
	{
		ui->showAddressBook();
		int choice = ui->getUserChoice();

		system("cls");
		switch(choice)
		{
		case 1:	// 打印学生
		{
			vector<student> stus = stu->get_AllStudents();
			ui->printStudents(stus);
			break;
		}
		case 2:	// 添加学生
		{
			student t;
			cout << "请输入添加学生的学号、姓名、班级号" << endl;
			cin >> t.student_id >> t.name >> t.class_id;
			t.group_id = 0;
			stu->InsertStudent_info(t);
			cout << "添加成功" << endl;
			break;
		}
		case 3:	// 删除学生
		{
			int stu_id;
			cout << "请输入要删除的学生的学号" << endl;
			cin >> stu_id;
			stu->Delete(stu_id);
			cout << "删除成功" << endl;
			break;
		}
		case 4: // 更新学生
		{
			student t;
			cout << "请输入更改的学生的学号、姓名、班级号和组号" << endl;
			cin >> t.student_id >> t.name >> t.class_id >> t.group_id;
			stu->Update(t);
			cout << "更新成功" << endl;
			break;
		}
		case 5: // 通过学号找学生
		{
			int stu_id;
			cout << "请输入要寻找的学生的学号" << endl;
			cin >> stu_id;
			student t = stu->FindStudent(stu_id);
			cout << "查找结果:" << endl;
			cout << t.student_id << "   " << t.name << "   " << t.class_id << "   " << t.group_id << endl;
			break;
		}
		case 6: // 打印组
		{
			vector<group> grps = stu->get_AllGroups();
			ui->printGroups(grps);
			break;
		}
		case 7: // 学生分组
		{
			cout << "先输入要成为一组的学生的学号:（输入0表示结束）" << endl;
			vector<int> stugroup_ids;
			int id;
			while(cin>>id&&id!=0)
			{
				stugroup_ids.push_back(id);
			}

			string group_name;
			cout << "输入组名:" << endl;
			cin >> group_name;
			stu->Group(stugroup_ids, group_name); 
			break;
		}
		case 8: // 通过组号寻找一组学生
		{
			int group_id;
			cout << "输入要查找组的组号:" << endl;
			cin >> group_id;
			vector<student> tmp = stu->get_GroupStudent(group_id);
			ui->printStudents(tmp);
			break;
		}
		case 0:
		{
			flag = true;
			break;
		}
		}

		Sleep(2000);
		system("cls");
		if (flag)	break;
	}

}
