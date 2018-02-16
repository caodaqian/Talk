#pragma once
#ifndef VIEW_REGISTER_H
#define VIEW_REGISTER_H

#include "view.h"
#include <mysql/mysql.h>
#include <string>
using namespace std;

class view_register: public view
{
public:
	view_register(void *mpcon);
	~view_register(){}
	virtual void process(Json::Value val, int cli_fd);
	virtual void responce();
private:
	string _message;
	int _cli_fd;
	bool _flag;
    MYSQL *_mpcon;
};

#endif // !VIEW_REGISTER_H
