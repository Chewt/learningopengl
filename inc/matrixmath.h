#ifndef MATRIXMATH_H
#define MATRIXMATH_H
#include <math.h>

typedef struct
{
    float x;
    float y;
} vec2;

typedef struct
{
    float x;
    float y;
    float z;
} vec3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct
{
    vec4 r1;
    vec4 r2;
    vec4 r3;
    vec4 r4;
} mat4x4;

void mat4x4_identity(mat4x4* dest);
void mat4x4_projectionp(mat4x4* dest, float theta, float ar, float np,float fp);
void mat4x4_lookat(mat4x4* dest, vec4* p, vec4* t, vec4* u);
void mat4x4_row_to_col(mat4x4*dest, mat4x4* src);

void mat4x4_rotx(mat4x4* dest, mat4x4* src, float theta);
void mat4x4_roty(mat4x4* dest, mat4x4* src, float theta);
void mat4x4_rotz(mat4x4* dest, mat4x4* src, float theta);

void mat4x4_mul(mat4x4* dest, mat4x4* matl, mat4x4* matr);
void mat4x4_add(mat4x4* dest, mat4x4* matl, mat4x4* matr);
void mat4x4_scale(mat4x4* dest, mat4x4* src, float scalar);
void mat4x4_translate(mat4x4* dest, vec4* trans);

void vec4_rotx(vec4* dest, vec4* src, float theta);
void vec4_roty(vec4* dest, vec4* src, float theta);
void vec4_rotz(vec4* dest, vec4* src, float theta);

void vec4_scale(vec4* dest, vec4* src, float scalar);
void vec4_translate(vec4* dest, vec4* src, vec4* trans);
void vec4_norm(vec4* dest, vec4* input);

float vec4_dot(vec4* vecl, vec4* vecr);
void vec4_cross(vec4* dest, vec4* vecl, vec4* vecr);
void vec4_add(vec4* dest, vec4* l, vec4* r);
void vec4_sub(vec4* dest, vec4* l, vec4* r);

void vec4_mul_mat4x4(vec4* dest, vec4* vec, mat4x4* mat);
void print_vec4(vec4* v);

float to_rad(float theta);

#endif
