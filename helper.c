
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

int cb_auth(http_auth_data *auth_data, void *data)
{
	char username[128];
	char *password;

	printf("Username: ");
	fgets(username, 128, stdin);
	username[strlen(username)-1] = '\0';

	password = getpass("Password: ");

	auth_data->username = strdup(username);
	auth_data->password = strdup(password);

	return 0;
}
