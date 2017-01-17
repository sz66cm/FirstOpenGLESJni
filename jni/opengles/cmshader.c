#include "cmshader.h"
#include <stdio.h>

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
const char* vertexShaderCode2 = "uniform mat4 uMVPMatrix; 	\n"\
    "attribute vec3 aPosition; 					\n"\
    "attribute vec4 aColor; 					\n"\
    "varying vec4 vColor;					\n"\
    "void main() 						\n"\
    "{ 								\n"\
    "gl_Position=uMVPMatrix * vec4(aPosition,1);		\n"\
    "vColor = aColor; 						\n"\
    "} 								\n";
const char* fragShaderCode = "precision mediump float;		\n"\
    "varying vec2 vTextureCoord;				\n"\
    "uniform sampler2D sTexture;				\n"\
    "void main()						\n"\
    "{								\n"\
    "gl_FragColor=texture2D(sTexture,vTextureCoord);		\n"\
    "}								\n";
const char* fragShaderCode2 = "precision mediump float;		\n"\
    "varying vec4 vColor;					\n"\
    "void main()						\n"\
    "{								\n"\
    "	gl_FragColor = vColor;					\n"\
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
drawFrame (GLuint program)
{
  GLuint pP;
  pP = program;
  if (pP == 0)
    {
      pP = initShader ();
      LOGI("drawFrame() program initShader!");
    }
  if (pP == 0)
    {
      LOGW("drawFrame() program is NULL return!");
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
  printArray("m1", muMVPMatrix);
  //������Z�������ƶ�1
  translateM(muMVPMatrix, 0, 0, 0, 1);
  printArray("mT", muMVPMatrix);
  //����͸��ͶӰ����
  float radio = 720.0f / 1134.0f;
  mProjMatrix = frustumM (NULL,//GLfloat* m
			  0,//offset
			  -radio, radio,//Left, Right
			  -1, 1,// bottom, top
			  1, 10//near, far
  );
  printArray("mPM", mProjMatrix);
  //���ù۲����
  mLookAtMatrix = setLookAtM (NULL,
			      0,
			      0, 0, 3,//eyeX, eyeY, eyeZ
			      0, 0, 0,//targetX, targetY, targetZ
			      0, 1, 0//upX, upY, upZ
  );
  printArray("mVM", mLookAtMatrix);
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

//��ʼ����ɫ��
GLuint
initShader ()
{
  //������ɫ����ʼ��
  GLuint pVertexShader = glCreateShader (GL_VERTEX_SHADER);
  if (pVertexShader != 0)
    {	//�������ɹ�,�ͼ���shader
	//����shader��Դ����
      glShaderSource (pVertexShader, 1, &vertexShaderCode2, NULL);
      //����shader
      glCompileShader (pVertexShader);
      //��ȡShader�������
      GLint vertexShaderStatu = -1;
      glGetShaderiv (pVertexShader, GL_COMPILE_STATUS, &vertexShaderStatu);
      if (vertexShaderStatu != 0)
	{
	  LOGI("initShader() vertexShaderStatu = %d, CompileShader succuss!",
	       vertexShaderStatu);
	}
      else
	{
	  GLint logLen;
	  logLen = 0;
	  glGetShaderiv (pVertexShader,		//GLuint shader
	      GL_INFO_LOG_LENGTH,		//GLenum pname
	      &logLen);
	  GLchar* pVLog = (GLchar*) malloc (sizeof(GLchar) * logLen);
	  glGetShaderInfoLog (pVertexShader,		//GLuint shader
	      logLen,		//GLsizei maxLength
	      NULL,		//GLsizei *length
	      pVLog		//GLchar *infoLog
	      );
	  LOGW(
	      "initShader() vertexShaderStatu = %d, CompileShader fail! info : %s",
	      vertexShaderStatu, pVLog);
	  free (pVLog);
	  glDeleteShader (pVertexShader);		//�����ͷ�ָ��
	}
    }
  else
    {
      LOGW("initShader() pVertexShader glCreateShader(GL_VERTEX_SHADER) fail!");
    }

  //ƬԪ��ɫ����ʼ��
  GLuint pFragShader = glCreateShader (GL_FRAGMENT_SHADER);
  if (pFragShader != 0)
    {		//ƬԪ��ɫ��ָᘄ����ɹ�
		//���dshader��Դ���a
      glShaderSource (pFragShader, 1, &fragShaderCode2, NULL);
      //���gshader
      glCompileShader (pFragShader);
      //�@ȡ��ɫ�����g��r
      GLint fragShaderStatu = -1;
      glGetShaderiv (pFragShader,		//GLuint shader
	  GL_COMPILE_STATUS,		//GLenum pname
	  &fragShaderStatu		//GLint *params
	  );
      if (fragShaderStatu != 0)
	{
	  LOGI("initShader() fragShaderStatu = %d, CompileShader succuss!",
	       fragShaderStatu);
	}
      else
	{
	  GLint logLen;
	  logLen = 0;
	  glGetShaderiv (pFragShader,		//GLuint shader
	      GL_INFO_LOG_LENGTH,		//GLenum pname
	      &logLen);
	  GLchar* pFLog = (GLchar*) malloc (sizeof(GLchar) * logLen);
	  glGetShaderInfoLog (pFragShader,		//GLuint shader
	      logLen,		//GLsizei maxLength
	      NULL,		//GLsizei *length
	      pFLog		//GLchar *infoLog
	      );
	  LOGW(
	      "initShader() fragShaderStatu = %d, CompileShader fail! info : %s",
	      fragShaderStatu, pFLog);
	  free (pFLog);
	  glDeleteShader (pFragShader);
	}
    }
  else
    {
      LOGW("initShader() pFragShader glCreateShader(GL_FRAGMENT_SHADER) fail!");
    }

  //��������
  GLuint pProgram = glCreateProgram ();
  if (pProgram != 0)
    {
      //������м�����c��ɫ��
      glAttachShader (pProgram, pVertexShader);
      GLint error = glGetError ();
      if (GL_NO_ERROR != error)
	{
	  LOGW("initShader() AttachShader(pVertexShader) error!");
	}
      //������м���ƬԪ��ɫ��
      glAttachShader (pProgram, pFragShader);
      error = glGetError ();
      if (GL_NO_ERROR != error)
	{
	  LOGW("initShader() AttachShader(pFragShader) error!");
	}
      //���ӳ���
      glLinkProgram (pProgram);
      //��ȡ���ӳɹ�program
      GLint linkProgramStatu = 0;
      glGetProgramiv (pProgram,		//GLuint program
	  GL_LINK_STATUS,		//GLenum pname
	  &linkProgramStatu		//GLint *params
	  );
      if (linkProgramStatu != GL_TRUE)
	{
	  GLint logLen;		//�洢Log�ĳ���
	  glGetProgramiv (pProgram,		//GLuint program
	      GL_INFO_LOG_LENGTH,		//GLenum pname
	      &logLen		//GLint *params
	      );
	  GLchar* logStr = (GLchar*) malloc (sizeof(GLchar) * logLen);
	  glGetProgramInfoLog (pProgram,		//GLuint program
	      logLen,		//GLsizei maxLength
	      NULL,		//GLsizei *length
	      logStr		//GLchar *infoLog
	      );
	  LOGW("initShader() glLinkProgram fail! info : %s", logStr);
	  glDeleteProgram (pProgram);
	  free (logStr);
	}
      else
	{
	  LOGI("initShader() glLinkProgram success!");
	}
    }
  else
    {
      LOGW("initShader() pProgram glCreateProgram() fail!");
    }
  return pProgram;
}
