#ifndef CM_MATRIX_H
#define CM_MATRIX_H
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define PI 3.14159265

#endif

//���֮�䳤��
float length(float x, float y, float z);

//������ת����
float* getRotateM(int rmOffset, float a, float x, float y, float z);

//ƽ�ƾ���
void translateM(float* m, int mOffset, float x, float y, float z);

//����ͷ����
float* setLookAtM(float* rm, int rmOffset,
        float eyeX, float eyeY, float eyeZ,
        float centerX, float centerY, float centerZ, float upX, float upY,
        float upZ);

/**
 * 4x4������
 * param left : ���Ӿ��,�K�������ŽY��
 */
void matrixMM4(float* left, float* right);

/**
 * ͸�Ӿ���
 *
 */
float* frustumM(float* m, int offset,
            float left, float right, float bottom, float top,
            float near, float far);
