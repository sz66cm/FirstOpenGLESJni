#include "cmshader.h"
#include "matrix.h"

#define UNIT_SIZE 0.15f

const char* vertexShaderCode = \
						"uniform mat4 uMVPMatrix; 					\n" \
						"uniform mat4 uLookAtMatrix; 				\n" \
						"uniform mat4 uTMatrix; 					\n" \
						"uniform mat4 uRXMatrix; 					\n" \
						"uniform mat4 uRYMatrix; 					\n" \
						"uniform mat4 uRZMatrix; 					\n" \
						"attribute vec3 aPosition; 					\n" \
						"attribute vec2 aTexCoor; 					\n" \
						"varying vec2 vTextureCoord;				\n" \
						"void main() 								\n" \
						"{ 											\n" \
						"gl_Position=uMVPMatrix*vec4(aPosition,1); 		\n" \
						"vTextureCoord=aTexCoor; 					\n" \
						"} 											\n" \
						;
const char* fragShaderCode = \
		"precision mediump float;							\n" \
		"varying vec2 vTextureCoord;						\n" \
		"uniform sampler2D sTexture;						\n" \
		"void main()										\n" \
		"{													\n" \
		"gl_FragColor=texture2D(sTexture,vTextureCoord);	\n" \
		"}													\n" \
		;

const float vertices[] =
{
	0 * UNIT_SIZE, 11 * UNIT_SIZE, 0,
	-11 * UNIT_SIZE, -11 * UNIT_SIZE, 0,
	11 * UNIT_SIZE, -11 * UNIT_SIZE, 0
};

//��Ⱦ
void drawFrame(GLuint program)
{
	GLuint pP = program;
	if(pP == 0)
	{
		pP = initShader();
	}
	if(pP == 0)
	{
		LOGW("drawFrame() program is NULL return!");
		return;
	}
	glUseProgram(pP);
	//��ȡ��ɫ���б任���������
	GLfloat* mMVPMatrix = setLookAtM(NULL, 0,
			0,0,3,//eye
			0,0,0,//camera
			0,1,0//up
			);
	//ͶӰ����
	float radio = 720.0f / 1280.0f;
	GLfloat* mProjectMatrix = frustumM(NULL, 0,
			-radio, radio, //left right
			-1, 1,//bottom top
			1, 10//near far
			);
	//��Z��ǰ��һ����λ
	translateM(mMVPMatrix, 0, 0, 0, 1);
	//��ȡ��ɫ���ܱ任��������
	GLint muMVPMatrixHandle = glGetUniformLocation(
			pP,//GLuint program
			"uMVPMatrix"//const GLchar *name
	);
	//��������
	matrixMM4(mMVPMatrix, mProjectMatrix);
	//���ܱ任�����䵽��ɫ��
	glUniformMatrix4fv(
			muMVPMatrixHandle,//GLint location
			1,//GLsizei count
			GL_FALSE,//GLboolean transpose
			mMVPMatrix//const GLfloat *value
	);
	LOGI("glUniformMatrix4fv - end");
	//��ȡ��ɫ���ж�����������
	GLint pVertexAttribHandle = glGetAttribLocation(
			pP,//	GLuint program
			"aPosition"//  const GLchar *name
	);
	LOGI("pVertexAttribHandle : %d", pVertexAttribHandle);
	//Ϊ��ɫ��ָ����������
	glVertexAttribPointer(
			pVertexAttribHandle,//GLuint index
			3,//GLint size X Y Z
			GL_FLOAT,//GLenum type
			GL_FALSE,//GLboolean normalized
			3 * 4,//GLsizei stride
			vertices//const GLvoid * pointer
	);
	//������λ����������
	glEnableVertexAttribArray(pVertexAttribHandle);

	//����������
	glDrawArrays(
			GL_TRIANGLES,//GLenum mode
			0,//GLint first index
			3//GLsizei counts
	);
	LOGI("glDrawArrays - end");
	//�ͷ��ڴ�
	free(mProjectMatrix);
	free(mMVPMatrix);
}

