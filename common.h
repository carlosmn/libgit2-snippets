#ifndef COMMON_H
#define COMMON_H

#include <git2.h>

#define NORETURN __attribute__((noreturn))

void NORETURN die(const char *, ...);
void NORETURN die_giterror();

typedef int (*cmd)(git_repository *repo, int, const char **);

int cmd_fetch(git_repository *, int, const char **);
int cmd_pack_objects(git_repository *, int, const char **);
int cmd_push(git_repository *, int, const char **);

int cb_auth(http_auth_data *auth_data, void *data);

#endif /* COMMON_H */
