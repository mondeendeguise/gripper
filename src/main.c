#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "shader.h"
#include "mesh.h"
#include "vector.h"
#include "matrix.h"
#include "transforms.h"
#include "camera.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define WINDOW_NAME "GRIPPER"

static void glfw_error_callback(int error, const char *description);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define return_defer(value) do { result = (value); goto defer; } while(0)

static int window_width = WINDOW_WIDTH;
static int window_height = WINDOW_HEIGHT;

#define INPUT_MOVE_FORWARD   1     // 0000 0001
#define INPUT_MOVE_BACKWARD  2     // 0000 0010
#define INPUT_MOVE_LEFT      4     // 0000 0100
#define INPUT_MOVE_RIGHT     8     // 0000 1000

#define INPUT_LOOK_UP        16    // 0001 0000
#define INPUT_LOOK_DOWN      32    // 0010 0000
#define INPUT_LOOK_LEFT      64    // 0100 0000
#define INPUT_LOOK_RIGHT     128   // 1000 0000

#define SET_BIT(num, bit) num = num | bit
#define CLEAR_BIT(num, bit) num = num & ~ bit

static unsigned short int input = {0};

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
    if(!window) return_defer(-1);

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
    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    const float acceleration = 3.0f;
    const float deceleration = 0.7f;
    const float sensitivity = 4.0f;

    V3f pos = {0};
    V3f facing = {0};
    V3f velocity = {0};

    while(!glfwWindowShouldClose(window)) {
        current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
        glUniform1f(uniform_time, delta_time);

        /* TODO: Represent WASD movement direction as a normalized vector.
         *       Current implementation causes diagonal movement to be faster
         *       than movement in cardinal directions. */

        if(input & INPUT_MOVE_FORWARD) {
            velocity.c[2] += cosf(degrees_to_radians(0.0f) + facing.c[1]) * acceleration;
            velocity.c[0] += sinf(degrees_to_radians(0.0f) + facing.c[1]) * acceleration;
        }

        if(input & INPUT_MOVE_LEFT) {
            velocity.c[2] += cosf(degrees_to_radians(270.0f) + facing.c[1]) * acceleration;
            velocity.c[0] += sinf(degrees_to_radians(270.0f) + facing.c[1]) * acceleration;
        }

        if(input & INPUT_MOVE_BACKWARD) {
            velocity.c[2] += cosf(degrees_to_radians(180.0f) + facing.c[1]) * acceleration;
            velocity.c[0] += sinf(degrees_to_radians(180.0f) + facing.c[1]) * acceleration;
        }

        if(input & INPUT_MOVE_RIGHT) {
            velocity.c[2] += cosf(degrees_to_radians(90.0f) + facing.c[1]) * acceleration;
            velocity.c[0] += sinf(degrees_to_radians(90.0f) + facing.c[1]) * acceleration;
        }

        if(velocity.c[0]) velocity.c[0] *= deceleration;
        if(velocity.c[1]) velocity.c[1] *= deceleration;
        if(velocity.c[2]) velocity.c[2] *= deceleration;

        /* v3f_print(velocity); */

        pos = v3f_add(pos, v3f_multiply(velocity, v3ff(delta_time)));

        if(input & INPUT_LOOK_UP)       facing.c[0] += sensitivity * delta_time;
        if(input & INPUT_LOOK_LEFT)     facing.c[1] -= sensitivity * delta_time;
        if(input & INPUT_LOOK_DOWN)     facing.c[0] -= sensitivity * delta_time;
        if(input & INPUT_LOOK_RIGHT)    facing.c[1] += sensitivity * delta_time;

        M4x4f model = m4x4f_diagonal(1.0f);
        model = m4x4f_multiply(model, m4x4f_rotate_z(current_time/2 * degrees_to_radians(180.0f)));
        model = m4x4f_multiply(model, m4x4f_rotate_y(current_time/2 * degrees_to_radians(180.0f)));
        model = m4x4f_multiply(model, m4x4f_rotate_x(current_time/2 * degrees_to_radians(180.0f)));

        M4x4f view = m4x4f_diagonal(1.0f);
        view = m4x4f_multiply(view, m4x4f_rotate_x(facing.c[0]));
        view = m4x4f_multiply(view, m4x4f_rotate_y(-facing.c[1]));
        view = m4x4f_multiply(view, m4x4f_translate_v3f(v3f_subtract(v3ff(0.0f), pos)));

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

#define REGISTER_KEYMAP(controller, key, bit) \
    case (key): {                             \
        if(action == GLFW_PRESS) {            \
            SET_BIT((controller), (bit));     \
        } else if(action == GLFW_RELEASE) {   \
            CLEAR_BIT((controller), (bit));   \
        }                                     \
    } break

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;
    switch(key) {
        case GLFW_KEY_ESCAPE: {
            if(action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
        } break;

        REGISTER_KEYMAP(input, GLFW_KEY_W, INPUT_MOVE_FORWARD);
        REGISTER_KEYMAP(input, GLFW_KEY_A, INPUT_MOVE_LEFT);
        REGISTER_KEYMAP(input, GLFW_KEY_S, INPUT_MOVE_BACKWARD);
        REGISTER_KEYMAP(input, GLFW_KEY_D, INPUT_MOVE_RIGHT);

        REGISTER_KEYMAP(input, GLFW_KEY_UP, INPUT_LOOK_UP);
        REGISTER_KEYMAP(input, GLFW_KEY_LEFT, INPUT_LOOK_LEFT);
        REGISTER_KEYMAP(input, GLFW_KEY_DOWN, INPUT_LOOK_DOWN);
        REGISTER_KEYMAP(input, GLFW_KEY_RIGHT, INPUT_LOOK_RIGHT);

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
