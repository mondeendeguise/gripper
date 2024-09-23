#!/usr/bin/env bash

set -xe

PACKAGE=gripper

CC=gcc
CFLAGS="-Wall -Wextra -pedantic -O2"
LINKER=gcc
LIBS="-lglfw -lGLEW -lGL -lm"

SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=${BUILD_DIR}/obj
BIN=${BUILD_DIR}/${PACKAGE}

mkdir -p ${BUILD_DIR}
mkdir -p ${OBJ_DIR}

${CC} -o ${OBJ_DIR}/main.c.o ${CFLAGS} -c ${SRC_DIR}/main.c
${CC} -o ${OBJ_DIR}/fs.c.o ${CFLAGS} -c ${SRC_DIR}/fs.c
${CC} -o ${OBJ_DIR}/shader.c.o ${CFLAGS} -c ${SRC_DIR}/shader.c
${CC} -o ${OBJ_DIR}/mesh.c.o ${CFLAGS} -c ${SRC_DIR}/mesh.c
${CC} -o ${OBJ_DIR}/vector.c.o ${CFLAGS} -c ${SRC_DIR}/vector.c
${CC} -o ${OBJ_DIR}/matrix.c.o ${CFLAGS} -c ${SRC_DIR}/matrix.c
${CC} -o ${OBJ_DIR}/transforms.c.o ${CFLAGS} -c ${SRC_DIR}/transforms.c

${LINKER} -o ${BIN} ${OBJ_DIR}/*.c.o ${LIBS}
