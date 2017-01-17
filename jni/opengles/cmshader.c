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

//渲染
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
  //初始化变换矩阵
  GLfloat* muMVPMatrix;
  GLfloat* mLookAtMatrix;
  GLfloat* mProjMatrix;
  GLfloat* mRXMatrix;
  GLfloat* mRZMatrix;
  muMVPMatrix = getRotateM(NULL, 0, 0, 1, 0, 0);
  printArray("m1", muMVPMatrix);
  //设置沿Z轴正向移动1
  translateM(muMVPMatrix, 0, 0, 0, 1);
  printArray("mT", muMVPMatrix);
  //设置透视投影矩阵
  float radio = 720.0f / 1134.0f;
  mProjMatrix = frustumM (NULL,//GLfloat* m
			  0,//offset
			  -radio, radio,//Left, Right
			  -1, 1,// bottom, top
			  1, 10//near, far
  );
  printArray("mPM", mProjMatrix);
  //设置观察矩阵
  mLookAtMatrix = setLookAtM (NULL,
			      0,
			      0, 0, 3,//eyeX, eyeY, eyeZ
			      0, 0, 0,//targetX, targetY, targetZ
			      0, 1, 0//upX, upY, upZ
  );
  printArray("mVM", mLookAtMatrix);
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

//初始化着色器
GLuint
initShader ()
{
  //顶点着色器初始化
  GLuint pVertexShader = glCreateShader (GL_VERTEX_SHADER);
  if (pVertexShader != 0)
    {	//若创建成功,就加载shader
	//加载shader的源代码
      glShaderSource (pVertexShader, 1, &vertexShaderCode2, NULL);
      //编译shader
      glCompileShader (pVertexShader);
      //获取Shader编译情况
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
	  glDeleteShader (pVertexShader);		//销毁释放指针
	}
    }
  else
    {
      LOGW("initShader() pVertexShader glCreateShader(GL_VERTEX_SHADER) fail!");
    }

  //片元着色器初始化
  GLuint pFragShader = glCreateShader (GL_FRAGMENT_SHADER);
  if (pFragShader != 0)
    {		//片元著色器指針創建成功
		//加載shader的源代碼
      glShaderSource (pFragShader, 1, &fragShaderCode2, NULL);
      //編譯shader
      glCompileShader (pFragShader);
      //獲取著色器編譯情況
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

  //創建程序
  GLuint pProgram = glCreateProgram ();
  if (pProgram != 0)
    {
      //向程序中加入頂點著色器
      glAttachShader (pProgram, pVertexShader);
      GLint error = glGetError ();
      if (GL_NO_ERROR != error)
	{
	  LOGW("initShader() AttachShader(pVertexShader) error!");
	}
      //向程序中加入片元着色器
      glAttachShader (pProgram, pFragShader);
      error = glGetError ();
      if (GL_NO_ERROR != error)
	{
	  LOGW("initShader() AttachShader(pFragShader) error!");
	}
      //链接程序
      glLinkProgram (pProgram);
      //获取链接成功program
      GLint linkProgramStatu = 0;
      glGetProgramiv (pProgram,		//GLuint program
	  GL_LINK_STATUS,		//GLenum pname
	  &linkProgramStatu		//GLint *params
	  );
      if (linkProgramStatu != GL_TRUE)
	{
	  GLint logLen;		//存储Log的长度
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
