//
//  GLExtra.cpp
//  SprayCam
//
//  Created by David Hirvonen on 10/8/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#include "graphics/GLExtra.h"

_GATHERER_GRAPHICS_BEGIN

void glErrorTest()
{
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        printf("Error (%d)\n", error);
        fflush(stdout);
    }
}

void glCheckError()
{
    glErrorTest();
}

// Set these up with build in transpose for opengl column-major (I'm hard wired to think in row-major)
static const int m00 = 0;
static const int m01 = 4;
static const int m02 = 8;
static const int m03 = 12;

static const int m10 = 1;
static const int m11 = 5;
static const int m12 = 9;
static const int m13 = 13;

static const int m20 = 2;
static const int m21 = 6;
static const int m22 = 10;
static const int m23 = 14;

static const int m30 = 3;
static const int m31 = 7;
static const int m32 = 11;
static const int m33 = 15;

void glMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1;
    m[0+4*1] = 0;
    m[0+4*2] = 0;
    m[0+4*3] = 0;
    m[1+4*0] = 0;
    m[1+4*1] = 1;
    m[1+4*2] = 0;
    m[1+4*3] = 0;
    m[2+4*0] = 0;
    m[2+4*1] = 0;
    m[2+4*2] = 1;
    m[2+4*3] = 0;
    m[3+4*0] = 0;
    m[3+4*1] = 0;
    m[3+4*2] = 0;
    m[3+4*3] = 1;
}

void glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far, GLfloat *matrix)
{
    GLfloat r_l = right - left;
    GLfloat t_b = top - bottom;
    GLfloat f_n = far - near;
    GLfloat tx = - (right + left) / (right - left);
    GLfloat ty = - (top + bottom) / (top - bottom);
    GLfloat tz = - (far + near) / (far - near);

    glMakeIdentityf(matrix);

    matrix[m00] = 2.0f / r_l;
    matrix[m03] = tx;

    matrix[m11] = 2.0f / t_b;
    matrix[m13] = ty;

    matrix[m22] = 2.0f / f_n; // negative here ?
    matrix[m23] = tz;
}

void glPerspectivef(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, GLfloat *matrix)
{
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians = fovy / 2 * 3.14 / 180;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
    {
        return;
    }
    cotangent = cos(radians) / sine;

    glMakeIdentityf(matrix);
    matrix[m00] = cotangent / aspect;
    matrix[m11] = cotangent;
    matrix[m22] = -(zFar + zNear) / deltaZ;
    matrix[m32] = -1;
    matrix[m23] = -2 * zNear * zFar / deltaZ;
    matrix[m33] = 0;
}

///////////////////////////////////////////////////////////////////////////////
// http://www.songho.ca/opengl/gl_transform.html
// set a perspective frustum with 6 params similar to glFrustum()
// (left, right, bottom, top, near, far)
// Note: this is for row-major notation.
///////////////////////////////////////////////////////////////////////////////
void glFrustumf(float l, float r, float b, float t, float n, float f, float *matrix)
{
    glMakeIdentityf(matrix);

    matrix[m00]  = 2 * n / (r - l);
    matrix[m02]  = (r + l) / (r - l);

    matrix[m11]  = 2 * n / (t - b);
    matrix[m12]  = (t + b) / (t - b);

    matrix[m22] = -(f + n) / (f - n);
    matrix[m23] = -(2 * f * n) / (f - n);

    matrix[m32] = -1;
    matrix[m33] = 0;
}

cv::Mat glOrtho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
    // build ortho projection with (column major) glOrth and return the tranpose for row major operations
    cv::Mat tmp = cv::Mat::eye(4, 4, CV_32F);
    glOrthof(left, right, bottom, top, near, far, (GLfloat *)tmp.data);
    return tmp.t();
}

cv::Mat glFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
    // build ortho projection with (column major) glOrth and return the tranpose for row major operations
    cv::Mat tmp = cv::Mat::eye(4, 4, CV_32F);
    glFrustumf(left, right, bottom, top, near, far, (GLfloat *)tmp.data);
    return tmp.t();
}

_GATHERER_GRAPHICS_END
