#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "beecrypt/sha512.h"

#if JAVAGLUE

#if HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_MALLOC_H
# include <malloc.h>
#endif

#include "beecrypt/java/beecrypt_provider_SHA512.h"

jlong JNICALL Java_beecrypt_provider_SHA512_allocParam(JNIEnv* env, jclass dummy)
{
	jlong param = (jlong) malloc(sizeof(sha512Param));
	if (param == 0)
	{
		jclass ex = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}

	sha512Reset((sha512Param*) param);

	return param;
}

jlong JNICALL Java_beecrypt_provider_SHA512_cloneParam(JNIEnv* env, jclass dummy, jlong param) 
{
	jlong clone = (jlong) malloc(sizeof(sha512Param));
	if (clone == 0)
	{
		jclass ex = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}

	memcpy((void*) clone, (void*) param, sizeof(sha512Param));

	return clone;
}

void JNICALL Java_beecrypt_provider_SHA512_freeParam(JNIEnv* env, jclass dummy, jlong param)
{
	if (param)
		free((void*) param);
}

jbyteArray JNICALL Java_beecrypt_provider_SHA512_digest__J(JNIEnv* env, jclass dummy, jlong param)
{
	jbyte digest[64];
	jbyteArray result = (*env)->NewByteArray(env, 64);

	sha512Digest((sha512Param*) param, digest);

	(*env)->SetByteArrayRegion(env, result, 0, 64, digest);

	return result;
}

jint JNICALL Java_beecrypt_provider_SHA512_digest__J_3BII(JNIEnv* env, jclass dummy, jlong param, jbyteArray buf, jint off, jint len)
{
	if (len < 64)
	{
		jclass ex = (*env)->FindClass(env, "java/security/DigestException");
		if (ex)
			(*env)->ThrowNew(env, ex, "len must be at least 64");
	}
	else
	{
		jbyte digest[64];

		sha512Digest((sha512Param*) param, digest);

		(*env)->SetByteArrayRegion(env, buf, off, 64, digest);

		return 64;
	}
}

void JNICALL Java_beecrypt_provider_SHA512_reset(JNIEnv* env, jclass dummy, jlong param)
{
	sha512Reset((sha512Param*) param);
}

void JNICALL Java_beecrypt_provider_SHA512_update__JB(JNIEnv* env, jclass dummy, jlong param, jbyte input)
{
	sha512Update((sha512Param*) param, &input, 1);
}

void JNICALL Java_beecrypt_provider_SHA512_update__J_3BII(JNIEnv* env, jclass dummy, jlong param, jbyteArray input, jint off, jint len)
{
	jbyte* data = (*env)->GetByteArrayElements(env, input, 0);
	if (data)
	{
		sha512Update((sha512Param*) param, data+off, len);

		(*env)->ReleaseByteArrayElements(env, input, data, JNI_ABORT);
	}
	else
	{
		jclass ex = (*env)->FindClass(env, "java/lang/NullPointerException");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}
}

#endif
