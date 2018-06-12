#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <math.h>

#ifndef TRUE
#define TRUE   1
#define FALSE  0
#endif

#define VECTOR_EPSILON    0.00001f
#define DISTANCE_EPSILON  1e-08f
#define ANGLE_EPSILON     0.00872665f  // 0.5 degrees

#define MOD(A,B,C) (float) sqrt( A*A + B*B + C*C )

#define PI_VALUE           3.14159265359f
#define DEGREE_TO_RAD      0.0174533f /* 2.0 * 3.1415927 / 360.0 */
#define RAD_TO_DEGREE     57.2958f    /* 360 / ( 2.0 * 3.1415927 ) */


void VectorNormalize( int *ierr, float *vX, float *vY, float *vz );
void UnitVectorPP( int *ierr, float *wX, float *wY, float *wZ, 
							  float aX,  float aY,  float aZ, 
							  float bX,  float bY,  float bz );
void UnitVectorVV( int *ierr, float *wX, float *wY, float *wZ, 
							  float uX,  float uY,  float uZ, 
							  float vX,  float vY,  float vz );
void VectorRotY( float *v, float inc );	// rotates vector : around Y axis
void VectorRotXZ( float *vIn, float inc, int flagStop );

void VectorNormalize(int *ierr, float *vx, float *vy, float *vz) {
    float A, B, C;
    float modf;
    double mod;

    if (*ierr) return;

    A = *vx;
    B = *vy;
    C = *vz;
    mod = A * A + B * B + C*C;
    if (mod > VECTOR_EPSILON) {
        modf = (float) (1. / sqrt(mod));
        *vx = A * modf;
        *vy = B * modf;
        *vz = C * modf;
    } else {
        *vx = 0.0;
        *vy = 0.0;
        *vz = 1.0;
        *ierr = TRUE;
    }
}

void UnitVectorPP(int *ierr, float *wx, float *wy, float *wz,
        float ax, float ay, float az,
        float bx, float by, float bz) {
    if (*ierr) return;
    *wx = bx - ax;
    *wy = by - ay;
    *wz = bz - az;
    VectorNormalize(ierr, wx, wy, wz);
}

void UnitVectorVV(int *ierr, float *wx, float *wy, float *wz,
        float ux, float uy, float uz,
        float vx, float vy, float vz) {
    if (*ierr) return;
    *wx = uy * vz - uz * vy;
    *wy = uz * vx - ux * vz;
    *wz = ux * vy - uy * vx;
    VectorNormalize(ierr, wx, wy, wz);
}

void VectorRotY(float *vIn, float inc) {
    float alpha;
    float modZX;
    float mod;
    mod = MOD(vIn[0], vIn[1], vIn[2]);
    if (mod < VECTOR_EPSILON) return;
    vIn[0] = vIn[0] / mod;
    vIn[1] = vIn[1] / mod;
    vIn[2] = vIn[2] / mod;

    if (fabs(vIn[1]) > sin(PI_VALUE / 2.0 - ANGLE_EPSILON)) return;
    modZX = (float) sqrt(vIn[0] * vIn[0] + vIn[2] * vIn[2]);
    alpha = (float) acos(vIn[2] / modZX);
    if (vIn[0] < 0.0f) alpha = 2.0f * PI_VALUE - alpha;

    alpha += inc;
    vIn[0] = (float) sin(alpha) * modZX;
    vIn[2] = (float) cos(alpha) * modZX;

    vIn[0] = vIn[0] * mod;
    vIn[1] = vIn[1] * mod;
    vIn[2] = vIn[2] * mod;
}

void VectorRotXZ(float *vIn, float inc, int flagStop) {
    float alpha, beta;
    float mod;
    float maxAngle = 90.0f * DEGREE_TO_RAD - ANGLE_EPSILON;

    mod = MOD(vIn[0], vIn[1], vIn[2]);
    if (mod < VECTOR_EPSILON) return;
    vIn[0] = vIn[0] / mod;
    vIn[1] = vIn[1] / mod;
    vIn[2] = vIn[2] / mod;

    if (fabs(vIn[1]) > sin(maxAngle)) return;

    alpha = (float) acos(vIn[2] / sqrt(vIn[0] * vIn[0] + vIn[2] * vIn[2]));
    if (vIn[0] < 0.0f) alpha = 2.0f * PI_VALUE - alpha;
    beta = (float) asin(vIn[1]);
    if (flagStop) {
        beta += inc;
        if (beta > maxAngle) beta = maxAngle;
        else if (beta < -maxAngle) beta = -maxAngle;
    } else {
        if (alpha > PI_VALUE) beta += inc;
        else beta -= inc;
    }

    vIn[0] = (float) cos(beta) * (float) sin(alpha);
    vIn[1] = (float) sin(beta);
    vIn[2] = (float) cos(beta) * (float) cos(alpha);

    vIn[0] = vIn[0] * mod;
    vIn[1] = vIn[1] * mod;
    vIn[2] = vIn[2] * mod;
}

#endif /* TOOLS_H */
