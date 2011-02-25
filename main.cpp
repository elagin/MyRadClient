
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <iostream>
//#include <fstream>
using namespace std;

//#include	"conf.h"
//#include	"includes.h"
#include	"freeradius-client.h"
//#include	"pathnames.h"
//#include        <dict.h>

//#include "headers.h"

/*
void rc_avpair_insert (VALUE_PAIR **a, VALUE_PAIR *p, VALUE_PAIR *b)
{
	VALUE_PAIR     *this_node = NULL;
	VALUE_PAIR     *vp;

	if (b->next != NULL)
	{
//		rc_log(LOG_CRIT, "rc_avpair_insert: value pair (0x%p) next ptr. (0x%p) not NULL", b, b->next);
		abort ();
	}

	if (*a == NULL)
	{
		*a = b;
		return;
	}

	vp = *a;

	if ( p == NULL) // run to end of "a" list
	{
		while (vp != NULL)
		{
			this_node = vp;
			vp = vp->next;
		}
	}
	else // look for the "p" entry in the "a" list
	{
		this_node = *a;
		while (this_node != NULL)
		{
			if (this_node == p)
			{
				break;
			}
			this_node = this_node->next;
		}
	}

	b->next = this_node->next;
	this_node->next = b;

	return;
}

int rc_avpair_assign (VALUE_PAIR *vp, const void *pval, int len)
{
	switch (vp->type)
	{
		case PW_TYPE_STRING:
			if (len == -1)
				len = (uint32_t)strlen((char *)pval);
			if (len > AUTH_STRING_LEN) {
		        	// rc_log(LOG_ERR, "rc_avpair_assign: bad attribute length");
		        	return -1;
			}
			memcpy(vp->strvalue, (char *)pval, len);
			vp->strvalue[len] = '\0';
			vp->lvalue = len;
			break;

		case PW_TYPE_DATE:
		case PW_TYPE_INTEGER:
	        case PW_TYPE_IPADDR:
			vp->lvalue = * (uint32_t *) pval;
			break;

		default:
			// rc_log(LOG_ERR, "rc_avpair_assign: unknown attribute %d", vp->type);
			return -1;
	}
	return 0;
}

VALUE_PAIR *rc_avpair_new (const rc_handle *rh, int attrid, const void *pval, int len, int vendorpec)
{
	VALUE_PAIR     *vp = NULL;
	DICT_ATTR      *pda;

	attrid = attrid | (vendorpec << 16);
	if ((pda = rc_dict_getattr (rh, attrid)) == NULL)
	{
		// rc_log(LOG_ERR,"rc_avpair_new: unknown attribute %d", attrid);
		return NULL;
	}
	if (vendorpec != 0 && rc_dict_getvend(rh, vendorpec) == NULL)
	{
		// rc_log(LOG_ERR,"rc_avpair_new: unknown Vendor-Id %d", vendorpec);
		return NULL;
	}
	if ((vp = (VALUE_PAIR*)malloc (sizeof (VALUE_PAIR))) != NULL)
	{
		strncpy (vp->name, pda->name, sizeof (vp->name));
		vp->attribute = attrid;
		vp->next = NULL;
		vp->type = pda->type;
		if (rc_avpair_assign (vp, pval, len) == 0)
		{
			//XXX: Fix up Digest-Attributes
			switch (vp->attribute) {
			case PW_DIGEST_REALM:
			case PW_DIGEST_NONCE:
			case PW_DIGEST_METHOD:
			case PW_DIGEST_URI:
			case PW_DIGEST_QOP:
			case PW_DIGEST_ALGORITHM:
			case PW_DIGEST_BODY_DIGEST:
			case PW_DIGEST_CNONCE:
			case PW_DIGEST_NONCE_COUNT:
			case PW_DIGEST_USER_NAME:
				// overlapping!
				if (vp->lvalue > AUTH_STRING_LEN - 2)
					vp->lvalue = AUTH_STRING_LEN - 2;
				memmove(&vp->strvalue[2], &vp->strvalue[0], vp->lvalue);
				vp->strvalue[0] = vp->attribute - PW_DIGEST_REALM + 1;
				vp->lvalue += 2;
				vp->strvalue[1] = vp->lvalue;
				vp->strvalue[vp->lvalue] = '\0';
				vp->attribute = PW_DIGEST_ATTRIBUTES;
			default:
				break;
			}
			return vp;
		}
		free (vp);
		vp = NULL;
	}
	else
	{
		// rc_log(LOG_CRIT,"rc_avpair_new: out of memory");
	}

	return vp;
}
 
VALUE_PAIR *rc_avpair_add (const rc_handle *rh, VALUE_PAIR **list, int attrid, const void *pval, int len, int vendorpec)
{
	VALUE_PAIR     *vp;

	vp = rc_avpair_new (rh, attrid, pval, len, vendorpec);

	if (vp != NULL)
	{
		rc_avpair_insert (list, NULL, vp);
	}

	return vp;

}
*/

struct test2{
    string aa;
    string bb;
};

int main (int argc, char **argv)
{
    int             result;
    string		username = "login-000001";
    string            passwd = "pass-000001";

    VALUE_PAIR 	*send, *received;
    uint32_t		service;

    rc_handle	*rh = NULL;

    char 		msg[4096];
    char conf_file[] = "/usr/local/etc/radiusclient/radiusclient.conf";
    char dic[] = "dictionary";

    string host = "192.168.10.102";
//        string conf_file = "/usr/local/etc/radiusclient/radiusclient.conf";


    if ((rh = rc_read_config(conf_file)) == NULL)
    {
        return ERROR_RC;
    }

    if (rc_read_dictionary(rh, rc_conf_str(rh, dic)) != 0)
            return ERROR_RC;


    if (rc_avpair_add(rh, &send, PW_USER_NAME, username.c_str(), -1, 0) == NULL)
            return ERROR_RC;

    if (rc_avpair_add(rh, &send, PW_USER_PASSWORD, passwd.c_str(), -1, 0) == NULL)
            return ERROR_RC;

    service = PW_AUTHENTICATE_ONLY;
    if (rc_avpair_add(rh, &send, PW_SERVICE_TYPE, &service, -1, 0) == NULL)
            return ERROR_RC;

    fprintf(stdout, "msg: %s\n", msg);
    result = rc_auth(rh, host.c_str(), 0, send, &received, msg);

    if (result == OK_RC)
    {
            fprintf(stderr, "\"%s\" RADIUS Authentication OK\n", username.c_str());
    }
    else
    {
            fprintf(stderr, "\"%s\" RADIUS Authentication failure (RC=%i)\n", username.c_str(), result);
    }

    return result;
}