/*
 * Copyright 2023-2024 Tom Schwindl <schwindl@posteo.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS
 * IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ref: https://pubs.opengroup.org/onlinepubs/9699919799/utilities/uname.html
 */
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "util.h"

static void
usage(void)
{
	die(1, "usage: uname [-amnrsv]");
}

static void
print_info(char const *const val, bool const flag)
{
	static bool skip = true;

	if (!flag)
		return;
	if (!skip)
		putchar(' ');
	skip = false;
	fputs(val, stdout);
}

int
main(int argc, char *argv[])
{
	bool mflag = false;
	bool nflag = false;
	bool rflag = false;
	bool sflag = false;
	bool vflag = false;
	int opt;
	struct utsname uts;

	x_setup("uname", argv);
	while ((opt = getopt(argc, argv, "amnrsv")) != -1) {
		switch (opt) {
		case 'a':
			mflag = nflag = rflag = sflag = vflag = true;
			break;
		case 'm':
			mflag = true;
			break;
		case 'n':
			nflag = true;
			break;
		case 'r':
			rflag = true;
			break;
		case 's':
			sflag = true;
			break;
		case 'v':
			vflag = true;
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	if (!(mflag || nflag || rflag || sflag || vflag))
		sflag = true;
	if (uname(&uts))
		die(1, "%s: uname:", prog_);

	print_info(uts.sysname,  sflag);
	print_info(uts.nodename, nflag);
	print_info(uts.release,  rflag);
	print_info(uts.version,  vflag);
	print_info(uts.machine,  mflag);
	putchar('\n');
}
