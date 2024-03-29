/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "util.h"

static const char *getpwd(const char *cwd);

static void
usage(void)
{
	eprintf("usage: %s [-LP]\n", argv0);
}

int
main(int argc, char *argv[])
{
	char *cwd;
	char mode = 'L';

	ARGBEGIN {
	case 'L':
	case 'P':
		mode = ARGC();
		break;
	default:
		usage();
	} ARGEND;

	cwd = agetcwd();
	puts((mode == 'L') ? getpwd(cwd) : cwd);

	return 0;
}

static const char *
getpwd(const char *cwd)
{
	const char *pwd;
	struct stat cst, pst;

	if (!(pwd = getenv("PWD")) || pwd[0] != '/' || stat(pwd, &pst) < 0)
		return cwd;
	if (stat(cwd, &cst) < 0)
		eprintf("stat %s:", cwd);
	if (pst.st_dev == cst.st_dev && pst.st_ino == cst.st_ino)
		return pwd;
	else
		return cwd;
}
