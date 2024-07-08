#include "StudentManager.h"

StudentManager::StudentManager()
{
	con = mysql_init(NULL);
	// 设置字符编码
	mysql_options(con, MYSQL_SET_CHARSET_NAME, "GBK");
	
	if(!mysql_real_connect(con,host,user,pw,database_name,port,NULL,0))
	{
		cerr << "Failed to connect the database" << endl;
		exit(0);
	}
}

StudentManager::~StudentManager()
{
	mysql_close(con);
}

bool StudentManager::InsertStudent_info(student& t)
{
	char sql[1024];
	sprintf(sql, "INSERT INTO student_information (student_id,name,class_id,group_id) values\
		(%d,'%s',%d,%d) ", t.student_id, t.name.c_str(), t.class_id, t.group_id);

	if(mysql_query(con,sql))
	{
		cerr << "Failed to insert data:Error:" << endl << mysql_error(con) << endl;
		return false;
	}
	return true;
}

bool StudentManager::InsertGroup_info(string group_name)
{
	char sql[1024];
	sprintf(sql, "INSERT INTO student_group (group_name) values\
		('%s') ", group_name.c_str());
	if (mysql_query(con, sql))
	{
		cerr << "Failed to insert data:Error:" << endl << mysql_error(con) << endl;
		return false;
	}
	return true;
}

bool StudentManager::Update(student& t)
{
	char sql[1024];
	sprintf(sql, "UPDATE student_information \
				  SET  name = '%s',class_id = %d, group_id =%d\
				  WHERE student_id = %d", t.name.c_str(), t.class_id, t.group_id, t.student_id);

	if (mysql_query(con, sql))
	{
		cerr << "Failed to update data:Error:" << endl << mysql_error(con) << endl;
		return false;
	}
	return true;
}

bool StudentManager::Delete(int student_id)
{
	char sql[1024];
	sprintf(sql, "DELETE FROM student_information WHERE student_id = %d", student_id);
	if (mysql_query(con, sql))
	{
		cerr << "Failed to delete data:Error:" << endl << mysql_error(con) << endl;
		return false;
	}

	return true;
}

student StudentManager::FindStudent(int student_id)
{
	student ret;
	ret.student_id = 0;

	char sql[1024];
	sprintf(sql,"SELECT * FROM student_information WHERE student_id = %d",student_id);

	if(mysql_query(con,sql))
	{
		cerr << "Failed to query data: Error:" << endl << mysql_error(con) << endl;
		return {};
	}

	MYSQL_RES* res = mysql_store_result(con);
	if (res == NULL)
	{
		cerr << "Failed to get result set: Error:" << endl << mysql_error(con) << endl;
		return {};
	}
	// 检查是否有结果
	if (mysql_num_rows(res) == 0)
	{
		cerr << "No student found with ID " << student_id << endl;
		mysql_free_result(res);
		return {};
	}

	MYSQL_ROW row = mysql_fetch_row(res);

	ret.student_id = atoi(row[0]);
	ret.name = row[1];
	ret.class_id = atoi(row[2]);
	ret.group_id = atoi(row[3]);

	mysql_free_result(res);

	return ret;
}

vector<student> StudentManager::get_GroupStudent(int group_id)
{
	vector<student> StuList;
	char sql[1024];
	sprintf(sql, "SELECT * FROM student_information WHERE group_id = %d", group_id);
	if (mysql_query(con, sql))
	{
		cerr << "Failed to query data: Error:" << endl << mysql_error(con) << endl;
		return {};
	}


	MYSQL_RES* res = mysql_store_result(con);

	MYSQL_ROW row;
	while((row= mysql_fetch_row(res)))
	{
		student stu;

		stu.student_id = atoi(row[0]);
		stu.name = row[1];
		stu.class_id = atoi(row[2]);
		stu.group_id = atoi(row[3]);

		StuList.push_back(stu);
	}
	return StuList;
}

void StudentManager::Group(vector<int> student_ids,string group_name)
{
	InsertGroup_info(group_name);
	int group = mysql_insert_id(con);;

	for(int id:student_ids)
	{
		student t = FindStudent(id);
		t.group_id = group;
		Update(t);
	}
}

vector<student> StudentManager::get_AllStudents()
{
	vector<student> ret;
	char sql[1024];
	sprintf(sql, "SELECT * FROM student_information");
	if (mysql_query(con, sql))
	{
		cerr << "Failed to query data: Error:" << endl << mysql_error(con) << endl;
		return {};
	}

	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res)))
	{
		student stu;

		stu.student_id = atoi(row[0]);
		stu.name = row[1];
		stu.class_id = atoi(row[2]);
		stu.group_id = atoi(row[3]);

		ret.push_back(stu);
	}

	return ret;
}

vector<group> StudentManager::get_AllGroups()
{
	vector<group> ret;
	char sql[1024];
	sprintf(sql, "SELECT * FROM student_group");
	if (mysql_query(con, sql))
	{
		cerr << "Failed to query data: Error:" << endl << mysql_error(con) << endl;
		return {};
	}

	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res)))
	{
		group g;
		g.group_id = atoi(row[0]);
		g.group_name = row[1];

		ret.push_back(g);
	}

	return ret;
}
