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

/*!\file AlgorithmParameterGeneratorSpi.h
 * \ingroup CXX_SECURITY_m
 */

#ifndef _CLASS_BEE_SECURITY_ALGORITHMPARAMETERGENERATORSPI_H
#define _CLASS_BEE_SECURITY_ALGORITHMPARAMETERGENERATORSPI_H

#ifdef __cplusplus

#include "beecrypt/c++/lang/Object.h"
using beecrypt::lang::Object;
#include "beecrypt/c++/security/AlgorithmParameters.h"
using beecrypt::security::AlgorithmParameters;
#include "beecrypt/c++/security/SecureRandom.h"
using beecrypt::security::SecureRandom;
#include "beecrypt/c++/security/InvalidAlgorithmParameterException.h"
using beecrypt::security::InvalidAlgorithmParameterException;
#include "beecrypt/c++/security/InvalidParameterException.h"
using beecrypt::security::InvalidParameterException;
#include "beecrypt/c++/security/spec/AlgorithmParameterSpec.h"
using beecrypt::security::spec::AlgorithmParameterSpec;

namespace beecrypt {
	namespace security {
		/*!\ingroup CXX_SECURITY_m
		 */
		class BEECRYPTCXXAPI AlgorithmParameterGeneratorSpi : public Object
		{
			friend class AlgorithmParameterGenerator;

		protected:
			virtual AlgorithmParameters* engineGenerateParameters() = 0;
			virtual void engineInit(const AlgorithmParameterSpec& genParamSpec, SecureRandom* random) throw (InvalidAlgorithmParameterException) = 0;
			virtual void engineInit(int size, SecureRandom* random) throw (InvalidParameterException) = 0;

		public:
			virtual ~AlgorithmParameterGeneratorSpi() {}
		};
	}
}

#endif

#endif
