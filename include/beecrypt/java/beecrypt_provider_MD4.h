/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class beecrypt_provider_MD4 */

#ifndef _Included_beecrypt_provider_MD4
#define _Included_beecrypt_provider_MD4
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     beecrypt_provider_MD4
 * Method:    allocParam
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_beecrypt_provider_MD4_allocParam
  (JNIEnv *, jclass);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    cloneParam
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_beecrypt_provider_MD4_cloneParam
  (JNIEnv *, jclass, jlong);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    freeParam
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_beecrypt_provider_MD4_freeParam
  (JNIEnv *, jclass, jlong);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    digest
 * Signature: (J)[B
 */
JNIEXPORT jbyteArray JNICALL Java_beecrypt_provider_MD4_digest__J
  (JNIEnv *, jclass, jlong);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    digest
 * Signature: (J[BII)I
 */
JNIEXPORT jint JNICALL Java_beecrypt_provider_MD4_digest__J_3BII
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    reset
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_beecrypt_provider_MD4_reset
  (JNIEnv *, jclass, jlong);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    update
 * Signature: (JB)V
 */
JNIEXPORT void JNICALL Java_beecrypt_provider_MD4_update__JB
  (JNIEnv *, jclass, jlong, jbyte);

/*
 * Class:     beecrypt_provider_MD4
 * Method:    update
 * Signature: (J[BII)V
 */
JNIEXPORT void JNICALL Java_beecrypt_provider_MD4_update__J_3BII
  (JNIEnv *, jclass, jlong, jbyteArray, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
