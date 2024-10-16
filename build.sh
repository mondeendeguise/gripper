#!/usr/bin/env bash

set -xe

PACKAGE=gripper

CC=gcc
CFLAGS="-Wall -Wextra -pedantic -O2"
LINK=gcc
LIBS="-lglfw -lGLEW -lGL -lm"

SRC_DIR=src
BUILD_DIR=build
BIN=${BUILD_DIR}/${PACKAGE}

mkdir -p ${BUILD_DIR}
mkdir -p ${BUILD_DIR}/lib
mkdir -p ${BUILD_DIR}/gmath

${CC} -o ${BUILD_DIR}/gmath/vector.c.o ${CFLAGS} -fPIC -c ${SRC_DIR}/gmath/vector.c
${CC} -o ${BUILD_DIR}/gmath/matrix.c.o ${CFLAGS} -fPIC -c ${SRC_DIR}/gmath/matrix.c
${CC} -o ${BUILD_DIR}/gmath/transforms.c.o ${CFLAGS} -fPIC -c ${SRC_DIR}/gmath/transforms.c
${CC} -o ${BUILD_DIR}/gmath/camera.c.o ${CFLAGS} -fPIC -c ${SRC_DIR}/gmath/camera.c

${LINK} -o ${BUILD_DIR}/lib/libgmath.so -shared ${BUILD_DIR}/gmath/*.c.o

${CC} -o ${BUILD_DIR}/main.c.o ${CFLAGS} -c ${SRC_DIR}/main.c
${CC} -o ${BUILD_DIR}/fs.c.o ${CFLAGS} -c ${SRC_DIR}/fs.c
${CC} -o ${BUILD_DIR}/shader.c.o ${CFLAGS} -c ${SRC_DIR}/shader.c
${CC} -o ${BUILD_DIR}/mesh.c.o ${CFLAGS} -c ${SRC_DIR}/mesh.c

${LINK} -o ${BIN} ${BUILD_DIR}/*.c.o ${LIBS} -L${BUILD_DIR}/lib -Wl,-rpath=${BUILD_DIR}/lib -lgmath
