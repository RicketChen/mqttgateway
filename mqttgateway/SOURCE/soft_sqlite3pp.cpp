#include "soft_sqlite3pp.h"
#include "MyData.h"

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

using namespace sqlite3pp;
using std::cout;
using std::endl;


MySqlite::MySqlite(char* dbname)
{
	db = database(dbname);
	transaction xct(db, true);
}
MySqlite::~MySqlite()
{

}
int MySqlite::selectfrom(char* tablename,char* format,...)
{
	char dest[100];
	char dbcmd[200];

	va_list	arg_list;
	va_start(arg_list, format); //一定要“...”之前的那个参数
	vsprintf(dest, format, arg_list);
	va_end(arg_list);
	sprintf(dbcmd, "SELECT %s FROM %s", dest, tablename);

	query qry(db, dbcmd);

	cout << "qry is " << qry.column_count() << endl;

	for (int i = 0; i < qry.column_count(); ++i)
	{
		cout << qry.column_name(i) << "\t";
	}
	cout << endl;

	for (query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		for (int j = 0; j < qry.column_count(); ++j)
		{
			cout << (*i).get<const char*>(j) << "\t";
		}
		cout << endl;
	}
	return 0;
}
int MySqlite::GetAllInfo()
{
//	query qry(db, "SELECT id,ServerLink,ClientId,UserName,Password FROM MqttParam");

	query qry(db, "SELECT Name FROM VarParam");

	for (int i = 0; i < qry.column_count(); ++i)
	{
		cout << qry.column_name(i) << "\t";
	}
	cout << endl;
	int j = 0;
	for (query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
//		cout << (*i).get<const char*>(0) << endl;
		strcpy(MqttInfo[j].url, (*i).get<const char*>(0));
//		MqttInfo[j].id = (*i).get<int>(0);
//		strcpy(MqttInfo[j].url, (*i).get<const char*>(0));
//		strcpy(MqttInfo[j].clientid, (*i).get<const char*>(2));
//		strcpy(MqttInfo[j].username, (*i).get<const char*>(3));
//		strcpy(MqttInfo[j].password, (*i).get<const char*>(4));
		j++;
	}
	for(j--;j >= 0;j--)
		cout << "data is " <<  MqttInfo[j].url<< endl;
}
int MySqlite::GetCountFromTable(char* tablename)
{
	char cmd[100];
	sprintf(cmd, "%s%s", "Select count(*) from ", tablename);
	query qry(db, cmd);
	for (query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		cout << (*i).get<int>(0) << endl;
	}
}