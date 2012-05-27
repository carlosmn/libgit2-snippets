
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

static int cb_update_tips(
		const char *ref,
		const git_oid *old,
		const git_oid *new,
		void *data)
{
	char *hex;

	fprintf(stderr, "%s:\t", ref);

	hex = git_oid_allocfmt(old);
	fprintf(stderr, "%s -> ", hex);
	free(hex);

	hex = git_oid_allocfmt(new);
	fprintf(stderr, "%s\n", hex);
	free(hex);

	return 0;
}

static int cb_auth(git_auth_cb_data *data)
{
	char username[128];
	char *password;

	printf("Username: ");
	fgets(username, 128, stdin);
	username[strlen(username)-1] = '\0';

	password = getpass("Password: ");

	data->username = strdup(username);
	data->password = strdup(password);

	return 0;
}

int cmd_fetch(git_repository *repo, int argc, const char **argv)
{
	int error;
	git_remote *r;
	git_off_t bytes = 0;
	git_indexer_stats stats;
	git_auth_cb_data auth;

	if (argc < 1)
		die("usage: ./git fetch <remote>");

	error = git_remote_load(&r, repo, argv[0]);
	if (error < 0)
		die_giterror();

	git_remote_callback(r, GIT_CALLBACK_AUTH, cb_auth, &auth);
	git_remote_callback(r, GIT_CALLBACK_UPDATE_TIPS, cb_update_tips, NULL);

	error = git_remote_connect(r, GIT_DIR_FETCH);
	if (error < 0)
		die_giterror();

	error = git_remote_download(r, &bytes, &stats);
	if (error < 0)
		die_giterror();

	git_remote_disconnect(r);

	fprintf(stderr, "Received %d/%d objects in %" PRId64 " bytes\n",
		stats.processed, stats.total, bytes);

	error = git_remote_update_tips(r);
	if (error < 0)
		die_giterror();

	git_remote_free(r);
	free(auth.username);
	free(auth.password);

	return error;
}
