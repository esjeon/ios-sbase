#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "util.h"

static void
usage(void)
{
	eprintf("usage: rmdir dir...\n");
}

int
main(int argc, char **argv)
{
	argv++;;
	if(!*argv)
		usage();

	while(*argv) {
		if(rmdir(*argv++) == -1)
			fprintf(stderr, "rmdir: '%s': %s\n",
					argv[-1], strerror(errno));
	}

	return 0;
}
