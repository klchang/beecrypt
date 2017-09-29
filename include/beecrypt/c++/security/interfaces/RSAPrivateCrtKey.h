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

/*!\file RSAPrivateCrtKey.h
 * \ingroup CXX_SECURITY_INTERFACES_m
 */

#ifndef _INTERFACE_BEE_SECURITY_INTERFACES_RSAPRIVATECRTKEY_H
#define _INTERFACE_BEE_SECURITY_INTERFACES_RSAPRIVATECRTKEY_H

#ifdef __cplusplus

#include "beecrypt/c++/security/interfaces/RSAPrivateKey.h"
using beecrypt::security::interfaces::RSAPrivateKey;

namespace beecrypt {
	namespace security {
		namespace interfaces {
			/*!\brief The interface to an RSA private key, as defined in the PKCS#1 standard, using the Chinese Remainder Theorem (CRT) information values.
			 * \ingroup CXX_SECURITY_INTERFACES_m
			 */
			class RSAPrivateCrtKey : public virtual RSAPrivateKey
			{
			public:
				virtual ~RSAPrivateCrtKey() {}

				virtual const BigInteger& getPublicExponent() const throw () = 0;
				virtual const BigInteger& getPrimeP() const throw () = 0;
				virtual const BigInteger& getPrimeQ() const throw () = 0;
				virtual const BigInteger& getPrimeExponentP() const throw () = 0;
				virtual const BigInteger& getPrimeExponentQ() const throw () = 0;
				virtual const BigInteger& getCrtCoefficient() const throw () = 0;
			};
		}
	}
}

#endif

#endif
