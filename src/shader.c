#include "shader.h"
#include "fs.h"

#include <GL/glew.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool compile_shader(GLuint *shader, const char *path, GLenum type)
{
    GLchar *source = buffer_file(path);
    if(!source) {
        fprintf(stderr, "failed to read `%s`: %s\n", path, strerror(errno));
        return false;
    }

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, (const GLchar * const *)&source, NULL);
    glCompileShader(*shader);

    free(source);

    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(*shader, 512, NULL, buffer);
        fprintf(stderr, "failed to compile shader `%s`:\n\t%s", path, buffer);

        return false;
    }

    return true;
}
