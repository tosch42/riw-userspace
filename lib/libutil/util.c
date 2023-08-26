/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

void
die(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fprintf(stderr, "%s ", fmt);
		perror(NULL);
	}

	exit(1);
}

int
output_file(int fd)
{
	ssize_t n;
	Buf *buf = buf_create(BUFLEN);

	while ((n = read(fd, buf->data, buf->size)) > 0)
		xwrite_all(STDOUT_FILENO, buf->data, n);
	buf_free(buf);

	return (n < 0) ? 1 : 0;
}

ssize_t
write_all(int fd, void *buf, size_t count)
{
	ssize_t n = 0;
	ssize_t i = n;

	while (count) {
		if ((n = write(fd, (char *)buf+i, count)) == -1)
			return n;
		count -= n;
		i += n;
	}

	return count;
}

/* FIXME: awfully inefficient */
ssize_t
get_line(int fd, Buf *buf)
{
	ssize_t n;
	ssize_t ret = 0;
	size_t i = 0;
	char c;

	while ((n = read(fd, &c, 1)) > 0) {
		if ((ret += n) >= buf->size) {
			buf->size *= 2;
			buf->data = xrealloc(buf->data, buf->size);
		}

		buf->data[i++] = c;
		if (c == '\n') {
			buf->data[i] = '\0';
			break;
		}
	}
	if (n <= 0)
		ret = n;

	return ret;
}