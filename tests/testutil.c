/*
 * Copyright (c) 2003 Bob Deblier
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "beecrypt/beecrypt.h"

#include <string.h>

int fromhex(byte* data, const char* hexdata)
{
	int length = strlen(hexdata);
	int count = 0, index = 0;
	byte b = 0;
	char ch;

	if (length & 1)
		count = 1;

	while (index++ < length)
	{
		ch = *(hexdata++);

		b <<= 4;
		if (ch >= '0' && ch <= '9')
			b += (ch - '0');
		else if (ch >= 'A' && ch <= 'F')
			b += (ch - 'A') + 10;
		else if (ch >= 'a' && ch <= 'f')
			b += (ch - 'a') + 10;

		count++;
		if (count == 2)
		{
			*(data++) = b;
			b = 0;
			count = 0;
		}
	}
	return (length+1) >> 1;
}

int hexdump(const byte* data, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
	{
		printf("%02x", data[i]);
		if ((i & 0xf) == 0xf)
			printf("\n");
		else
			printf(" ");
	}
	if ((i & 0xf))
		printf("\n");
}
