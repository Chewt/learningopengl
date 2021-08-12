#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "shader.h"
#include "matrixmath.h"
#include "textures.h"

/* Image loader */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void key_callback(GLFWwindow* window, int key, int scancode, 
                                      int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

void print_mat4x4(mat4x4* mat);

void frame_buffer_callback(GLFWwindow* window, int width_n, int height_n);

float cube_verts[] = 
{
    // Bottom face
     0.5f, -0.5f,  0.5f,   1.f, 1.f, 
     0.5f, -0.5f, -0.5f,   1.f, 0.f,
    -0.5f, -0.5f, -0.5f,   0.f, 0.f,
     0.5f, -0.5f,  0.5f,   1.f, 1.f,
    -0.5f, -0.5f, -0.5f,   0.f, 0.f,
    -0.5f, -0.5f,  0.5f,   0.f, 1.f,

    // Front face
    -0.5f, -0.5f,  0.5f,   0.f, 0.f,
     0.0f,  0.5f,  0.0f,   .5f, 1.f,
     0.5f, -0.5f,  0.5f,   1.f, 0.f,

     // Right Face
     0.5f, -0.5f,  0.5f,   0.f, 0.f,
     0.0f,  0.5f,  0.0f,   .5f, 1.f,
     0.5f, -0.5f, -0.5f,   1.f, 0.f,

     // Back Face
     0.5f, -0.5f, -0.5f,   0.f, 0.f,
     0.0f,  0.5f,  0.0f,   .5f, 1.f,
    -0.5f, -0.5f, -0.5f,   1.f, 0.f,

     // Left Face
    -0.5f, -0.5f, -0.5f,   0.f, 0.f,
     0.0f,  0.5f,  0.0f,   .5f, 1.f,
    -0.5f, -0.5f,  0.5f,   1.f, 0.f,
};

unsigned int program;

int width = 640;
int height = 480;

float deltaTime, lastFrame;

int move_mode = 0;

vec4 cameraPos = (vec4){0.f, 0.f, 3.f, 0.f};
vec4 cameraFront = (vec4){0.f, 0.f, -1.f, 0.f};
vec4 cameraUp = (vec4){0.f, 1.f, 0.f, 0.f};

float pitch;
float yaw = -90.f;

int main()
{
    if (!glfwInit())
    {
        fprintf(stderr, "glfw initialization failed!\n");
        return -1;
    }

    /* Set OpenGL version to 3.3 and specify that we only want core profile */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create the window */
    GLFWwindow* window = glfwCreateWindow(width, height, "floating", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "glfw window creation faled!\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    /* Set callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    /* OpenGL stuff below */

    /* Load in shaders and assign vertex attributes*/
    program = createShader("shaders/vertex.glsl", "shaders/fragment.glsl");

    unsigned int texture;
    texture  = createTexture("textures/RTS_Crate.png", GL_TEXTURE0);

    /* Set behaviour for going past normalized texture space */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    /* Set Texture filtering method for magnifying and minifying */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Now we want a VAO for our square */
    unsigned int VAO_square;
    glGenVertexArrays(1, &VAO_square);
    glBindVertexArray(VAO_square);

    unsigned int VBO_square;
    glGenBuffers(1, &VBO_square);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_square);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts,
            GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)(
            sizeof(float) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /* How many frames to wait before swaping buffers? - VSYNC*/
    glfwSwapInterval(1);

    glUseProgram(program);
    shaderSetInt(program, "mytexture", 0);

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    // Disable cursor

    // Set up projection matrix;
    mat4x4 projection;
    mat4x4_projectionp(&projection, 90.f, (float)width/(float)height,.1f,100.f);
    shaderSetmat4x4(program, "projection", &projection);
    vec4 positions[] = 
    {
        (vec4){0.f, 0.f, 0.f, 0.f},
        (vec4){.5f, 0.f, -1.f, 0.f},
        (vec4){.3f, 0.f,-3.f, 0.f},
        (vec4){-.4f, 0.f,-.5f, 0.f},
    };

    while (!glfwWindowShouldClose(window))
    {
        /* render loop */

        glClearColor(0.2f, 0.3f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        float timeValue = glfwGetTime();
        float radius = 4.f;
        float camX = sin(timeValue) * radius;
        float camZ = cos(timeValue) * radius;

        deltaTime = timeValue - lastFrame;
        lastFrame = timeValue;
        
        vec4 direction;
        direction.x = cos(to_rad(yaw)) * cos(to_rad(pitch));
        direction.y = sin(to_rad(pitch));
        direction.z = sin(to_rad(yaw)) * cos(to_rad(pitch));
        direction.w = 0.f;
        vec4_norm(&cameraFront, &direction);

        mat4x4 view;
        vec4 cameraTarget;
        vec4_add(&cameraTarget, &cameraPos, &cameraFront);
        mat4x4_lookat(&view, &cameraPos, &cameraTarget, &cameraUp);
        shaderSetmat4x4(program, "view", &view);

        int i;
        for (i = 0; i < 4; ++i)
        {
            mat4x4 model;
            mat4x4_identity(&model);

            if (i == 3)
                mat4x4_rotx(&model, &model, timeValue * 90.f);
            else
                mat4x4_rotx(&model, &model, 20.f * i);

            mat4x4_translate(&model, &positions[i]);

            shaderSetmat4x4(program, "model", &model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO_square);

            glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_verts) / sizeof(float));
        }


        processInput(window);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 5.f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
    {
        vec4 dir;
        vec4_scale(&dir, &cameraFront, cameraSpeed);
        vec4_add(&cameraPos, &cameraPos, &dir);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        vec4 dir;
        vec4_scale(&dir, &cameraFront, cameraSpeed);
        vec4_sub(&cameraPos, &cameraPos, &dir);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vec4 ldir;
        vec4_cross(&ldir, &cameraFront, &cameraUp);
        vec4_norm(&ldir, &ldir);
        vec4_scale(&ldir, &ldir, cameraSpeed);
        vec4_sub(&cameraPos, &cameraPos, &ldir);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        vec4 rdir;
        vec4_cross(&rdir, &cameraFront, &cameraUp);
        vec4_norm(&rdir, &rdir);
        vec4_scale(&rdir, &rdir, cameraSpeed);
        vec4_add(&cameraPos, &cameraPos, &rdir);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        vec4 udir;
        vec4_scale(&udir, &cameraUp, cameraSpeed);
        vec4_add(&cameraPos, &cameraPos, &udir);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        vec4 udir;
        vec4_scale(&udir, &cameraUp, cameraSpeed);
        vec4_sub(&cameraPos, &cameraPos, &udir);
    }
    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
    {
        yaw -= 90.f * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
    {
        yaw += 90.f * deltaTime;
    }
}

float mixer;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int
        mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_SPACE && action  == GLFW_PRESS)
    {
        if (!move_mode)
        {
            glfwSetCursorPos(window, 0, 0);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            move_mode = 1;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            move_mode = 0;
        }
    }
}

void frame_buffer_callback(GLFWwindow* window, int width_n, int height_n)
{
    glViewport(0, 0, width, height);
    width = width_n;
    height = height_n;
}

void print_mat4x4(mat4x4* mat)
{
    printf("%0.2f, %0.2f, %0.2f, %0.2f,\n", mat->r1.x, mat->r1.y, mat->r1.z,
            mat->r1.w);
    printf("%0.2f, %0.2f, %0.2f, %0.2f,\n", mat->r2.x, mat->r2.y, mat->r2.z,
            mat->r2.w);
    printf("%0.2f, %0.2f, %0.2f, %0.2f,\n", mat->r3.x, mat->r3.y, mat->r3.z,
            mat->r3.w);
    printf("%0.2f, %0.2f, %0.2f, %0.2f,\n", mat->r4.x, mat->r4.y, mat->r4.z,
            mat->r4.w);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (move_mode)
    {
        float sensitivity = 0.1f;

        pitch += -ypos * sensitivity;
        yaw += xpos * sensitivity;
        glfwSetCursorPos(window, 0, 0);
    }
}
