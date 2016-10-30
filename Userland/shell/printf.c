/*
 * The Minimal snprintf() implementation
 *
 * Copyright (c) 2013,2014 Michal Ludvig <michal@logix.cz>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the auhor nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----
 *
 * This is a minimal snprintf() implementation optimised
 * for embedded systems with a very limited program memory.
 * mini_snprintf() doesn't support _all_ the formatting
 * the glibc does but on the other hand is a lot smaller.
 * Here are some numbers from my STM32 project (.bin file size):
 *      no snprintf():      10768 bytes
 *      mini snprintf():    11420 bytes     (+  652 bytes)
 *      glibc snprintf():   34860 bytes     (+24092 bytes)
 * Wasting nearly 24kB of memory just for snprintf() on
 * a chip with 32kB flash is crazy. Use mini_snprintf() instead.
 *
 */

#include <c_string.h>
#include <stdarg.h>
#include <stdint.h>
#include <printf.h>
#include <clib.h>

static uint64_t
strlen(const int8_t *s)
{
	uint64_t len = 0;
	while (s[len] != '\0') len++;
	return len;
}

static uint64_t
itoa(int64_t value, uint64_t radix, uint64_t uppercase, uint64_t unsig,
	 int8_t *buffer, uint64_t zero_pad)
{
	int8_t	*pbuffer = buffer;
	int	negative = 0;
	uint64_t	i, len;

	/* No support for unusual radixes. */
	if (radix > 16)
		return 0;

	if (value < 0 && !unsig) {
		negative = 1;
		value = -value;
	}

	/* This builds the string back to front ... */
	do {
		int64_t digit = value % radix;
		*(pbuffer++) = (digit < 10 ? '0' + digit : (uppercase ? 'A' : 'a') + digit - 10);
		value /= radix;
	} while (value > 0);

	for (i = (pbuffer - buffer); i < zero_pad; i++)
		*(pbuffer++) = '0';

	if (negative)
		*(pbuffer++) = '-';

	*(pbuffer) = '\0';

	/* ... now we reverse it (could do it recursively but will
	 * conserve the stack space) */
	len = (pbuffer - buffer);
	for (i = 0; i < len / 2; i++) {
		int8_t j = buffer[i];
		buffer[i] = buffer[len-i-1];
		buffer[len-i-1] = j;
	}

	return len;
}

uint64_t c_atoi(uint8_t *p) {
    uint64_t k = 0;
    while (*p) {
        k = (k<<3)+(k<<1)+(*p)-'0';
        p++;
     }
     return k;
}

int
vfprintf(int8_t *fmt, va_list va)
{
	int8_t bf[24];
	int8_t ch;
	int64_t count = 0;
	int64_t len;

	while ((ch=*(fmt++))) {
		if (ch!='%') {
			putchar(ch);
			count++;
		}
		else {
			int8_t zero_pad = 0;
			int8_t *ptr;
			uint64_t len;

			ch=*(fmt++);

			/* Zero padding requested */
			if (ch=='0') {
				ch=*(fmt++);
				if (ch == '\0')
					goto end;
				if (ch >= '0' && ch <= '9')
					zero_pad = ch - '0';
				ch=*(fmt++);
			}

			switch (ch) {
				case 0:
					goto end;

				case 'u':
				case 'd':
					len = itoa(va_arg(va, uint64_t), 10, 0, (ch=='u'), bf, zero_pad);
					putchars(bf, len);
					count+=len;
					break;

				case 'x':
				case 'X':
					len = itoa(va_arg(va, uint64_t), 16, (ch=='X'), 1, bf, zero_pad);
					putchars(bf, len);
					count += len;
					break;

				case 'c' :
					putchar((int8_t)(va_arg(va, int)));
					count++;
					break;

				case 's' :
					ptr = va_arg(va, int8_t*);
					len = strlen(ptr);
					putchars(ptr, len);
					count += len;
					break;

				default:
					putchar(ch);
					count++;
					break;
			}
		}
	}
end:
	return count;
}


int
printf(int8_t *fmt, ...)
{
	int64_t ret;
	va_list va;
	va_start(va, fmt);
	ret = vfprintf(fmt, va);
	va_end(va);

	return ret;
}
