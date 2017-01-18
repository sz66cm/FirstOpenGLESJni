#include <stdio.h>
#include "cmshader.h"
#include "esUtil.h"

#define UNIT_SIZE 0.15f

const char* vertexShaderCode = "uniform mat4 uMVPMatrix; 	\n"\
    "uniform mat4 uLookAtMatrix; 				\n"\
    "uniform mat4 uTMatrix; 					\n"\
    "uniform mat4 uRXMatrix; 					\n"\
    "uniform mat4 uRYMatrix; 					\n"\
    "uniform mat4 uRZMatrix; 					\n"\
    "attribute vec3 aPosition; 					\n"\
    "attribute vec2 aTexCoor; 					\n"\
    "varying vec2 vTextureCoord;				\n"\
    "void main() 						\n"\
    "{ 								\n"\
    "gl_Position=uMVPMatrix*vec4(aPosition,1); 			\n"\
    "vTextureCoord=aTexCoor; 					\n"\
    "} 								\n";

const char* fragShaderCode = "precision mediump float;		\n"\
    "varying vec2 vTextureCoord;				\n"\
    "uniform sampler2D sTexture;				\n"\
    "void main()						\n"\
    "{								\n"\
    "gl_FragColor=texture2D(sTexture,vTextureCoord);		\n"\
    "}								\n";


const float vertices[] =
  { 0 * UNIT_SIZE, 11 * UNIT_SIZE, 0, -11 * UNIT_SIZE, -11 * UNIT_SIZE, 0, 11
      * UNIT_SIZE, -11 * UNIT_SIZE, 0 };
const float vertices2[] =
  { -4 * UNIT_SIZE, 0, 0,
      0, -4 * UNIT_SIZE, 0,
      4 * UNIT_SIZE, 0, 0,
      0, 4 * UNIT_SIZE, 0,
      -4 * UNIT_SIZE, 0, 0,
      4 * UNIT_SIZE, 0, 0 };
const float colors2[] =
  { 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0 };

//渲染
void
drawFrame (void* instance)
{
  Instance *ins;
  ins = (Instance *)instance;
  GLuint pP;
  pP = ins->pProgram;
  if (pP == 0)
    {
      LOGI("drawFrame() pP init fail! return");
      return;
    }
  glUseProgram (pP);
  //初始化变换矩阵
  GLfloat* muMVPMatrix;
  GLfloat* mLookAtMatrix;
  GLfloat* mProjMatrix;
  GLfloat* mRXMatrix;
  GLfloat* mRZMatrix;
  muMVPMatrix = getRotateM(NULL, 0, 0, 1, 0, 0);
  //设置沿Z轴正向移动1
  translateM(muMVPMatrix, 0, 0, 0, 1);
  //设置透视投影矩阵
  float radio = (float)ins->width / (float)ins->height;
//  float radio = 720.0f / 1134.0f;
  mProjMatrix = frustumM (NULL,//GLfloat* m
			  0,//offset
			  -radio, radio,//Left, Right
			  -1, 1,// bottom, top
			  1, 10//near, far
  );
  //设置观察矩阵
  mLookAtMatrix = setLookAtM (NULL,
			      0,
			      0, 0, 3,//eyeX, eyeY, eyeZ
			      0, 0, 0,//targetX, targetY, targetZ
			      0, 1, 0//upX, upY, upZ
  );
  //绕X轴旋转
  GLfloat xAngle = 0;
  mRXMatrix = getRotateM(NULL, 0, xAngle, 1, 0, 0);
  //绕Z轴旋转
  static GLfloat zAngle = 60;
  mRZMatrix = getRotateM(NULL, 0, zAngle, 0, 0, 1);
  ++zAngle;
  //整理矩阵
  matrixMM4(muMVPMatrix, mRXMatrix);
  matrixMM4(muMVPMatrix, mRZMatrix);
  matrixMM4(muMVPMatrix, mLookAtMatrix);
  matrixMM4(muMVPMatrix, mProjMatrix);
  //将矩阵传入着色器
  //	1.获取着色器总变换矩阵引用
  GLint muMVPMatrixHandle;
  muMVPMatrixHandle = glGetUniformLocation(pP, "uMVPMatrix");
  //	2.输入变换矩阵数据
  glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, muMVPMatrix);
  //	3.释放
  free(mRXMatrix);
  free(mRZMatrix);
  free(mProjMatrix);
  free(mLookAtMatrix);
  free(muMVPMatrix);
  //为顶点着色器输入着色数据
  //	1.获取顶点着色器位置引用
  GLint maPositionHandle;
  maPositionHandle = glGetAttribLocation(pP, "aPosition");
  //	2.输入数据
  glVertexAttribPointer (maPositionHandle,//GLuint indx,
			 3,//GLint size,
			 GL_FLOAT,//GLenum type,
			 GL_FALSE,//GLboolean normalized,
			 3 * 4,//GLsizei stride,
			 vertices2//const GLvoid* ptr
  );
  //为顶点着色器输入颜色数据
  //	1. 获取顶点着色器颜色引用
  GLint maColorHandle;
  maColorHandle = glGetAttribLocation(pP, "aColor");
  //	2. 输入数据
  glVertexAttribPointer	(maColorHandle,//GLuint index,
			 4,//GLint size R G B A
			 GL_FLOAT,//GLenum type
			 GL_FALSE,//GLboolean normalized
			 4 * 4,//GLsizei stride
			 colors2
  );
  //允许顶点位置数据数组
  glEnableVertexAttribArray(maPositionHandle);
  glEnableVertexAttribArray(maColorHandle);
  //绘制三角形
  glDrawArrays (GL_TRIANGLES,//GLenum mode,
		0,//GLint first,
		6//GLsizei count
  );
}
