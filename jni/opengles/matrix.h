#ifndef CM_MATRIX_H
#define CM_MATRIX_H
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265

#endif

//������ת����
float* getRotateM(int rmOffset, float a, float x, float y, float z);

//ƽ�ƾ���
void translateM(float* m, int mOffset, float x, float y, float z);
