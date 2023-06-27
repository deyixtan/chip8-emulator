FLAGS=-g -Wall -Wextra -Werror
INCLUDES=-I .\include
LIBS=-L .\lib -lmingw32 -lSDL2main -lSDL2
OBJECTS=.\build\chip8memory.o .\build\chip8stack.o .\build\chip8keyboard.o  .\build\chip8display.o .\build\chip8instruction.o .\build\chip8.o .\build\emulator.o

all: ${OBJECTS}
	gcc .\src\main.c ${FLAGS} ${INCLUDES} ${OBJECTS} ${LIBS} -o .\bin\chip8

clean:
	del build\*.o

.\build\chip8memory.o: src\chip8memory.c
	gcc .\src\chip8memory.c ${FLAGS} ${INCLUDES} -c -o .\build\chip8memory.o

.\build\chip8stack.o: src\chip8stack.c
	gcc .\src\chip8stack.c ${FLAGS} ${INCLUDES} -c -o .\build\chip8stack.o

.\build\chip8keyboard.o: src\chip8keyboard.c
	gcc .\src\chip8keyboard.c ${FLAGS} ${INCLUDES} -c -o .\build\chip8keyboard.o

.\build\chip8display.o: src\chip8display.c
	gcc .\src\chip8display.c ${FLAGS} ${INCLUDES} -c -o .\build\chip8display.o

.\build\chip8instruction.o: src\chip8instruction.c
	gcc .\src\chip8instruction.c ${FLAGS} ${INCLUDES} -c -o .\build\chip8instruction.o

.\build\chip8.o: src\chip8.c
	gcc .\src\chip8.c ${FLAGS} ${INCLUDES} -c -o .\build\chip8.o

.\build\emulator.o: src\emulator.c
	gcc .\src\emulator.c ${FLAGS} ${INCLUDES} -c -o .\build\emulator.o
