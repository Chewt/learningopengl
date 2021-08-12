#include <string.h>
#include <stdio.h>
#include "matrixmath.h"

float to_rad(float theta)
{
    return theta / 180.f * 3.141592654;
}

void mat4x4_row_to_col(mat4x4*dest, mat4x4* src)
{
    mat4x4 temp;
    temp.r1 = (vec4){src->r1.x, src->r2.x, src->r3.x, src->r4.x};
    temp.r2 = (vec4){src->r1.y, src->r2.y, src->r3.y, src->r4.y};
    temp.r3 = (vec4){src->r1.z, src->r2.z, src->r3.z, src->r4.z};
    temp.r4 = (vec4){src->r1.w, src->r2.w, src->r3.w, src->r4.w};
    memcpy(dest, &temp, sizeof(mat4x4));
}

void mat4x4_projectionp(mat4x4* dest, float theta, float ar, float np, float fp)
{
    theta = theta / 180.f * 3.1415f;

    float fov = 1.f / tan(theta / 2.f);
    float q = fp / (fp - np);

    dest->r1 = (vec4){fov / ar, 0.f, 0.f ,  0.f};
    dest->r2 = (vec4){0.f, fov, 0.f ,  0.f};
    dest->r3 = (vec4){0.f, 0.f, -q  , -q * np};
    dest->r4 = (vec4){0.f, 0.f, -1.f,  0.f};
}

void mat4x4_identity(mat4x4* dest)
{
    dest->r1 = (vec4){1.f, 0.f, 0.f, 0.f};
    dest->r2 = (vec4){0.f, 1.f, 0.f, 0.f};
    dest->r3 = (vec4){0.f, 0.f, 1.f, 0.f};
    dest->r4 = (vec4){0.f, 0.f, 0.f, 1.f};
}

void mat4x4_lookat(mat4x4* dest, vec4* p, vec4* t, vec4* u)
{
    vec4 r, d, cu;
    vec4_sub(&d, p, t);
    vec4_norm(&d, &d);
    vec4_cross(&r, u, &d);
    vec4_norm(&r, &r);
    vec4_cross(&cu, &d, &r);
    vec4_norm(&cu, &cu);

    mat4x4 temp1;
    temp1.r1 = (vec4){r.x, r.y, r.z,  0.f};
    temp1.r2 = (vec4){cu.x, cu.y, cu.z, 0.f};
    temp1.r3 = (vec4){d.x, d.y, d.z,  0.f};
    temp1.r4 = (vec4){0.f,  0.f,  0.f,   1.f};
    mat4x4 temp2;
    temp2.r1 = (vec4){1.f,  0.f,  0.f, -p->x};
    temp2.r2 = (vec4){0.f,  1.f,  0.f, -p->y};
    temp2.r3 = (vec4){0.f,  0.f,  1.f, -p->z};
    temp2.r4 = (vec4){0.f,  0.f,  0.f,   1.f};
    mat4x4_mul(dest, &temp1, &temp2);
}

void mat4x4_rotx(mat4x4* dest, mat4x4* src, float theta)
{
    theta = theta / 180.f * 3.1415f;
    mat4x4 temp;
    temp.r1 = (vec4){1.f,        0.f,         0.f, 0.f};
    temp.r2 = (vec4){0.f, cos(theta), -sin(theta), 0.f};
    temp.r3 = (vec4){0.f, sin(theta),  cos(theta), 0.f};
    temp.r4 = (vec4){0.f,        0.f,         0.f, 1.f};

    mat4x4_mul(dest, &temp, src);
}

void mat4x4_roty(mat4x4* dest, mat4x4* src, float theta)
{
    theta = theta / 180.f * 3.1415f;
    mat4x4 temp;
    temp.r1 = (vec4){ cos(theta), 0.f, sin(theta), 0.f};
    temp.r2 = (vec4){        0.f, 1.f,        0.f, 0.f};
    temp.r3 = (vec4){-sin(theta), 0.f, cos(theta), 0.f};
    temp.r4 = (vec4){        0.f, 0.f,        0.f, 1.f};

    mat4x4_mul(dest, &temp, src);
}

