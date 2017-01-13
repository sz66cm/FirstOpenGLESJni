#ifndef CMSHADER_H
#define CMSHADER_H
//include Header
#include <android/log.h>
#include <stdlib.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif

#define LOG_TAG "cmopengles"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define GL_VERTEX_SHADER 35633
#define GL_FRAGMENT_SHADER 0x8b30
#define GL_COMPILE_STATUS 35713
#define GL_LINK_STATUS 35714
#define GL_INFO_LOG_LENGTH 0x8b84
#define GL_TRUE 1

#define GL_FLOAT 0x1406

GLuint initShader();

void drawFrame(GLuint program);
