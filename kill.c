/* See LICENSE file for copyright and license details. */
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/wait.h>
#include <unistd.h>

#include "util.h"

struct {
	const char *name;
	int sig;
} sigs[] = {
	{ "0", 0 },
#define SIG(n) { #n, SIG##n }
	SIG(ABRT), SIG(ALRM), SIG(BUS),  SIG(CHLD), SIG(CONT), SIG(FPE),  SIG(HUP),
	SIG(ILL),  SIG(INT),  SIG(KILL), SIG(PIPE), SIG(QUIT), SIG(SEGV), SIG(STOP),
	SIG(TERM), SIG(TSTP), SIG(TTIN), SIG(TTOU), SIG(USR1), SIG(USR2), SIG(URG),
#undef SIG
};

const char *sig2name(int);
int name2sig(const char *);

static void
usage(void)
{
	weprintf("usage: %s [-s signame | -signum | -signame] pid ...\n", argv0);
	weprintf("       %s -l [exit_status]\n", argv0);
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *end;
	int ret = 0;
	int sig = SIGTERM;
	pid_t pid;
	size_t i;

	argv0 = argv[0];
	if (argc < 2)
		usage();

	argc--, argv++;
	if (strcmp(argv[0], "-l") == 0) {
		argc--, argv++;
		if (argc == 0) {
			for (i = 0; i < LEN(sigs); i++)
				puts(sigs[i].name);
			exit(0);
		} else if (argc > 1)
			usage();
		errno = 0;
		sig = strtol(argv[0], &end, 10);
		if (*end != '\0' || errno != 0)
			eprintf("%s: bad signal number\n", argv[0]);
		if (sig > 128)
			sig = WTERMSIG(sig);
		puts(sig2name(sig));
		exit(0);
	}

	if (strcmp(argv[0], "-s") == 0) {
		argc--, argv++;
		if (argc == 0)
			usage();
		sig = name2sig(argv[0]);
		argc--, argv++;
	} else if (argv[0][0] == '-') {
		if (isdigit(argv[0][1])) {
			/* handle XSI extension -signum */
			errno = 0;
			sig = strtol(&argv[0][1], &end, 10);
			if (*end != '\0' || errno != 0)
				eprintf("%s: bad signal number\n", &argv[0][1]);
			sig2name(sig);
			argc--, argv++;
		} else if (argv[0][1] != '-') {
			/* handle XSI extension -signame */
			sig = name2sig(&argv[0][1]);
			argc--, argv++;
		}
	}

	if (argc > 0 && strcmp(argv[0], "--") == 0)
		argc--, argv++;

	if (argc == 0)
		usage();

	for (; argc; argc--, argv++) {
		errno = 0;
		pid = strtol(argv[0], &end, 10);
		if (*end == '\0' && errno == 0) {
			if (kill(pid, sig) < 0) {
				weprintf("kill %d:", pid);
				ret = 1;
			}
		} else {
			weprintf("%s: bad pid\n", argv[0]);
			ret = 1;
		}
	}

	exit(ret);
}

const char *
sig2name(int sig)
{
	size_t i;

	for (i = 0; i < LEN(sigs); i++)
		if (sigs[i].sig == sig)
			return sigs[i].name;
	eprintf("%d: bad signal number\n", sig);
	/* unreachable */
	return NULL;
}

int
name2sig(const char *name)
{
	size_t i;

	for (i = 0; i < LEN(sigs); i++)
		if (strcasecmp(sigs[i].name, name) == 0)
			return sigs[i].sig;
	eprintf("%s: bad signal name\n", name);
	/* unreachable */
	return -1;
}
