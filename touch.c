/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#include "util.h"

static void touch(const char *);

static int cflag = 0;
static time_t t;

static void
usage(void)
{
	eprintf("usage: %s [-c] [-t stamp] file...\n", argv0);
}

int
main(int argc, char *argv[])
{
	t = time(NULL);

	ARGBEGIN {
	case 'c':
		cflag = 1;
		break;
	case 't':
		t = estrtol(EARGF(usage()), 0);
		break;
	default:
		usage();
	} ARGEND;

	if (argc < 1)
		usage();

	for (; argc > 0; argc--, argv++)
		touch(argv[0]);

	return 0;
}

static void
touch(const char *str)
{
	int fd;
	struct stat st;
	struct utimbuf ut;

	if (stat(str, &st) == 0) {
		ut.actime = st.st_atime;
		ut.modtime = t;
		if (utime(str, &ut) < 0)
			eprintf("utime %s:", str);
		return;
	}
	else if (errno != ENOENT)
		eprintf("stat %s:", str);
	else if (cflag)
		return;
	if ((fd = open(str, O_CREAT|O_EXCL,
	               S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) < 0)
		eprintf("open %s:", str);
	close(fd);
	touch(str);
}
