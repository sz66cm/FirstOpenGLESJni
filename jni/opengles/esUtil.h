/***������������һЩOpenGL ES 2.0 �Ķ���****/
#ifndef _CM_ES_UTIL_
#define _CM_ES_UTIL_
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "matrix.h"

#define LOG_TAG "esUtil"

#define LOGI_EU(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW_EU(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)

/**
 * ��ʼ����ɫ��
 */
GLint
initShader(const char * strShaderCode, unsigned int shaderType);

/**
 * �����ɫ����ʼ��״̬
 */
GLint
checkInitShader(GLint pShader);

/**
 * ��ʼ����Ⱦ����
 */
GLint
initProgram(GLuint* shaderArray, GLint size);

/**
 * ����������״̬
 */
GLint
checkLinkProgram(GLint pProgram);


#endif
