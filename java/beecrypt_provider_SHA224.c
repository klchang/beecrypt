#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "beecrypt/sha224.h"

#if JAVAGLUE

#if HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_MALLOC_H
# include <malloc.h>
#endif

#include "beecrypt/java/beecrypt_provider_SHA224.h"

jlong JNICALL Java_beecrypt_provider_SHA224_allocParam(JNIEnv* env, jclass dummy)
{
	jlong param = (jlong) malloc(sizeof(sha224Param));
	if (param == 0)
	{
		jclass ex = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}

	sha224Reset((sha224Param*) param);

	return param;
}

jlong JNICALL Java_beecrypt_provider_SHA224_cloneParam(JNIEnv* env, jclass dummy, jlong param) 
{
	jlong clone = (jlong) malloc(sizeof(sha224Param));
	if (clone == 0)
	{
		jclass ex = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}

	memcpy((void*) clone, (void*) param, sizeof(sha224Param));

	return clone;
}

void JNICALL Java_beecrypt_provider_SHA224_freeParam(JNIEnv* env, jclass dummy, jlong param)
{
	if (param)
		free((void*) param);
}

jbyteArray JNICALL Java_beecrypt_provider_SHA224_digest__J(JNIEnv* env, jclass dummy, jlong param)
{
	jbyte digest[24];
	jbyteArray result = (*env)->NewByteArray(env, 24);

	sha224Digest((sha224Param*) param, digest);

	(*env)->SetByteArrayRegion(env, result, 0, 24, digest);

	return result;
}

jint JNICALL Java_beecrypt_provider_SHA224_digest__J_3BII(JNIEnv* env, jclass dummy, jlong param, jbyteArray buf, jint off, jint len)
{
	if (len < 24)
	{
		jclass ex = (*env)->FindClass(env, "java/security/DigestException");
		if (ex)
			(*env)->ThrowNew(env, ex, "len must be at least 24");
	}
	else
	{
		jbyte digest[24];

		sha224Digest((sha224Param*) param, digest);

		(*env)->SetByteArrayRegion(env, buf, off, 24, digest);

		return 24;
	}
}

void JNICALL Java_beecrypt_provider_SHA224_reset(JNIEnv* env, jclass dummy, jlong param)
{
	sha224Reset((sha224Param*) param);
}

void JNICALL Java_beecrypt_provider_SHA224_update__JB(JNIEnv* env, jclass dummy, jlong param, jbyte input)
{
	sha224Update((sha224Param*) param, &input, 1);
}

void JNICALL Java_beecrypt_provider_SHA224_update__J_3BII(JNIEnv* env, jclass dummy, jlong param, jbyteArray input, jint off, jint len)
{
	jbyte* data = (*env)->GetByteArrayElements(env, input, 0);
	if (data)
	{
		sha224Update((sha224Param*) param, data+off, len);

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
