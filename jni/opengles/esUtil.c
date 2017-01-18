#include "esUtil.h"

/**
 * ��ʼ����ɫ��
 */
GLint
initShader(const char * strShaderCode, unsigned int shaderType)
{
  GLint shader = 0;
  //����shader����
  shader = glCreateShader(shaderType);
  //	1.�ж������Ƿ�ɹ�
  if(shader != 0)
    {
      //����ɹ�,׼������shader
      glShaderSource(shader,//GLint pShader
		     1,//num of shader
		     &strShaderCode,//String of shader code
		     NULL//const GLint* length
      );
      //����shader
      glCompileShader(shader);
      if(checkInitShader(shader) <= 0)
	{
	  shader = 0;
	  goto EXIT;
	}

    }
  EXIT:
  return shader;
}

/**
 * �����ɫ����ʼ�����
 */
GLint
checkInitShader(GLint pShader)
{
  //��ȡShader�������
  GLint shaderStatu = -1;
  glGetShaderiv(pShader, GL_COMPILE_STATUS, &shaderStatu);
  if (shaderStatu != 0)
    {
      LOGI_EU("checkInitShader() shader = %d, complie is success!", shaderStatu);
    }
  else
    {
      GLint logLen = 0;
      glGetShaderiv(pShader,//GLuint shader
		    GL_INFO_LOG_LENGTH,//GLenum pname
		    &logLen
      );
      GLchar pVLog[logLen];
      glGetShaderInfoLog(pShader,
			 logLen,
			 NULL,
			 pVLog
      );
      LOGW_EU("checkInitShader() shader = %d, CompileShader fail! info : %s",
	   pShader, pVLog
      );
      glDeleteShader(pShader);//�ͷ���ɫ�������ָ��
    }
  return shaderStatu;
}

/**
 * ��ʼ����Ⱦ����
 */
GLint
initProgram(GLuint* shaderArray, GLint size)
{
  //��������
  GLuint pProgram = glCreateProgram();
  //�ж��Ƿ��������ɹ�
  if (pProgram != 0)
    {
      int i;
      for (i = 0; i < size; ++i) {
	//������м��붥����ɫ��
	glAttachShader (pProgram, shaderArray[i]);
	GLint error = glGetError();
	if (GL_NO_ERROR != error)
	  {
	    LOGW_EU("initProgram() AttachShader[%d] error!", i);
	  }
      }
      //���ӳ���
      glLinkProgram (pProgram);
      //��ȡ����״̬
      if(checkLinkProgram(pProgram) != 0)
	{
	  LOGI_EU("initProgram() success!");
	}
      else
	{
	  pProgram = 0;
	  LOGI_EU("initProgram() glLinkProgram fail!");
	}
    }
  else
    {
      LOGW_EU("initProgram() pProgram glCreateProgram() fail!");
    }
  return pProgram;
}

/**
 * ����������״̬
 */
GLint
checkLinkProgram (GLint pProgram)
{
  GLint linkProgramStatu = 0;
  glGetProgramiv (pProgram,
  GL_LINK_STATUS,
		  &linkProgramStatu);
  if (linkProgramStatu != GL_TRUE)
    {
      GLint logLen = 0; //�洢Log����
      glGetProgramiv (pProgram,
      GL_INFO_LOG_LENGTH,
		      &logLen);
      GLchar logStr[logLen];
      glGetProgramInfoLog (pProgram, logLen,
      NULL,
			   logStr);
      LOGI_EU("glLinkProgram fail! info : %s", logStr);
      glDeleteProgram (pProgram);
    }
  else
    {
      LOGI_EU("glLinkProgram success!");
    }
  return linkProgramStatu;
}
