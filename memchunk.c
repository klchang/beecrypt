/*
 * Copyright (c) 2001 X-Way Rights BV
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

/*!\file memchunk.c
 * \author Bob Deblier <bob.deblier@telenet.be>
 */

#define BEECRYPT_DLL_EXPORT

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "beecrypt/memchunk.h"

memchunk* memchunkAlloc(size_t size)
{
	memchunk* tmp = (memchunk*) calloc(1, sizeof(memchunk));

	if (tmp)
	{
		tmp->size = size;
		tmp->data = (byte*) malloc(size);

		if (tmp->data == (byte*) 0)
		{
			free(tmp);
			tmp = 0;
		}
	}

	return tmp;
}

void memchunkInit(memchunk* m)
{
	m->data = (byte*) 0;
	m->size = 0;
}

void memchunkWipe(memchunk* m)
{
	if (m)
	{
		if (m->data)
		{
			memset(m->data, 0, m->size);
		}
	}
}

void memchunkFree(memchunk* m)
{
	if (m)
	{
		if (m->data)
		{
			free(m->data);

			m->size = 0;
			m->data = (byte*) 0;
		}
		free(m);
	}
}

memchunk* memchunkResize(memchunk* m, size_t size)
{
	if (m)
	{
		if (m->data)
			m->data = (byte*) realloc(m->data, size);
		else
			m->data = (byte*) malloc(size);

		if (m->data == (byte*) 0)
		{
			free(m);
			m = (memchunk*) 0;
		}
		else
			m->size = size;
	}

	return m;
}

memchunk* memchunkClone(const memchunk* m)
{
	if (m)
	{
		memchunk* tmp = memchunkAlloc(m->size);

		if (tmp)
			memcpy(tmp->data, m->data, m->size);

		return tmp;
	}

	return (memchunk*) 0;
}
