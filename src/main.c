#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"
#include "mesh.h"
#include "vector.h"
#include "matrix.h"
#include "transforms.h"
#include "camera.h"
#include "sv.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define WINDOW_NAME "GRIPPER"

static void glfw_error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define return_defer(value) { result = (value); goto defer; }

static int window_width = WINDOW_WIDTH;
static int window_height = WINDOW_HEIGHT;

#define INPUT_FORWARD   1
#define INPUT_BACKWARD  2
#define INPUT_LEFT      4
#define INPUT_RIGHT     8
static unsigned char input = {0};

int main(void)
{
    int result = 0;
    GLFWwindow *window = NULL;

    char *vert_shader_source = NULL;
    char *frag_shader_source = NULL;

    glfwSetErrorCallback(glfw_error_callback);

    if(!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // OpenGL minimum version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if(!window) return_defer(-1)

    glfwMakeContextCurrent(window);

    // GLFW Context ready; initialize OpenGL
    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "GLEW ERROR: %s\n", glewGetErrorString(err));
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1);

    // OpenGL setup done

    Vertex vertices[] = {
        vertex(v3f(-0.5f,  0.5f,  0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
        vertex(v3f(-0.5f, -0.5f,  0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
        vertex(v3f( 0.5f, -0.5f,  0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
        vertex(v3f( 0.5f,  0.5f,  0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),

        vertex(v3f(-0.5f,  0.5f, -0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
        vertex(v3f(-0.5f, -0.5f, -0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
        vertex(v3f( 0.5f, -0.5f, -0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
        vertex(v3f( 0.5f,  0.5f, -0.5f), v4f(1.0f, 0.0f, 0.0f, 1.0f)),
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0,

        3, 2, 6,
        6, 7, 3,

        7, 6, 5,
        5, 4, 7,

        4, 5, 1,
        1, 0, 4,

        4, 0, 3,
        3, 7, 4,

        1, 5, 6,
        6, 2, 1,
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(VA_POS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));
    glEnableVertexAttribArray(VA_POS);

    glVertexAttribPointer(VA_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));
    glEnableVertexAttribArray(VA_COLOR);

    GLuint vertex_shader;
    if(!compile_shader(&vertex_shader, "shaders/main.vert", GL_VERTEX_SHADER)) return_defer(-2);

    GLuint fragment_shader;
    if(!compile_shader(&fragment_shader, "shaders/main.frag", GL_FRAGMENT_SHADER)) return_defer(-2);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glBindFragDataLocation(shader_program, 0, "out_color");
    glLinkProgram(shader_program);

    glUseProgram(shader_program);
    
    double current_time = 0.0f;
    double last_time = 0.0f;
    double delta_time = 0.0f;

    // Uniforms
    GLint uniform_time = glGetUniformLocation(shader_program, "time");
    GLint uniform_mvp = glGetUniformLocation(shader_program, "mvp");

    // Wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    V3f pos = {0};
    float speed = 5.0f;

    while(!glfwWindowShouldClose(window)) {
        current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
        glUniform1f(uniform_time, delta_time);

        if(input & INPUT_FORWARD)  pos.c[2] += delta_time * speed;
        if(input & INPUT_BACKWARD) pos.c[2] -= delta_time * speed;
        if(input & INPUT_LEFT)     pos.c[0] -= delta_time * speed;
        if(input & INPUT_RIGHT)    pos.c[0] += delta_time * speed;

        M4x4f model = m4x4f_diagonal(1.0f);
        model = m4x4f_multiply(model, m4x4f_rotation_z(current_time/2 * degrees_to_radians(180.0f)));
        model = m4x4f_multiply(model, m4x4f_rotation_y(current_time/2 * degrees_to_radians(180.0f)));
        model = m4x4f_multiply(model, m4x4f_rotation_x(current_time/2 * degrees_to_radians(180.0f)));

        M4x4f view = m4x4f_translate_v3f(v3f_subtract(v3ff(0.0f), pos));

        float aspect_ratio = (float) window_height / (float) window_width;
        float fov = 90.0f;
        M4x4f projection = m4x4f_projection(aspect_ratio, fov, 0.1f, 100.0f);

        M4x4f mvp = m4x4f_diagonal(1.0f);
        mvp = m4x4f_multiply(mvp, projection);
        mvp = m4x4f_multiply(mvp, view);
        mvp = m4x4f_multiply(mvp, model);

        glUniformMatrix4fv(uniform_mvp, 1, GL_TRUE, mvp.c);

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

defer:

    if(vert_shader_source) free(vert_shader_source);
    if(frag_shader_source) free(frag_shader_source);

    if(window) glfwDestroyWindow(window);
    glfwTerminate();

    return result;
}

static void glfw_error_callback(int error, const char *description)
{
    (void) error;
    fprintf(stderr, "GLFW ERROR: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;
    switch(key) {
        case GLFW_KEY_ESCAPE: {
            if(action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
        } break;

        case GLFW_KEY_W: {
            if(action == GLFW_PRESS) {
                input = input | INPUT_FORWARD;
            } else if(action == GLFW_RELEASE) {
                input = input & ~ INPUT_FORWARD;
            }
        } break;

        case GLFW_KEY_S: {
            if(action == GLFW_PRESS) {
                input = input | INPUT_BACKWARD;
            } else if(action == GLFW_RELEASE) {
                input = input & ~ INPUT_BACKWARD;
            }
        } break;

        case GLFW_KEY_A: {
            if(action == GLFW_PRESS) {
                input = input | INPUT_LEFT;
            } else if(action == GLFW_RELEASE) {
                input = input & ~ INPUT_LEFT;
            }
        } break;

        case GLFW_KEY_D: {
            if(action == GLFW_PRESS) {
                input = input | INPUT_RIGHT;
            } else if(action == GLFW_RELEASE) {
                input = input & ~ INPUT_RIGHT;
            }
        } break;

        default: break;
    }
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void) window;
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
}
