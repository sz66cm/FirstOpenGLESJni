#include "cmshader.h"
#include "matrix.h"

const char* vertexShaderCode = \
						"uniform mat4 uMVPMatrix; 					\n" \
						"attribute vec3 aPosition; 					\n" \
						"attribute vec2 aTexCoor; 					\n" \
						"varying vec2 vTextureCoord;				\n" \
						"void main() 								\n" \
						"{ 											\n" \
						"gl_Position=uMVPMatrix*vec4(aPosition,1); 	\n" \
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
	GLfloat* rotateMatrix;
	rotateMatrix = getRotateM(0, 0, 1, 0, 0);
	int i;
	for(i = 0; i < 16; i++)
	{
		LOGI("GLFloat[%d] = %f", i, *(rotateMatrix + i));
	}
	translateM(rotateMatrix, 0, 8, 8, 8);
	for(i = 0; i < 16; i++)
	{
		LOGI("After translate GLFloat[%d] = %f", i, *(rotateMatrix + i));
	}
	free(rotateMatrix);
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
			LOGW("initShader() vertexShaderStatu = %d, CompileShader fail!", vertexShaderStatu);
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
			LOGW("initShader() fragShaderStatu = %d, CompileShader fail!", fragShaderStatu);
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
