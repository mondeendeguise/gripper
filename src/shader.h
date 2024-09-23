#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include <stdbool.h>

bool compile_shader(GLuint *shader, const char *path, GLenum type);

#endif // SHADER_H_
