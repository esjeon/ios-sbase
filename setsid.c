/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	eprintf("usage: %s cmd [arg ...]\n", argv0);
}

int
main(int argc, char *argv[])
{
	int savederrno;

	ARGBEGIN {
	default:
		usage();
	} ARGEND;

	if (argc < 1)
		usage();

	if (getpgrp() == getpid()) {
		switch (fork()) {
		case -1:
			eprintf("fork:");
		case 0:
			break;
		default:
			return 0;
		}
	}
	if (setsid() < 0)
		eprintf("setsid:");
	execvp(argv[0], argv);
	savederrno = errno;
	weprintf("execvp %s:", argv[0]);
	return (savederrno == ENOENT) ? 127 : 126;
}
