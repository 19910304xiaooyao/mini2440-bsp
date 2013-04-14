/* vi: set sw=4 ts=4: */
/*
 * Unicode support routines.
 *
 * Copyright (C) 2010 Denys Vlasenko
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */
#include "libbb.h"
#include "unicode.h"

const char* FAST_FUNC printable_string(uni_stat_t *stats, const char *str)
{
	static char *saved[4];
	static unsigned cur_saved; /* = 0 */

	char *dst;
	const char *s;

	s = str;
	while (1) {
		unsigned char c = *s;
		if (c == '\0') {
			/* 99+% of inputs do not need conversion */
			if (stats) {
				stats->byte_count = (s - str);
				stats->unicode_count = (s - str);
				stats->unicode_width = (s - str);
			}
			return str;
		}
		if (c < ' ')
			break;
#if 0 // by ck, fix chinese charset issue
		if (c >= 0x7f)
			break;
#endif
		s++;
	}

#if ENABLE_UNICODE_SUPPORT
	dst = unicode_conv_to_printable(stats, str);
#else
	{
		char *d = dst = xstrdup(str);
		while (1) {
			unsigned char c = *d;
			if (c == '\0')
				break;
#if 0
			if (c < ' ' || c >= 0x7f)
#else
			if (c < ' ')
#endif
				*d = '?';
			d++;
		}
		if (stats) {
			stats->byte_count = (d - dst);
			stats->unicode_count = (d - dst);
			stats->unicode_width = (d - dst);
		}
	}
#endif

	free(saved[cur_saved]);
	saved[cur_saved] = dst;
	cur_saved = (cur_saved + 1) & (ARRAY_SIZE(saved)-1);

	return dst;
}