void mat4x4_rotz(mat4x4* dest, mat4x4* src, float theta)
{
    theta = theta / 180.f * 3.1415f;
    mat4x4 temp;
    temp.r1 = (vec4){cos(theta), -sin(theta), 0.f, 0.f};
    temp.r2 = (vec4){sin(theta),  cos(theta), 0.f, 0.f};
    temp.r3 = (vec4){       0.f,         0.f, 1.f, 0.f};
    temp.r4 = (vec4){       0.f,         0.f, 0.f, 1.f};

    mat4x4_mul(dest, &temp, src);
}

void mat4x4_mul(mat4x4* dest, mat4x4* matl, mat4x4* matr)
{
    vec4 col1 = (vec4){matr->r1.x,matr->r2.x,matr->r3.x,matr->r4.x};
    vec4 col2 = (vec4){matr->r1.y,matr->r2.y,matr->r3.y,matr->r4.y};
    vec4 col3 = (vec4){matr->r1.z,matr->r2.z,matr->r3.z,matr->r4.z};
    vec4 col4 = (vec4){matr->r1.w,matr->r2.w,matr->r3.w,matr->r4.w};

    mat4x4 temp;

    temp.r1.x = vec4_dot(&matl->r1, &col1);
    temp.r1.y = vec4_dot(&matl->r1, &col2);
    temp.r1.z = vec4_dot(&matl->r1, &col3);
    temp.r1.w = vec4_dot(&matl->r1, &col4);

    temp.r2.x = vec4_dot(&matl->r2, &col1);
    temp.r2.y = vec4_dot(&matl->r2, &col2);
    temp.r2.z = vec4_dot(&matl->r2, &col3);
    temp.r2.w = vec4_dot(&matl->r2, &col4);

    temp.r3.x = vec4_dot(&matl->r3, &col1);
    temp.r3.y = vec4_dot(&matl->r3, &col2);
    temp.r3.z = vec4_dot(&matl->r3, &col3);
    temp.r3.w = vec4_dot(&matl->r3, &col4);

    temp.r4.x = vec4_dot(&matl->r4, &col1);
    temp.r4.y = vec4_dot(&matl->r4, &col2);
    temp.r4.z = vec4_dot(&matl->r4, &col3);
    temp.r4.w = vec4_dot(&matl->r4, &col4);
    memcpy(dest, &temp, sizeof(mat4x4));
}

void mat4x4_add(mat4x4* dest, mat4x4* matl, mat4x4* matr)
{
    dest->r1.x = matl->r1.x + matr->r1.x;
    dest->r1.y = matl->r1.y + matr->r1.y;
    dest->r1.z = matl->r1.z + matr->r1.z;
    dest->r1.w = matl->r1.w + matr->r1.w;
    
    dest->r2.x = matl->r2.x + matr->r2.x;
    dest->r2.y = matl->r2.y + matr->r2.y;
    dest->r2.z = matl->r2.z + matr->r2.z;
    dest->r2.w = matl->r2.w + matr->r2.w;
    
    dest->r3.x = matl->r3.x + matr->r3.x;
    dest->r3.y = matl->r3.y + matr->r3.y;
    dest->r3.z = matl->r3.z + matr->r3.z;
    dest->r3.w = matl->r3.w + matr->r3.w;
    
    dest->r4.x = matl->r4.x + matr->r4.x;
    dest->r4.y = matl->r4.y + matr->r4.y;
    dest->r4.z = matl->r4.z + matr->r4.z;
    dest->r4.w = matl->r4.w + matr->r4.w;
}

void mat4x4_scale(mat4x4* dest, mat4x4* src, float scalar)
{
    vec4_scale(&dest->r1, &src->r1, scalar);
    vec4_scale(&dest->r2, &src->r2, scalar);
    vec4_scale(&dest->r3, &src->r3, scalar);
    dest->r4 = src->r4;
}

