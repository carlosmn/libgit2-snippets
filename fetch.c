
#include <stdio.h>

#include "common.h"

static int cb_update_tips(const char *ref, const git_oid *old, const git_oid *new)
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

int cmd_fetch(git_repository *repo, int argc, const char **argv)
{
	int error;
	git_remote *r;
	git_off_t bytes = 0;
	git_indexer_stats stats;

	if (argc < 1)
		die("usage: ./git fetch <remote>");

	error = git_remote_load(&r, repo, argv[0]);
	if (error < GIT_SUCCESS)
		die_giterror();

	error = git_remote_connect(r, GIT_DIR_FETCH);
	if (error < GIT_SUCCESS)
		die_giterror();

	error = git_remote_download(r, &bytes, &stats);
	if (error < GIT_SUCCESS)
		die_giterror();

	git_remote_disconnect(r);

	fprintf(stderr, "Received %d/%d objects in %" PRId64 " bytes\n",
		stats.processed, stats.total, bytes);

	error = git_remote_update_tips(r, cb_update_tips);
	if (error < GIT_SUCCESS)
		die_giterror();

	git_remote_free(r);

	return error;
}
