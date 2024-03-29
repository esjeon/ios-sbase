/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	eprintf("usage: sync\n");
}

int
main(int argc, char *argv[])
{
	if (argc != 1)
		usage();
	sync();

	return 0;
}
