
#include <stdio.h>

#include "common.h"

int cmd_fetch(git_repository *repo, int argc, const char **argv)
{
	int error;
	char *pack;
	git_remote *r;

	if (argc < 1)
		die("usage: ./git fetch <remote>");

	error = git_remote_load(&r, repo, argv[0]);
	if (error < GIT_SUCCESS)
		die_giterror();

	error = git_remote_connect(r, GIT_DIR_FETCH);
	if (error < GIT_SUCCESS)
		die_giterror();

	error = git_remote_download(&pack, r);
	if (error < GIT_SUCCESS)
		die_giterror();

	fprintf(stderr, "fetched pack: %s\n", pack);

	git_remote_disconnect(r);

	git_remote_update_tips(r);

	git_remote_free(r);

	return error;
}
