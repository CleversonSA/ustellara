# Elegant makefile

# Name of the project
PROJ_NAME=ustellara

# PREFIX is enviroment variable, but if its not set, then set default value
ifeq ($(PREFIX),)
  PREFIX := /usr/local
endif

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
	 -Wall	\
	 -fcommon

# Command used at clean target
RM = rm -rf

# Compiling and linking
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o ./build/$@ -lncurses -lpthread
	@ echo 'Finished building binary:  $@'
	@ echo  ' '

./build/objects/%.o: ./src/%.c ./src/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ -lncurses -lpthread
	@ echo ' '

./build/objects/main.o: ./src/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $< '
	$(CC) $< $(CC_FLAGS) -o $@ -lncurses -lpthread
	@ echo ' '

objFolder:
	@ mkdir -p build
	@ mkdir -p build/objects

install: 
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 655 ./build/$(PROJ_NAME) $(DESTDIR)$(PREFIX)/bin

clean:
	@ $(RM) ./build/objects/*.o $(PROJ_NAME)
	@ rmdir ./build/objects

.PHONY: all clean
