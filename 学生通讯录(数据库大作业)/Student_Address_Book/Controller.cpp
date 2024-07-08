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
		case 1:	// ��ӡѧ��
		{
			vector<student> stus = stu->get_AllStudents();
			ui->printStudents(stus);
			break;
		}
		case 2:	// ���ѧ��
		{
			student t;
			cout << "���������ѧ����ѧ�š��������༶��" << endl;
			cin >> t.student_id >> t.name >> t.class_id;
			t.group_id = 0;
			stu->InsertStudent_info(t);
			cout << "��ӳɹ�" << endl;
			break;
		}
		case 3:	// ɾ��ѧ��
		{
			int stu_id;
			cout << "������Ҫɾ����ѧ����ѧ��" << endl;
			cin >> stu_id;
			stu->Delete(stu_id);
			cout << "ɾ���ɹ�" << endl;
			break;
		}
		case 4: // ����ѧ��
		{
			student t;
			cout << "��������ĵ�ѧ����ѧ�š��������༶�ź����" << endl;
			cin >> t.student_id >> t.name >> t.class_id >> t.group_id;
			stu->Update(t);
			cout << "���³ɹ�" << endl;
			break;
		}
		case 5: // ͨ��ѧ����ѧ��
		{
			int stu_id;
			cout << "������ҪѰ�ҵ�ѧ����ѧ��" << endl;
			cin >> stu_id;
			student t = stu->FindStudent(stu_id);
			cout << "���ҽ��:" << endl;
			cout << t.student_id << "   " << t.name << "   " << t.class_id << "   " << t.group_id << endl;
			break;
		}
		case 6: // ��ӡ��
		{
			vector<group> grps = stu->get_AllGroups();
			ui->printGroups(grps);
			break;
		}
		case 7: // ѧ������
		{
			cout << "������Ҫ��Ϊһ���ѧ����ѧ��:������0��ʾ������" << endl;
			vector<int> stugroup_ids;
			int id;
			while(cin>>id&&id!=0)
			{
				stugroup_ids.push_back(id);
			}

			string group_name;
			cout << "��������:" << endl;
			cin >> group_name;
			stu->Group(stugroup_ids, group_name); 
			break;
		}
		case 8: // ͨ�����Ѱ��һ��ѧ��
		{
			int group_id;
			cout << "����Ҫ����������:" << endl;
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
