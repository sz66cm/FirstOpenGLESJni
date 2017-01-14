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

//渲染
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
	//获取着色器中变换矩阵的引用
	GLfloat* mMVPMatrix = setLookAtM(NULL, 0,
			0,0,3,//eye
			0,0,0,//camera
			0,1,0//up
			);
	//投影矩阵
	float radio = 720.0f / 1280.0f;
	GLfloat* mProjectMatrix = frustumM(NULL, 0,
			-radio, radio, //left right
			-1, 1,//bottom top
			1, 10//near far
			);
	//向Z轴前移一个单位
	translateM(mMVPMatrix, 0, 0, 0, 1);
	//获取着色器总变换矩阵引用
	GLint muMVPMatrixHandle = glGetUniformLocation(
			pP,//GLuint program
			"uMVPMatrix"//const GLchar *name
	);
	//矩阵整合
	matrixMM4(mMVPMatrix, mProjectMatrix);
	//将总变换矩阵传输到着色器
	glUniformMatrix4fv(
			muMVPMatrixHandle,//GLint location
			1,//GLsizei count
			GL_FALSE,//GLboolean transpose
			mMVPMatrix//const GLfloat *value
	);
	LOGI("glUniformMatrix4fv - end");
	//获取着色器中顶点属性引用
	GLint pVertexAttribHandle = glGetAttribLocation(
			pP,//	GLuint program
			"aPosition"//  const GLchar *name
	);
	LOGI("pVertexAttribHandle : %d", pVertexAttribHandle);
	//为着色器指定顶点数据
	glVertexAttribPointer(
			pVertexAttribHandle,//GLuint index
			3,//GLint size X Y Z
			GL_FLOAT,//GLenum type
			GL_FALSE,//GLboolean normalized
			3 * 4,//GLsizei stride
			vertices//const GLvoid * pointer
	);
	//允许顶点位置数据数组
	glEnableVertexAttribArray(pVertexAttribHandle);

	//绘制三角形
	glDrawArrays(
			GL_TRIANGLES,//GLenum mode
			0,//GLint first index
			3//GLsizei counts
	);
	LOGI("glDrawArrays - end");
	//释放内存
	free(mProjectMatrix);
	free(mMVPMatrix);
}

//初始化着色器
GLuint initShader()
{
	//顶点着色器初始化
	GLuint pVertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (pVertexShader != 0)
	{//若创建成功,就加载shader
		//加载shader的源代码
		glShaderSource(pVertexShader, 1, &vertexShaderCode, NULL);
		//编译shader
		glCompileShader(pVertexShader);
		//获取Shader编译情况
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
			glDeleteShader(pVertexShader);//销毁释放指针
		}
	} else {
		LOGW("initShader() pVertexShader glCreateShader(GL_VERTEX_SHADER) fail!");
	}

	//片元着色器初始化
	GLuint pFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if(pFragShader != 0)
	{//片元著色器指針創建成功
		//加載shader的源代碼
		glShaderSource(pFragShader, 1, &fragShaderCode, NULL);
		//編譯shader
		glCompileShader(pFragShader);
		//獲取著色器編譯情況
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

	//創建程序
	GLuint pProgram = glCreateProgram();
	if(pProgram != 0)
	{
		//向程序中加入頂點著色器
		glAttachShader(pProgram, pVertexShader);
		GLint error = glGetError();
		if (GL_NO_ERROR != error)
		{
			LOGW("initShader() AttachShader(pVertexShader) error!");
		}
		//向程序中加入片元着色器
		glAttachShader(pProgram, pFragShader);
		error = glGetError();
		if (GL_NO_ERROR != error)
		{
			LOGW("initShader() AttachShader(pFragShader) error!");
		}
		//链接程序
		glLinkProgram(pProgram);
		//获取链接成功program
		GLint linkProgramStatu = 0;
		glGetProgramiv(
				pProgram,//GLuint program
				GL_LINK_STATUS,//GLenum pname
				&linkProgramStatu//GLint *params
		);
		if(linkProgramStatu != GL_TRUE)
		{
			GLint logLen;//存储Log的长度
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
