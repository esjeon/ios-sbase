/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

static void
usage(void)
{
	eprintf("usage: unlink file\n");
}

int
main(int argc, char *argv[])
{
	if (argc != 2)
		usage();

	if (unlink(argv[1]) < 0)
		eprintf("unlink: '%s':", argv[1]);

	return 0;
}
