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

//��Ⱦ
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
  //��ʼ���任����
  GLfloat* muMVPMatrix;
  GLfloat* mLookAtMatrix;
  GLfloat* mProjMatrix;
  GLfloat* mRXMatrix;
  GLfloat* mRZMatrix;
  muMVPMatrix = getRotateM(NULL, 0, 0, 1, 0, 0);
  //������Z�������ƶ�1
  translateM(muMVPMatrix, 0, 0, 0, 1);
  //����͸��ͶӰ����
  float radio = (float)ins->width / (float)ins->height;
//  float radio = 720.0f / 1134.0f;
  mProjMatrix = frustumM (NULL,//GLfloat* m
			  0,//offset
			  -radio, radio,//Left, Right
			  -1, 1,// bottom, top
			  1, 10//near, far
  );
  //���ù۲����
  mLookAtMatrix = setLookAtM (NULL,
			      0,
			      0, 0, 3,//eyeX, eyeY, eyeZ
			      0, 0, 0,//targetX, targetY, targetZ
			      0, 1, 0//upX, upY, upZ
  );
  //��X����ת
  GLfloat xAngle = 0;
  mRXMatrix = getRotateM(NULL, 0, xAngle, 1, 0, 0);
  //��Z����ת
  static GLfloat zAngle = 60;
  mRZMatrix = getRotateM(NULL, 0, zAngle, 0, 0, 1);
  ++zAngle;
  //�������
  matrixMM4(muMVPMatrix, mRXMatrix);
  matrixMM4(muMVPMatrix, mRZMatrix);
  matrixMM4(muMVPMatrix, mLookAtMatrix);
  matrixMM4(muMVPMatrix, mProjMatrix);
  //����������ɫ��
  //	1.��ȡ��ɫ���ܱ任��������
  GLint muMVPMatrixHandle;
  muMVPMatrixHandle = glGetUniformLocation(pP, "uMVPMatrix");
  //	2.����任��������
  glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, muMVPMatrix);
  //	3.�ͷ�
  free(mRXMatrix);
  free(mRZMatrix);
  free(mProjMatrix);
  free(mLookAtMatrix);
  free(muMVPMatrix);
  //Ϊ������ɫ��������ɫ����
  //	1.��ȡ������ɫ��λ������
  GLint maPositionHandle;
  maPositionHandle = glGetAttribLocation(pP, "aPosition");
  //	2.��������
  glVertexAttribPointer (maPositionHandle,//GLuint indx,
			 3,//GLint size,
			 GL_FLOAT,//GLenum type,
			 GL_FALSE,//GLboolean normalized,
			 3 * 4,//GLsizei stride,
			 vertices2//const GLvoid* ptr
  );
  //Ϊ������ɫ��������ɫ����
  //	1. ��ȡ������ɫ����ɫ����
  GLint maColorHandle;
  maColorHandle = glGetAttribLocation(pP, "aColor");
  //	2. ��������
  glVertexAttribPointer	(maColorHandle,//GLuint index,
			 4,//GLint size R G B A
			 GL_FLOAT,//GLenum type
			 GL_FALSE,//GLboolean normalized
			 4 * 4,//GLsizei stride
			 colors2
  );
  //������λ����������
  glEnableVertexAttribArray(maPositionHandle);
  glEnableVertexAttribArray(maColorHandle);
  //����������
  glDrawArrays (GL_TRIANGLES,//GLenum mode,
		0,//GLint first,
		6//GLsizei count
  );
}
