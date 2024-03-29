/* See LICENSE file for copyright and license details. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

static void output(const char *, long, long, long);
static void wc(FILE *, const char *);

static int lflag = 0;
static int wflag = 0;
static char cmode = 0;
static long tc = 0, tl = 0, tw = 0;

static void
usage(void)
{
	eprintf("usage: %s [-clmw] [files...]\n", argv0);
}

int
main(int argc, char *argv[])
{
	FILE *fp;
	int i;

	ARGBEGIN {
	case 'c':
		cmode = 'c';
		break;
	case 'm':
		cmode = 'm';
		break;
	case 'l':
		lflag = 1;
		break;
	case 'w':
		wflag = 1;
		break;
	default:
		usage();
	} ARGEND;

	if (argc == 0) {
		wc(stdin, NULL);
	} else {
		for (i = 0; i < argc; i++) {
			if (!(fp = fopen(argv[i], "r"))) {
				weprintf("fopen %s:", argv[i]);
				continue;
			}
			wc(fp, argv[i]);
			fclose(fp);
		}
		if (argc > 1)
			output("total", tc, tl, tw);
	}
	return 0;
}

void
output(const char *str, long nc, long nl, long nw)
{
	int noflags = !cmode && !lflag && !wflag;

	if (lflag || noflags)
		printf(" %5ld", nl);
	if (wflag || noflags)
		printf(" %5ld", nw);
	if (cmode || noflags)
		printf(" %5ld", nc);
	if (str)
		printf(" %s", str);
	putchar('\n');
}

void
wc(FILE *fp, const char *str)
{
	int word = 0;
	int c;
	long nc = 0, nl = 0, nw = 0;

	while ((c = getc(fp)) != EOF) {
		if (cmode != 'm' || UTF8_POINT(c))
			nc++;
		if (c == '\n')
			nl++;
		if (!isspace(c))
			word = 1;
		else if (word) {
			word = 0;
			nw++;
		}
	}
	if (word)
		nw++;
	tc += nc;
	tl += nl;
	tw += nw;
	output(str, nc, nl, nw);
}