void mat4x4_translate(mat4x4* dest, vec4* trans)
{
    dest->r1.w += trans->x;
    dest->r2.w += trans->y;
    dest->r3.w += trans->z;
    dest->r4.w += trans->w;
}

void vec4_mul_mat4x4(vec4* dest, vec4* vec, mat4x4* mat)
{
    dest->x = vec4_dot(vec, &mat->r1);
    dest->y = vec4_dot(vec, &mat->r2);
    dest->z = vec4_dot(vec, &mat->r3);
    dest->w = vec4_dot(vec, &mat->r4);
    if (dest->w != 0.f)
    {
        dest->x /= dest->w;
        dest->y /= dest->w;
        dest->z /= dest->w;
    }
}

void vec4_scale(vec4* dest, vec4* src, float scalar)
{
    dest->x = src->x * scalar;
    dest->y = src->y * scalar;
    dest->z = src->z * scalar;
    dest->w = src->w * scalar;
}

void vec4_translate(vec4* dest, vec4* src, vec4* trans)
{
    dest->x = src->x + trans->x;
    dest->y = src->y + trans->y;
    dest->z = src->z + trans->z;
    dest->w = 1.f;
}

void vec4_rotx(vec4* dest, vec4* src, float theta)
{
    theta = theta / 180.f * 3.1415f;
    dest->x = src->x;
    dest->y = (cos(theta) * src->y) - (sin(theta) * src->z);
    dest->z = (sin(theta) * src->y) + (cos(theta) * src->z);
    dest->w = src->w;
}

void vec4_roty(vec4* dest, vec4* src, float theta)
{
    theta = theta / 180.f * 3.1415f;
    dest->x = (cos(theta) * src->x) + (sin(theta) * src->z);
    dest->y = src->y;
    dest->z = (-sin(theta) * src->x) + (cos(theta) * src->z);
    dest->w = src->w;
}
void vec4_rotz(vec4* dest, vec4* src, float theta)
{
    theta = theta / 180.f * 3.1415f;
    dest->x = (cos(theta) * src->x) - (sin(theta) * src->y);
    dest->y = (sin(theta) * src->x) + (cos(theta) * src->y);
    dest->z = src->z;
    dest->w = src->w;
}

float vec4_dot(vec4* l, vec4* r)
{
    return (l->x * r->x) + (l->y * r->y) + (l->z * r->z) + (l->w * r->w);
}

void vec4_cross(vec4* dest, vec4* l, vec4* r)
{
    float x, y, z, w;
    x = (l->y * r->z) - (l->z * r->y);
    y = (l->z * r->x) - (l->x * r->z);
    z = (l->x * r->y) - (l->y * r->x);
    w = (l->w == r->w) ? l->w : 1.f;
    dest->x = x;
    dest->y = y;
    dest->z = z;
    dest->w = w;
}

void vec4_norm(vec4* dest, vec4* input)
{
    float x = input->x;
    float y = input->y;
    float z = input->z;
    float divisor = x * x + y * y + z * z;
    divisor = sqrt(divisor);
    dest->x = x / divisor;
    dest->y = y / divisor;
    dest->z = z / divisor;
    dest->w = input->w;
}

void vec4_add(vec4* dest, vec4* l, vec4* r)
{
    float x, y, z;
    x = l->x + r->x;
    y = l->y + r->y;
    z = l->z + r->z;
    dest->x = x;
    dest->y = y;
    dest->z = z;
    dest->w = (l->w == r->w) ? l->w : 1.f;
}

void vec4_sub(vec4* dest, vec4* l, vec4* r)
{
    float x, y, z;
    x = l->x - r->x;
    y = l->y - r->y;
    z = l->z - r->z;
    dest->x = x;
    dest->y = y;
    dest->z = z;
    dest->w = (l->w == r->w) ? l->w : 1.f;
}

void print_vec4(vec4* v)
{
    printf("%0.2f, %0.2f, %0.2f, %0.2f\n", v->x, v->y, v->z, v->w);
}
