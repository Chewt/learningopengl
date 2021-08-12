#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "shader.h"

void shaderSetInt(unsigned int ID, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void shaderSetFloat(unsigned int ID, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void shaderSetmat4x4(unsigned int ID, const char* name, mat4x4* m)
{
    unsigned int loc = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(loc, 1, GL_TRUE, (const GLfloat*)m);
}

char* readfile(const char* filename)
{
    /* Reading contents of shader into buffer */
    FILE* f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Couldn't open shader: %s\n", filename);
    }
    if(fseek(f, 0, SEEK_END))
    {
        fprintf(stderr, "Error seeking for %s\n", filename);
    }
    long int filesize = ftell(f);

    GLubyte* file_contents = malloc(filesize + 1);
    if(fseek(f, 0, SEEK_SET))
    {
        fprintf(stderr, "Error seeking for %s\n", filename);
    }
    if(!fread(file_contents, 1, filesize, f))
        fprintf(stderr, "Error reading contents of file\n");
    fclose(f);
    file_contents[filesize] = '\0';

    return file_contents;
}

unsigned int createShader(const char* vertex_file, const char* fragment_file)
{
    GLubyte* vertex_code = readfile(vertex_file);
    GLubyte* fragment_code = readfile(fragment_file);

    /* Loading shaders to OpenGL context */
    GLuint vertex_buffer = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_buffer, 1, (const char * const *)&vertex_code, NULL);
    glCompileShader(vertex_buffer);

    GLuint fragment_buffer = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_buffer, 1, (const char * const *)&fragment_code,
            NULL);
    glCompileShader(fragment_buffer);

    // Check to see if it compiled correctly
    GLint compiled;
    glGetObjectParameterivARB(vertex_buffer, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        fprintf(stderr, "Shader compiled incorrectly!\n");
        char infoLog[512];
        glGetShaderInfoLog(vertex_buffer, 512, NULL, infoLog);
        fprintf(stderr, "%s\n%s", infoLog, vertex_code);
    }
    glGetObjectParameterivARB(fragment_buffer, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        fprintf(stderr, "Shader compiled incorrectly!\n");
        char infoLog[512];
        glGetShaderInfoLog(fragment_buffer, 512, NULL, infoLog);
        fprintf(stderr, "%s\n%s", infoLog, fragment_code);
    }

    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertex_buffer);
    glAttachShader(programID, fragment_buffer);

    glLinkProgram(programID);

    // Check if program linked correctly
    GLint linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        printf("Shaders linked incorrectly!\n");
        return -1;
    }

    free(vertex_code);
    free(fragment_code);

    glDeleteShader(vertex_buffer);
    glDeleteShader(fragment_buffer);

    return programID;
}
