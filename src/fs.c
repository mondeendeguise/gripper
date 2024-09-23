#include "fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

char *buffer_file(const char *file_path)
{
    char *buf = NULL;
    FILE *fp = NULL;

    fp = fopen(file_path, "r");
    if(fp == NULL) goto defer;
    if(fseek(fp, 0L, SEEK_END) < 0) goto defer;

    long size = ftell(fp);
    if(size < 0) goto defer;

    buf = malloc(size + 1);
    if(buf == NULL) goto defer;

    if(fseek(fp, 0, SEEK_SET) < 0) goto defer;

    fread(buf, 1, size, fp);
    if(ferror(fp)) goto defer;

    buf[size] = '\0';

    if(fp) {
        fclose(fp);
        errno = 0;
    }

    return buf;

defer:

    if(fp) {
        int s = errno;
        fclose(fp);
        errno = s;
    }

    if(buf) {
        free(buf);
    }

    return NULL;
}
