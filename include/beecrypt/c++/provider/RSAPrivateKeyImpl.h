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

/*!\file RSAPrivateKeyImpl.h
 * \ingroup CXX_PROVIDER_m
 */

#ifndef _CLASS_RSAPRIVATEKEYIMPL_H
#define _CLASS_RSAPRIVATEKEYIMPL_H

#ifdef __cplusplus

#include "beecrypt/c++/lang/Cloneable.h"
using beecrypt::lang::Cloneable;
#include "beecrypt/c++/lang/Object.h"
using beecrypt::lang::Object;
#include "beecrypt/c++/security/interfaces/RSAPrivateKey.h"
using beecrypt::security::interfaces::RSAPrivateKey;

namespace beecrypt {
	namespace provider {
		class RSAPrivateKeyImpl : public Object, public RSAPrivateKey, public Cloneable
		{
		protected:
			BigInteger _n;
			BigInteger _d;
			mutable bytearray* _enc;

		public:
			RSAPrivateKeyImpl(const RSAPrivateKey&);
			RSAPrivateKeyImpl(const RSAPrivateKeyImpl&);
			RSAPrivateKeyImpl(const BigInteger&, const BigInteger&);
			virtual ~RSAPrivateKeyImpl();

			virtual RSAPrivateKeyImpl* clone() const throw ();

			virtual bool equals(const Object* obj) const throw ();

			virtual const BigInteger& getModulus() const throw ();
			virtual const BigInteger& getPrivateExponent() const throw ();

			virtual const bytearray* getEncoded() const throw ();
			virtual const String& getAlgorithm() const throw ();
			virtual const String* getFormat() const throw ();
		};
	}
}

#endif

#endif
