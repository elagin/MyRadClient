
#include <stdio.h>
#include <stdlib.h>
#include <string.h>*/
//#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
//#include <fstream>


//#include	"conf.h"
#include	"includes.h"
#include	<freeradius-client.h>
#include	"pathnames.h"

//#include "headers.h"
using namespace std;


int main (int argc, char **argv)
{
	string		username = "login-000001";
//	string            passwd = "pass-000001";

        VALUE_PAIR 	*send, *received;
	uint32_t		service;

        rc_handle	*rh = NULL;
        
//        string conf_file = "/usr/local/etc/radiusclient/radiusclient.conf";

//     	if ((rh = rc_read_config() == NULL)
//		return ERROR_RC;

//	if (rc_read_dictionary(rh, rc_conf_str(rh, "dictionary")) != 0)
//		return ERROR_RC;


        if (rc_avpair_add(rh, &send, PW_USER_NAME, username.c_str(), -1, 0) == NULL)
		return ERROR_RC;

   
}