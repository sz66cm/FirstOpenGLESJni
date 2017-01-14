#include <jni.h>
#include "cmshader.h"

void initVertexData();

JNIEXPORT jint JNI_OnLoad(JavaVM* pVM, void* reserved) {
	LOGI("cmopengles.c JNI_OnLoad() start!");
	return JNI_VERSION_1_6;
}

JNIEXPORT jlong Java_com_cm_cmopengljni_CmOpenGL_onSurfaceCreate(JNIEnv* env,
		jobject thiz) {
	LOGI("CmOpenGL_onSurfaceCreate()");
	glClearColor(0.5f,0.5f,0.5f, 1.0f);
}

JNIEXPORT jlong Java_com_cm_cmopengljni_CmOpenGL_onSurfaceChanged(JNIEnv* env,
		jobject thiz, jlong ptr, jint width, jint height) {
	LOGI("CmOpenGL_onSurfaceChanged()");
	glViewport(0, 0, width, height);
}

JNIEXPORT jlong Java_com_cm_cmopengljni_CmOpenGL_onDrawFrame(JNIEnv* env,
		jobject thiz, jlong ptr) {
//	LOGI("CmOpenGL_onDrawFrame()");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	static int xx = 0;
//	if (xx == 0)
//	{
		drawFrame(0);
//		xx++;
//		LOGI("xx == 0 only one!");
//	}
}

//初始化顶点数据
void initVertexData()
{
	int vCount = 3;
	float UNIT_SIZE = 0.15f;
	float vertices[] =
	        {
	        	0*UNIT_SIZE,11*UNIT_SIZE,0,
	        	-11*UNIT_SIZE,-11*UNIT_SIZE,0,
	        	11*UNIT_SIZE,-11*UNIT_SIZE,0,
	        };
}

