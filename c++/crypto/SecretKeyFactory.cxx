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

#include "beecrypt/c++/crypto/SecretKeyFactory.h"
#include "beecrypt/c++/security/Security.h"
using beecrypt::security::Security;

using namespace beecrypt::crypto;

SecretKeyFactory::SecretKeyFactory(SecretKeyFactorySpi* spi, const Provider* provider, const String& algorithm)
{
	_kspi = spi;
	_prov = provider;
	_algo = algorithm;
}

SecretKeyFactory::~SecretKeyFactory()
{
	delete _kspi;
}

SecretKeyFactory* SecretKeyFactory::getInstance(const String& algorithm) throw (NoSuchAlgorithmException)
{
    Security::spi* tmp = Security::getSpi(algorithm, "SecretKeyFactory");

	assert(dynamic_cast<SecretKeyFactorySpi*>((SecretKeyFactorySpi*) tmp->cspi));

    SecretKeyFactory* result = new SecretKeyFactory((SecretKeyFactorySpi*) tmp->cspi, tmp->prov, tmp->name);

    delete tmp;

    return result;
}

SecretKeyFactory* SecretKeyFactory::getInstance(const String& algorithm, const String& provider) throw (NoSuchAlgorithmException, NoSuchProviderException)
{
    Security::spi* tmp = Security::getSpi(algorithm, "SecretKeyFactory", provider);

	assert(dynamic_cast<SecretKeyFactorySpi*>((SecretKeyFactorySpi*) tmp->cspi));

    SecretKeyFactory* result = new SecretKeyFactory((SecretKeyFactorySpi*) tmp->cspi, tmp->prov, tmp->name);

    delete tmp;

    return result;
}

SecretKeyFactory* SecretKeyFactory::getInstance(const String& algorithm, const Provider& provider) throw (NoSuchAlgorithmException)
{
    Security::spi* tmp = Security::getSpi(algorithm, "SecretKeyFactory", provider);

	assert(dynamic_cast<SecretKeyFactorySpi*>((SecretKeyFactorySpi*) tmp->cspi));

    SecretKeyFactory* result = new SecretKeyFactory((SecretKeyFactorySpi*) tmp->cspi, tmp->prov, tmp->name);

    delete tmp;

    return result;
}

SecretKey* SecretKeyFactory::generateSecret(const KeySpec& spec) throw (InvalidKeySpecException)
{
	return _kspi->engineGenerateSecret(spec);
}

KeySpec* SecretKeyFactory::getKeySpec(const SecretKey& key, const type_info& info) throw (InvalidKeySpecException)
{
	return _kspi->engineGetKeySpec(key, info);
}

SecretKey* SecretKeyFactory::translateKey(const SecretKey& key) throw (InvalidKeyException)
{
	return _kspi->engineTranslateKey(key);
}

const String& SecretKeyFactory::getAlgorithm() const throw ()
{
	return _algo;
}

const Provider& SecretKeyFactory::getProvider() const throw ()
{
	return *_prov;
}
