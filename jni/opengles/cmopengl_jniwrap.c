#include <jni.h>
#include "cmshader.h"
#include "matrix.h"

JNIEXPORT jint
JNI_OnLoad (JavaVM* pVM, void* reserved)
{
  LOGI("cmopengles.c JNI_OnLoad() start!");
  return JNI_VERSION_1_6;
}

JNIEXPORT jlong
Java_com_cm_cmopengljni_CmOpenGL_onSurfaceCreate (JNIEnv* env, jobject thiz)
{
  LOGI("CmOpenGL_onSurfaceCreate()");
  glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
}

JNIEXPORT jlong
Java_com_cm_cmopengljni_CmOpenGL_onSurfaceChanged (JNIEnv* env, jobject thiz,
						   jlong ptr, jint width,
						   jint height)
{
  LOGI("CmOpenGL_onSurfaceChanged() width = %d, height = %d", width, height);
  glViewport (0, 0, width, height);
}

JNIEXPORT jlong
Java_com_cm_cmopengljni_CmOpenGL_onDrawFrame (JNIEnv* env, jobject thiz,
					      jlong ptr)
{
  glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  drawFrame(0);
}

