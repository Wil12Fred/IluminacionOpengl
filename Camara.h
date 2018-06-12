#include <stdlib.h>
#include <GLUT/glut.h>
#include "Vector_tools.h"


#ifndef CAMARA_H
#define CAMARA_H

#define CAM_PARALLEL 1
#define CAM_CONIC 2

#define CAM_STOP 0
#define CAM_EXAMINAR 1
#define CAM_PASEAR 2
#define CAM_TRIPODE 3
#define CAM_PAN 4

class Camara {
public:
    float camViewX; // View point
    float camViewY;
    float camViewZ;
    float camAtX; // look At point
    float camAtY;
    float camAtZ;
    float camUpX; // Up vector
    float camUpY;
    float camUpZ;
    float camAperture; // field of view radians
    float camNear;
    float camFar;
    int camProjection; // PARALLEL or CONIC
    int camMovimiento; // EXAMINAR, ANDAR, TRIPODE or PAN	
    float aspectRatio;
    float x1, x2, y1, y2, z1, z2;
    float camIX, camIY, camIZ;
    float camJX, camJY, camJZ;
    float camKX, camKY, camKZ;
    
    Camara();
    Camara(float positionX, float positionY, float positionZ);
    Camara(const Camara& orig);
    virtual ~Camara();
    void SetCamera(
            float viewX, float viewY, float viewZ,
            float atX, float atY, float atZ,
            float upX, float upY, float upZ);
    void SetDependentParametersCamera();
    void SetGLCamera();
    void SetGLAspectRatioCamera();
    void AvanceFreeCamera( float step);
    void YawCamera( float angle); // local axis Y camera
    void Rotar_Latitud( float inc);
    void Rotar_Longitud(float inc);
};

Camara::Camara() {
}

Camara::Camara(float positionX, float positionY, float positionZ) {
    int ierr = 0;
    camViewX = positionX;
    camViewY = positionY;
    camViewZ = positionZ;
    camAtX = 0.0f;
    camAtY = 0.0f;
    camAtZ = 0.0f;
    camUpX = 0.0f;
    camUpY = 1.0f;
    camUpZ = 0.0f;
    camAperture = 60.0f * DEGREE_TO_RAD;
    camNear = 0.5f;
    camFar = 200.0f;
    camProjection = CAM_CONIC;
    aspectRatio = 1.0f;
    SetDependentParametersCamera();
}

void Camara::SetDependentParametersCamera() {
    float ix, iy, iz;
    float jx, jy, jz;
    float kx, ky, kz;
    float atX, atY, atZ;
    float upX, upY, upZ;
    float viewX, viewY, viewZ;
    int ierr = 0;

    viewX = camViewX;
    viewY = camViewY;
    viewZ = camViewZ;
    atX = camAtX;
    atY = camAtY;
    atZ = camAtZ;
    upX = 0.0f;
    upY = 1.0f;
    upZ = 0.0f;
    UnitVectorPP(&ierr, &kx, &ky, &kz,
            atX, atY, atZ,
            viewX, viewY, viewZ);
    UnitVectorVV(&ierr, &ix, &iy, &iz,
            upX, upY, upZ,
            kx, ky, kz);
    UnitVectorVV(&ierr, &jx, &jy, &jz,
            kx, ky, kz,
            ix, iy, iz);
    camKX = kx;
    camKY = ky;
    camKZ = kz;
    camIX = ix;
    camIY = iy;
    camIZ = iz;
    camJX = jx;
    camJY = jy;
    camJZ = jz;
    camUpX = jx;
    camUpY = jy;
    camUpZ = jz;
}

void Camara::SetGLCamera() {
    float fovy;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (camProjection == CAM_CONIC) {
        fovy = camAperture * RAD_TO_DEGREE;
        gluPerspective(fovy, aspectRatio, camNear, camFar);
    } else { // CAM_PARALLEL		
        glOrtho(x1, x2, y1, y2, z1, z2);
    }
    gluLookAt(camViewX, camViewY, camViewZ,
            camAtX, camAtY, camAtZ,
            camUpX, camUpY, camUpZ);
    glMatrixMode(GL_MODELVIEW); // *** GL_MODELVIEW ***

}

void Camara::SetCamera(
        float viewX, float viewY, float viewZ,
        float atX, float atY, float atZ,
        float upX, float upY, float upZ) {
    camViewX = viewX;
    camViewY = viewY;
    camViewZ = viewZ;
    camAtX = atX;
    camAtY = atY;
    camAtZ = atZ;
    camUpX = upX;
    camUpY = upY;
    camUpZ = upZ;
    SetDependentParametersCamera();
}

void Camara::AvanceFreeCamera(float step) {
    float vaX, vaY, vaZ;
    vaX = step * camKX;
    vaY = step * camKY;
    vaZ = step * camKZ;
    camViewX = camViewX + vaX;
    camViewY = camViewY + vaY;
    camViewZ = camViewZ + vaZ;
    camAtX = camAtX + vaX;
    camAtY = camAtY + vaY;
    camAtZ = camAtZ + vaZ;
    SetDependentParametersCamera();
}

void Camara::YawCamera(float angle) {
    float vIn[3];
    vIn[0] = camAtX - camViewX;
    vIn[1] = camAtY - camViewY;
    vIn[2] = camAtZ - camViewZ;
    VectorRotY(vIn, angle);
    camAtX = camViewX + vIn[0];
    camAtY = camViewY + vIn[1];
    camAtZ = camViewZ + vIn[2];
    SetDependentParametersCamera();
}

void Camara::Rotar_Longitud(float inc) {
    float vIn[3];
    vIn[0] = camViewX - camAtX;
    vIn[1] = camViewY - camAtY;
    vIn[2] = camViewZ - camAtZ;
    VectorRotY(vIn, inc);
    camViewX = camAtX + vIn[0];
    camViewZ = camAtZ + vIn[2];
    SetDependentParametersCamera();
}

void Camara::Rotar_Latitud( float inc) {
    float vIn[3];
    vIn[0] = camViewX - camAtX;
    vIn[1] = camViewY - camAtY;
    vIn[2] = camViewZ - camAtZ;
    VectorRotXZ(vIn, inc, TRUE);
    camViewX = camAtX + vIn[0];
    camViewY = camAtY + vIn[1];
    camViewZ = camAtZ + vIn[2];
    SetDependentParametersCamera();
}

void Camara::SetGLAspectRatioCamera() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    if (viewport[3] > 0)
        aspectRatio = (float) viewport[2] / (float) viewport[3]; // width/height
    else
        aspectRatio = 1.0f;
    SetDependentParametersCamera();
}

Camara::~Camara() {
}


#endif /* CAMARA_H */

