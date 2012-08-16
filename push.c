
#include <stdio.h>

#include "common.h"

int cmd_push(git_repository *repo, int argc, const char **argv)
{
	struct git_remote_callbacks callbacks;
	git_remote *remote;
	git_push *push;

	if (argc-- < 2)
		die("usage: ./git push <remote> <refspec>...");

	if (git_remote_load(&remote, repo, argv[0]) < 0)
		return -1;

	callbacks.http_auth = cb_auth;
	git_remote_set_callbacks(remote, &callbacks);

	if (git_push_new(&push, remote) < 0)
		return -1;

	while (argc) {
		if (git_push_add_refspec(push, argv[argc--]) < 0)
			return -1;
	}

	if (git_push_finish(push) < 0)
		return -1;

	git_push_free(push);
	git_remote_free(remote);

	return 0;
}
