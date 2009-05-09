/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class ru_lspl_patterns_Pattern */

#ifndef _Included_ru_lspl_patterns_Pattern
#define _Included_ru_lspl_patterns_Pattern
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getAlternativeCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Pattern_getAlternativeCount
  (JNIEnv *, jobject);

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getAlternative
 * Signature: (I)Lru/lspl/patterns/Alternative;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Pattern_getAlternative
  (JNIEnv *, jobject, jint);

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getDependencyCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ru_lspl_patterns_Pattern_getDependencyCount
  (JNIEnv *, jobject);

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getDependency
 * Signature: (I)Lru/lspl/patterns/Pattern;
 */
JNIEXPORT jobject JNICALL Java_ru_lspl_patterns_Pattern_getDependency
  (JNIEnv *, jobject, jint);

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    getSource
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Pattern_getSource
  (JNIEnv *, jobject);

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    dump
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_ru_lspl_patterns_Pattern_dump
  (JNIEnv *, jobject);

/*
 * Class:     ru_lspl_patterns_Pattern
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_ru_lspl_patterns_Pattern_finalize
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
