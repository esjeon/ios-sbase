/* (C)opyright MMVI Anselm R. Garbe <garbeam at gmail dot com>
 * See LICENSE file for license details. */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

static void
usage(void)
{
	eprintf("usage: %s cmd [arg ...]\n", argv0);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	default:
		usage();
	} ARGEND;

	if (argc < 1)
		usage();

	if(getpgrp() == getpid()) {
		switch(fork()){
		case -1:
			eprintf("fork:");
		case 0:
			break;
		default:
			exit(0);
		}
	}
	if(setsid() < 0)
		eprintf("setsid:");
	execvp(argv[0], argv);
	eprintf("execvp:");
	/* NOTREACHED */
	return 0;
}