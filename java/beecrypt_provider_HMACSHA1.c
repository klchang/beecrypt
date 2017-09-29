#if HAVE_CONFIG_H
# include "config.h"
#endif

#include "beecrypt/hmacsha1.h"

#if JAVAGLUE

#if HAVE_STDLIB_H
# include <stdlib.h>
#endif
#if HAVE_MALLOC_H
# include <malloc.h>
#endif

#include "beecrypt/java/beecrypt_provider_HMACSHA1.h"

jlong JNICALL Java_beecrypt_provider_HMACSHA1_allocParam(JNIEnv* env, jclass dummy)
{
	jlong param = (jlong) malloc(sizeof(hmacsha1Param));
	if (param == 0)
	{
		jclass ex = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}

	sha1Reset((sha1Param*) param);

	return param;
}

jlong JNICALL Java_beecrypt_provider_HMACSHA1_cloneParam(JNIEnv* env, jclass dummy, jlong param) 
{
	jlong clone = (jlong) malloc(sizeof(hmacsha1Param));
	if (clone == 0)
	{
		jclass ex = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}

	memcpy((void*) clone, (void*) param, sizeof(sha1Param));

	return clone;
}

void JNICALL Java_beecrypt_provider_HMACSHA1_freeParam(JNIEnv* env, jclass dummy, jlong param)
{
	if (param)
		free((void*) param);
}

jbyteArray JNICALL Java_beecrypt_provider_HMACSHA1_doFinal__J(JNIEnv* env, jclass dummy, jlong param)
{
	jbyte digest[20];
	jbyteArray result = (*env)->NewByteArray(env, 20);

	hmacsha1Digest((hmacsha1Param*) param, digest);

	(*env)->SetByteArrayRegion(env, result, 0, 20, digest);

	return result;
}

void JNICALL Java_beecrypt_provider_HMACSHA1_init(JNIEnv* env, jclass dummy, jlong param, jbyteArray rawkey)
{
	jbyte* data = (*env)->GetByteArrayElements(env, rawkey, 0);
	if (data)
	{
		jint len = (*env)->GetArrayLength(env, rawkey);

		hmacsha1Setup((hmacsha1Param*) param, data, len);

		(*env)->ReleaseByteArrayElements(env, rawkey, data, JNI_ABORT);
	}
	else
	{
		jclass ex = (*env)->FindClass(env, "java/lang/NullPointerException");
		if (ex)
			(*env)->ThrowNew(env, ex, (const char*) 0);
	}
}

void JNICALL Java_beecrypt_provider_HMACSHA1_reset(JNIEnv* env, jclass dummy, jlong param)
{
	hmacsha1Reset((hmacsha1Param*) param);
}

void JNICALL Java_beecrypt_provider_HMACSHA1_update__JB(JNIEnv* env, jclass dummy, jlong param, jbyte input)
{
	hmacsha1Update((hmacsha1Param*) param, &input, 1);
}

void JNICALL Java_beecrypt_provider_HMACSHA1_update__J_3BII(JNIEnv* env, jclass dummy, jlong param, jbyteArray input, jint off, jint len)
{
	jbyte* data = (*env)->GetByteArrayElements(env, input, 0);
	if (data)
	{
		hmacsha1Update((hmacsha1Param*) param, data+off, len);

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
