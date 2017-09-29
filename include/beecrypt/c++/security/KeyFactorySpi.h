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

/*!\file KeyFactorySpi.h
 * \ingroup CXX_SECURITY_m
 */

#ifndef _CLASS_BEE_SECURITY_KEYFACTORYSPI_H
#define _CLASS_BEE_SECURITY_KEYFACTORYSPI_H

#ifdef __cplusplus

#include "beecrypt/c++/lang/Object.h"
using beecrypt::lang::Object;
#include "beecrypt/c++/security/PrivateKey.h"
using beecrypt::security::PrivateKey;
#include "beecrypt/c++/security/PublicKey.h"
using beecrypt::security::PublicKey;
#include "beecrypt/c++/security/InvalidKeyException.h"
#include "beecrypt/c++/security/spec/KeySpec.h"
using beecrypt::security::spec::KeySpec;
#include "beecrypt/c++/security/spec/InvalidKeySpecException.h"
using beecrypt::security::spec::InvalidKeySpecException;

#include <typeinfo>
using std::type_info;

namespace beecrypt {
	namespace security {
		/*!\ingroup CXX_SECURITY_m
		 */
		class BEECRYPTCXXAPI KeyFactorySpi : public Object
		{
			friend class KeyFactory;

		protected:
			virtual PrivateKey* engineGeneratePrivate(const KeySpec& spec) throw (InvalidKeySpecException) = 0;
			virtual PublicKey* engineGeneratePublic(const KeySpec& spec) throw (InvalidKeySpecException) = 0;

			virtual KeySpec* engineGetKeySpec(const Key& key, const type_info& info) throw (InvalidKeySpecException) = 0;

			virtual Key* engineTranslateKey(const Key& key) throw (InvalidKeyException) = 0;

		public:
			virtual ~KeyFactorySpi() {}
		};
	}
}

#endif

#endif
