#ifndef SHADER_H
#define SHADER_H
#include "matrixmath.h"

unsigned int createShader(const char* vertex_file, const char* fragment_file);
void shaderSetFloat(unsigned int ID, const char* name, float value);
void shaderSetInt(unsigned int ID, const char* name, int value);
void shaderSetmat4x4(unsigned int ID, const char* name, mat4x4* m);

#endif
