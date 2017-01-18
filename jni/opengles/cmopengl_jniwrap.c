#include <jni.h>
#include "esUtil.h"
#include "cmshader.h"
#include "matrix.h"

const char* vertexShaderCode2 = "uniform mat4 uMVPMatrix; 	\n"\
    "attribute vec3 aPosition; 					\n"\
    "attribute vec4 aColor; 					\n"\
    "varying vec4 vColor;					\n"\
    "void main() 						\n"\
    "{ 								\n"\
    "gl_Position=uMVPMatrix * vec4(aPosition,1);		\n"\
    "vColor = aColor; 						\n"\
    "} 								\n";


const char* fragShaderCode2 = "precision mediump float;	\n"\
        "varying vec4 vColor;					\n"\
        "void main()						\n"\
        "{							\n"\
        "	gl_FragColor = vColor;				\n"\
        "}							\n";

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
  Instance *instance = (Instance *)malloc(sizeof(Instance));
  memset(instance, 0, sizeof(Instance));
  LOGI("onSurfaceCreate() Instance size is %d!, instance address is %p!", sizeof(Instance), instance);
  //初始化Program
  //	1.初始化各種Shader
  GLint shaders[2] = {0};
  shaders[0] = initShader(vertexShaderCode2, GL_VERTEX_SHADER);
  shaders[1] = initShader(fragShaderCode2, GL_FRAGMENT_SHADER);
  //	2.創建程序
  instance->pProgram = initProgram(shaders, 2);
  return (jlong)instance;
}

JNIEXPORT jlong
Java_com_cm_cmopengljni_CmOpenGL_onSurfaceChanged (JNIEnv* env, jobject thiz,
						   jlong ptr, jint width,
						   jint height)
{
  LOGI("onSurfaceChanged() width = %d, height = %d", width, height);
  glViewport (0, 0, width, height);
  Instance * instance = (Instance *) ptr;
  instance->width = width;
  instance->height = height;
  LOGI("onSurfaceChanged() ins->width = %d, ins->height = %d", instance->width, instance->height);
}

JNIEXPORT jlong
Java_com_cm_cmopengljni_CmOpenGL_onDrawFrame (JNIEnv* env, jobject thiz,
					      jlong ptr)
{
  glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  Instance *instance;
  instance = (Instance *)ptr;
  drawFrame(instance);
}
JNIEXPORT jlong
Java_com_cm_cmopengljni_CmOpenGL_release (JNIEnv* env, jobject thiz,
					      jlong ptr)
{
  Instance *instance;
  instance = (Instance *)ptr;
  if (instance != 0)
    {
      free(instance);
      LOGI("release() success!");
    }
  else
    {
      LOGI("release() has been free!");
    }
}

