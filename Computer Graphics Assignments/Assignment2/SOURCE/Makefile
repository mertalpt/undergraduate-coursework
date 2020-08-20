CC=g++
CFLAGS=-std=c++11 -pthread
LIBS=-lGL -lGLU -lGLEW -lglfw -I "./lib"
SRC=./src
OBJ=./obj
EXEC=Alpine.exe

OBJECTS=Main.o Logger.o Controller.o Overlay.o Scene.o ShaderUtil.o \
		IDrawable.o BodyPart.o ExtendedAngel.o HierarchicalModel.o

build: $(OBJECTS)
	${CC} ${CFLAGS} ${LIBS} -o ${EXEC} $(addprefix ${OBJ}/, $(OBJECTS))

clean:
	rm -rf ${OBJ} && mkdir ${OBJ} && rm -f ./${EXEC} \
	rm -f GL_LOGS.log && touch GL_LOGS.log

test: build
	optirun ./${EXEC}

leak-check: build
	optirun valgrind --leak-check=full ./${EXEC}

Main.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/Main.cpp -o ${OBJ}/Main.o

Logger.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/Logger.c -o ${OBJ}/Logger.o

Controller.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/Controller.cpp -o ${OBJ}/Controller.o

Overlay.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/Overlay.cpp -o ${OBJ}/Overlay.o

Scene.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/Scene.cpp -o ${OBJ}/Scene.o

ShaderUtil.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/ShaderUtil.cpp -o ${OBJ}/ShaderUtil.o

IDrawable.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/IDrawable.cpp -o ${OBJ}/IDrawable.o

BodyPart.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/BodyPart.cpp -o ${OBJ}/BodyPart.o

ExtendedAngel.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/ExtendedAngel.cpp -o ${OBJ}/ExtendedAngel.o

HierarchicalModel.o:
	${CC} ${CFLAGS} ${LIBS} -c ${SRC}/HierarchicalModel.cpp -o ${OBJ}/HierarchicalModel.o