//��ʼ����ɫ��
GLuint initShader()
{
	//������ɫ����ʼ��
	GLuint pVertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (pVertexShader != 0)
	{//�������ɹ�,�ͼ���shader
		//����shader��Դ����
		glShaderSource(pVertexShader, 1, &vertexShaderCode, NULL);
		//����shader
		glCompileShader(pVertexShader);
		//��ȡShader�������
		GLint vertexShaderStatu = -1;
		glGetShaderiv(pVertexShader, GL_COMPILE_STATUS, &vertexShaderStatu);
		if (vertexShaderStatu != 0)
		{
			LOGI("initShader() vertexShaderStatu = %d, CompileShader succuss!", vertexShaderStatu);
		} else
		{
			GLint logLen;
			logLen = 0;
			glGetShaderiv(
				pVertexShader,//GLuint shader
				GL_INFO_LOG_LENGTH,//GLenum pname
				&logLen
			);
			GLchar* pVLog = (GLchar*)malloc(sizeof(GLchar) * logLen);
			glGetShaderInfoLog(
				pVertexShader,//GLuint shader
				logLen,//GLsizei maxLength
				NULL,//GLsizei *length
				pVLog//GLchar *infoLog
			);
			LOGW("initShader() vertexShaderStatu = %d, CompileShader fail! info : %s", vertexShaderStatu, pVLog);
			free(pVLog);
			glDeleteShader(pVertexShader);//�����ͷ�ָ��
		}
	} else {
		LOGW("initShader() pVertexShader glCreateShader(GL_VERTEX_SHADER) fail!");
	}

	//ƬԪ��ɫ����ʼ��
	GLuint pFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if(pFragShader != 0)
	{//ƬԪ��ɫ��ָᘄ����ɹ�
		//���dshader��Դ���a
		glShaderSource(pFragShader, 1, &fragShaderCode, NULL);
		//���gshader
		glCompileShader(pFragShader);
		//�@ȡ��ɫ�����g��r
		GLint fragShaderStatu = -1;
		glGetShaderiv(
				pFragShader,//GLuint shader
				GL_COMPILE_STATUS,//GLenum pname
				&fragShaderStatu//GLint *params
				);
		if(fragShaderStatu != 0)
		{
			LOGI("initShader() fragShaderStatu = %d, CompileShader succuss!", fragShaderStatu);
		} else
		{
			GLint logLen;
			logLen = 0;
			glGetShaderiv(
				pFragShader,//GLuint shader
				GL_INFO_LOG_LENGTH,//GLenum pname
				&logLen
			);
			GLchar* pFLog = (GLchar*)malloc(sizeof(GLchar) * logLen);
			glGetShaderInfoLog(
				pFragShader,//GLuint shader
				logLen,//GLsizei maxLength
				NULL,//GLsizei *length
				pFLog//GLchar *infoLog
			);
			LOGW("initShader() fragShaderStatu = %d, CompileShader fail! info : %s", fragShaderStatu, pFLog);
			free(pFLog);
			glDeleteShader(pFragShader);
		}
	} else {
		LOGW("initShader() pFragShader glCreateShader(GL_FRAGMENT_SHADER) fail!");
	}

	//��������
	GLuint pProgram = glCreateProgram();
	if(pProgram != 0)
	{
		//������м�����c��ɫ��
		glAttachShader(pProgram, pVertexShader);
		GLint error = glGetError();
		if (GL_NO_ERROR != error)
		{
			LOGW("initShader() AttachShader(pVertexShader) error!");
		}
		//������м���ƬԪ��ɫ��
		glAttachShader(pProgram, pFragShader);
		error = glGetError();
		if (GL_NO_ERROR != error)
		{
			LOGW("initShader() AttachShader(pFragShader) error!");
		}
		//���ӳ���
		glLinkProgram(pProgram);
		//��ȡ���ӳɹ�program
		GLint linkProgramStatu = 0;
		glGetProgramiv(
				pProgram,//GLuint program
				GL_LINK_STATUS,//GLenum pname
				&linkProgramStatu//GLint *params
		);
		if(linkProgramStatu != GL_TRUE)
		{
			GLint logLen;//�洢Log�ĳ���
			glGetProgramiv(
					pProgram,//GLuint program
					GL_INFO_LOG_LENGTH,//GLenum pname
					&logLen//GLint *params
					);
			GLchar* logStr =  (GLchar*)malloc(sizeof(GLchar) * logLen);
			glGetProgramInfoLog(
				pProgram,//GLuint program
				logLen,//GLsizei maxLength
			 	NULL,//GLsizei *length
			 	logStr//GLchar *infoLog
				);
			LOGW("initShader() glLinkProgram fail! info : %s", logStr);
			glDeleteProgram(pProgram);
			free(logStr);
		} else
		{
			LOGI("initShader() glLinkProgram success!");
		}
	} else
	{
		LOGW("initShader() pProgram glCreateProgram() fail!");
	}
	return pProgram;
}
