# Elegant makefile

# Name of the project
PROJ_NAME=ustellara

# .c files
C_SOURCE=$(wildcard ./src/*.c)

# .h files
H_SOURCE=$(wildcard ./src/*.h)

# Object files
OBJ=$(subst .c,.o,$(subst src,build/objects,$(C_SOURCE)))

# Compiler and linker
CC=gcc

# Flags for compiler
CC_FLAGS=-c	\
	 -W	\
	 -Wall	

# Command used at clean target
RM = rm -rf

# Compiling and linking
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ -lncurses
	@ echo 'Finished building binary:  $@'
	@ echo  ' '

./objects/%.o: ./src/%.c ./src/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ -lncurses
	@ echo ' '

./objects/main.o: ./src/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $< '
	$(CC) $< $(CC_FLAGS) -o $@ -lncurses
	@ echo ' '

objFolder:
	@ mkdir -p build
	@ mkdir -p build/objects

clean:
	@ $(RM) ./build/objects/*.o $(PROJ_NAME)
	@ rmdir ./build/objects
	@ mv ./build/objects/* ./build/

.PHONY: all clean
