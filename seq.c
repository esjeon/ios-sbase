/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

static int digitsleft(const char *);
static int digitsright(const char *);
static int validfmt(const char *);

static void
usage(void)
{
	eprintf("usage: %s [-f fmt] [-s separator] [-w width] [start"
		       " [step]] end\n", argv0);
}

int
main(int argc, char *argv[])
{
	const char *starts = "1", *steps = "1", *ends = "1", *sep = "\n";
	int wflag = 0;
	char *tmp, ftmp[BUFSIZ], *fmt = ftmp;
	double start, step, end, out, dir;
	int left, right;

	ARGBEGIN {
	case 'f':
		if(!validfmt(tmp=EARGF(usage())))
			eprintf("%s: invalid format\n", tmp);
		fmt = tmp;
		break;
	case 's':
		sep = EARGF(usage());
		break;
	case 'w':
		wflag = 1;
		break;
	default:
		usage();
	} ARGEND;

	switch (argc) {
	case 3:
		steps = argv[1];
		argv[1] = argv[2];
		/* fallthrough */
	case 2:
		starts = argv[0];
		argv++;
		/* fallthrough */
	case 1:
		ends = argv[0];
		break;
	default:
		usage();
	}
	start = estrtod(starts);
	step  = estrtod(steps);
	end   = estrtod(ends);

	dir = (step > 0) ? 1.0 : -1.0;
	if (step == 0 || start * dir > end * dir)
		return 1;

	if (fmt == ftmp) {
		right = MAX(digitsright(starts),
		            MAX(digitsright(ends),
		                digitsright(steps)));

		if (wflag) {
			left = MAX(digitsleft(starts), digitsleft(ends));

			snprintf(ftmp, sizeof ftmp, "%%0%d.%df",
					right + left + (right != 0), right);
		} else
			snprintf(ftmp, sizeof ftmp, "%%.%df", right);
	}
	for (out = start; out * dir <= end * dir; out += step) {
		if (out != start)
			fputs(sep, stdout);
		printf(fmt, out);
	}
	printf("\n");

	return 0;
}

static int
digitsleft(const char *d)
{
	char *exp;
	int shift;

	if (*d == '+')
		d++;
	exp = strpbrk(d, "eE");
	shift = exp ? estrtol(&exp[1], 10) : 0;

	return MAX(0, strspn(d, "-0123456789") + shift);
}

static int
digitsright(const char *d)
{
	char *exp;
	int shift, after;

	exp = strpbrk(d, "eE");
	shift = exp ? estrtol(&exp[1], 10) : 0;
	after = (d = strchr(d, '.')) ? strspn(&d[1], "0123456789") : 0;

	return MAX(0, after - shift);
}

static int
validfmt(const char *fmt)
{
	int occur = 0;

literal:
	while (*fmt)
		if (*fmt++ == '%')
			goto format;
	return occur == 1;

format:
	if (*fmt == '%') {
		fmt++;
		goto literal;
	}
	fmt += strspn(fmt, "-+#0 '");
	fmt += strspn(fmt, "0123456789");
	if (*fmt == '.') {
		fmt++;
		fmt += strspn(fmt, "0123456789");
	}
	if (*fmt == 'L')
		fmt++;

	switch (*fmt) {
	case 'f': case 'F':
	case 'g': case 'G':
	case 'e': case 'E':
	case 'a': case 'A':
		occur++;
		goto literal;
	default:
		return 0;
	}
}
