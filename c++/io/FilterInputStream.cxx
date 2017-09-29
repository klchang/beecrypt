/*
 * Copyright (c) 2004 Beeyond Software Holding BV
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
 */

#define BEECRYPT_CXX_DLL_EXPORT

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "beecrypt/c++/io/FilterInputStream.h"

using namespace beecrypt::io;

FilterInputStream::FilterInputStream(InputStream& in) : in(in)
{
}

FilterInputStream::~FilterInputStream()
{
}

int FilterInputStream::available() throw (IOException)
{
	return in.available();
}

void FilterInputStream::close() throw (IOException)
{
	in.close();
}

void FilterInputStream::mark(int readlimit) throw ()
{
	synchronized (this)
	{
		in.mark(readlimit);
	}
}

bool FilterInputStream::markSupported() throw ()
{
	return in.markSupported();
}

int FilterInputStream::read() throw (IOException)
{
	return in.read();
}

int FilterInputStream::read(byte* data, int offset, int len) throw (IOException)
{
	return in.read(data, offset, len);
}

int FilterInputStream::read(bytearray& b) throw (IOException)
{
	return in.read(b);
}

void FilterInputStream::reset() throw (IOException)
{
	synchronized (this)
	{
		in.reset();
	}
}

int FilterInputStream::skip(int n) throw (IOException)
{
	return in.skip(n);
}
