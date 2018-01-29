#pragma once
#ifndef CONTROL_H
#define CONTROL_H

#include <map>
#include "view.h"
#include <mysql/mysql.h>
using namespace std;

extern pthread_mutex_t mutex;

typedef class control
{
public:
    control();
    //~control();

	void handle(char *buff, int cli_fd);
private:
    map<int,view*> _map;//model
}Control,*PControl;

#endif
