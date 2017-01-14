#include "matrix.h"

/**
 * 用于实现矩阵初始化,变换相关函数.
 */

float* getRotateM(int rmOffset, float a, float x, float y, float z)
{
	float* rm = (float*)malloc(sizeof(float) * 16);
    rm[rmOffset + 3] = 0;
    rm[rmOffset + 7] = 0;
    rm[rmOffset + 11]= 0;
    rm[rmOffset + 12]= 0;
    rm[rmOffset + 13]= 0;
    rm[rmOffset + 14]= 0;
    rm[rmOffset + 15]= 1;
    a *= (float) (PI / 180.0f);
    float s = (float) sin(a);
    float c = (float) cos(a);
    if (1.0f == x && 0.0f == y && 0.0f == z) {
        rm[rmOffset + 5] = c;   rm[rmOffset + 10]= c;
        rm[rmOffset + 6] = s;   rm[rmOffset + 9] = -s;
        rm[rmOffset + 1] = 0;   rm[rmOffset + 2] = 0;
        rm[rmOffset + 4] = 0;   rm[rmOffset + 8] = 0;
        rm[rmOffset + 0] = 1;
    } else if (0.0f == x && 1.0f == y && 0.0f == z) {
        rm[rmOffset + 0] = c;   rm[rmOffset + 10]= c;
        rm[rmOffset + 8] = s;   rm[rmOffset + 2] = -s;
        rm[rmOffset + 1] = 0;   rm[rmOffset + 4] = 0;
        rm[rmOffset + 6] = 0;   rm[rmOffset + 9] = 0;
        rm[rmOffset + 5] = 1;
    } else if (0.0f == x && 0.0f == y && 1.0f == z) {
        rm[rmOffset + 0] = c;   rm[rmOffset + 5] = c;
        rm[rmOffset + 1] = s;   rm[rmOffset + 4] = -s;
        rm[rmOffset + 2] = 0;   rm[rmOffset + 6] = 0;
        rm[rmOffset + 8] = 0;   rm[rmOffset + 9] = 0;
        rm[rmOffset + 10]= 1;
    } else {
        float len = (float)sqrt(x * x + y * y + z * z);
        if (1.0f != len) {
            float recipLen = 1.0f / len;
            x *= recipLen;
            y *= recipLen;
            z *= recipLen;
        }
        float nc = 1.0f - c;
        float xy = x * y;
        float yz = y * z;
        float zx = z * x;
        float xs = x * s;
        float ys = y * s;
        float zs = z * s;
        rm[rmOffset +  0] = x*x*nc +  c;
        rm[rmOffset +  4] =  xy*nc - zs;
        rm[rmOffset +  8] =  zx*nc + ys;
        rm[rmOffset +  1] =  xy*nc + zs;
        rm[rmOffset +  5] = y*y*nc +  c;
        rm[rmOffset +  9] =  yz*nc - xs;
        rm[rmOffset +  2] =  zx*nc - ys;
        rm[rmOffset +  6] =  yz*nc + xs;
        rm[rmOffset + 10] = z*z*nc +  c;
    }
    return rm;
}
/**
 * 平移矩阵变换
 */
void translateM(float* m, int mOffset, float x, float y, float z)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		int mi = mOffset + i;
		m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
	}
}

/**
 * 获得摄像头视角矩阵
 */
float* setLookAtM(float* rm, int rmOffset,
        float eyeX, float eyeY, float eyeZ,
        float centerX, float centerY, float centerZ, float upX, float upY,
        float upZ)
{
    // See the OpenGL GLUT documentation for gluLookAt for a description
    // of the algorithm. We implement it in a straightforward way:
	if(rm == 0)
	{
		rm = (float*)malloc(sizeof(float) * 16);
	}
    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    // Normalize f
    float rlf = 1.0f / length(fx, fy, fz);
    fx *= rlf;
    fy *= rlf;
    fz *= rlf;

    // compute s = f x up (x means "cross product")
    float sx = fy * upZ - fz * upY;
    float sy = fz * upX - fx * upZ;
    float sz = fx * upY - fy * upX;

    // and normalize s
    float rls = 1.0f / length(sx, sy, sz);
    sx *= rls;
    sy *= rls;
    sz *= rls;

    // compute u = s x f
    float ux = sy * fz - sz * fy;
    float uy = sz * fx - sx * fz;
    float uz = sx * fy - sy * fx;

    rm[rmOffset + 0] = sx;
    rm[rmOffset + 1] = ux;
    rm[rmOffset + 2] = -fx;
    rm[rmOffset + 3] = 0.0f;

    rm[rmOffset + 4] = sy;
    rm[rmOffset + 5] = uy;
    rm[rmOffset + 6] = -fy;
    rm[rmOffset + 7] = 0.0f;

    rm[rmOffset + 8] = sz;
    rm[rmOffset + 9] = uz;
    rm[rmOffset + 10] = -fz;
    rm[rmOffset + 11] = 0.0f;

    rm[rmOffset + 12] = 0.0f;
    rm[rmOffset + 13] = 0.0f;
    rm[rmOffset + 14] = 0.0f;
    rm[rmOffset + 15] = 1.0f;

    translateM(rm, rmOffset, -eyeX, -eyeY, -eyeZ);
    return rm;
}

//求点之间长度
float length(float x, float y, float z)
{
	return (float)sqrt(x * x + y * y + z * z);
}

/**
 * 4x4矩陣相乘
 * param left : 左子矩陣,並且最後存放結果
 */
void matrixMM4(float* left, float* right)
{
	float* tmp = (float*)malloc(sizeof(float) * 16);
	typedef float (*dyadicArray)[4];//二维数组类型定义
	dyadicArray dLeft = (dyadicArray) left;//这样强转后,对释放是否有影响.
	dyadicArray dRight = (dyadicArray) right;
	dyadicArray dTmp = (dyadicArray) tmp;

	int i,j,len;
	for (i = 0; i < 4; ++i) {
		for (j = 0; j < 4; ++j)
		{
			float t;
			for (len = 0; len < 4; ++len) {
				t += (dLeft[i][len] * dRight[len][j]);
			}
			dTmp[i][j] = t;
		}
	}
	memcpy(left, dTmp, 16 * sizeof(float));
	free(tmp);
}
