#include"view_login.h"
#include"view.h"
#include<iostream>
#include<mysql/mysql.h>
#include<stdio.h>
#include<string>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<json/json.h>
#include<sys/socket.h>
#include<stdlib.h>
using namespace std;

view_login::view_login(void *mpcon)
{
	_mpcon = (MYSQL *)mpcon;
}

void view_login::process(Json::Value val, int cli_fd)
{
	_cli_fd = cli_fd;
	
	MYSQL *mpcon = this->_mpcon;
	MYSQL_RES *mp_res;
	MYSQL_ROW mp_row;

	if(mysql_select_db(mpcon, "talk"))
	{
		cerr << "select fail：errno：" << errno << endl;
		return;
	}	
    
	//访问usr表
	string cmd ("SELECT * FROM user WHERE NAME='';");
	cmd.insert(cmd.size() - 2, val["name"].asString().c_str());
	if(mysql_real_query(mpcon, cmd.c_str(), strlen(cmd.c_str())))
	{
		cerr << "0 query fail;errno:" << errno << endl;
		return;
	}

	mp_res = mysql_store_result(mpcon);
	mp_row = mysql_fetch_row(mp_res);
    if(mp_row == 0)
    {
        //没有这个用户
        _flag = false;
        return;
    }
	if(strcmp(val["pw"].asString().c_str(), mp_row[1]) != 0)
	{
		//密码比对不上
		_flag = false;
		return;
	}

	//访问online
	cmd = "SELECT * FROM online WHERE NAME='';";
	cmd.insert(cmd.size() - 2, val["name"].asString().c_str());
	if(mysql_real_query(mpcon, cmd.c_str(), strlen(cmd.c_str())))
	{
		cerr << "0 query fail;errno:" << errno << endl;
		return;
	}

	mp_res = mysql_store_result(mpcon);
	mp_row = mysql_fetch_row(mp_res);

	if(mp_row != 0 && strcmp(val["name"].asString().c_str(), mp_row[1]) == 0)
	{
		_flag = false;
		return;
	}

	cmd = "INSERT INTO online VALUES('','');";
	char buff[sizeof(int)] = {0};
	sprintf(buff, "%d", cli_fd);
	cmd.insert(cmd.size() - 6, buff);
	cmd.insert(cmd.size() - 3, val["name"].asString().c_str());

	if(mysql_real_query(mpcon, cmd.c_str(), strlen(cmd.c_str())))
	{
		cerr << "0 query fail;errno:" << errno << endl;
		return;
	}

	//访问offline
	cmd = "SELECT * FROM offline WHERE NAME='';";
	cmd.insert(cmd.size() - 2, val["name"].asString().c_str());
	if(mysql_real_query(mpcon, cmd.c_str(), strlen(cmd.c_str())))
	{
		cerr << "0 query fail;errno:" << errno << endl;
		return;
	}

	mp_res = mysql_store_result(mpcon);
	mp_row = mysql_fetch_row(mp_res);
    if(mp_row == 0)
    {
	    _message = "\0";
    }
    else
    {
        _message = mp_row[1];
    }


	cmd = "DELETE FROM offline WHERE NAME='';";
	cmd.insert(cmd.size() - 2, val["name"].asString().c_str());
	mp_res = mysql_store_result(mpcon);
	if(mysql_real_query(mpcon, cmd.c_str(), strlen(cmd.c_str())))
	{
		cerr << "0 query fail;errno:" << errno << endl;
		return;
	}
    _flag = true;
}

void view_login::responce()
{
	if(_flag)
	{
		//登陆成功
        //遗留 buff放满了 】怎么办
		char buff[16] = "登陆成功！";
		/*if(_message.size() != 0)
		{
			strcat(buff, "[");
			strcat(buff, _message.c_str());
			strcat(buff, "]");
		}*/
		send(_cli_fd, buff, strlen(buff), 0);
		send(_cli_fd, _message.c_str(), strlen(_message.c_str()), 0);
	}
	else
	{
		//登陆失败
		char buff[] = "登陆失败，输入密码错误或账号在异地登陆";
		send(_cli_fd, buff, strlen(buff), 0);
		send(_cli_fd, (char *)0, sizeof(int), 0);
	}
	//send(string     _cli_fd);
}
 
