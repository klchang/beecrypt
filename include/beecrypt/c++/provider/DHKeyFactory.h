/*
 * Copyright (c) 2004 X-Way Rights BV
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

/*!\file DHKeyFactory.h
 * \ingroup CXX_PROVIDER_m
 */

#ifndef _CLASS_DHKEYFACTORY_H
#define _CLASS_DHKEYFACTORY_H

#ifdef __cplusplus

#include "beecrypt/c++/security/KeyFactorySpi.h"
using beecrypt::security::InvalidKeyException;
using beecrypt::security::Key;
using beecrypt::security::KeyFactorySpi;
using beecrypt::security::PrivateKey;
using beecrypt::security::PublicKey;
using beecrypt::security::spec::InvalidKeySpecException;
using beecrypt::security::spec::KeySpec;

namespace beecrypt {
	namespace provider {
		class DHKeyFactory : public KeyFactorySpi
		{
		protected:
			virtual PrivateKey* engineGeneratePrivate(const KeySpec&) throw (InvalidKeySpecException);
			virtual PublicKey* engineGeneratePublic(const KeySpec&) throw (InvalidKeySpecException);

			virtual KeySpec* engineGetKeySpec(const Key&, const type_info&) throw (InvalidKeySpecException);

			virtual Key* engineTranslateKey(const Key&) throw (InvalidKeyException);

		public:
			DHKeyFactory();
			virtual ~DHKeyFactory() {}
		};
	}
}

#endif

#endif
