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

#include "beecrypt/c++/security/KeyPair.h"
#include "beecrypt/c++/lang/NullPointerException.h"
using beecrypt::lang::NullPointerException;

using namespace beecrypt::security;

KeyPair::KeyPair(PublicKey* pub, PrivateKey* pri) : _pub(pub), _pri(pri)
{
	if (_pub == 0 || _pri == 0)
		throw NullPointerException();
}

KeyPair::~KeyPair()
{
	delete _pub;
	delete _pri;
}

const PublicKey& KeyPair::getPublic() const throw ()
{
	return *_pub;
}

const PrivateKey& KeyPair::getPrivate() const throw ()
{
	return *_pri;
}
