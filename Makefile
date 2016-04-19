CC = g++

INCLUDES = -Ibullet/ -Iutils/ -Isrc -Isrc/strategies
LIBRARIES = -lGL -lGLU -lGL -lstdc++ -lc -lm -lglut -lGLU -pthread

SRC := $(shell find src/ -name '*.cpp')
FILE_NAMES_SRC = $(SRC:.cpp=.o)

UTILS := $(wildcard utils/*.cpp)
FILE_NAMES_UTILS = $(UTILS:.cpp=.o)

SRCBULLET = bullet/
SOURCES := $(shell find $(SRCBULLET) -name '*.cpp')
FILE_NAMES_BULLET = $(SOURCES:.cpp=.o)

EXEC = Simulator


.cpp.o:
	$(CC) $(INCLUDES) $(LIBRARIES) -c -Wall -ffast-math -o $@ $< -w

all: $(EXEC)
	@echo Executando... Aguarde.

run:
	./Simulator

$(EXEC): $(FILE_NAMES_SRC) $(FILE_NAMES_BULLET) $(FILE_NAMES_UTILS)
	@$(CC) -Wall -fexceptions -g -ffast-math -m64 -fno-rtti -w  $(LIBRARIES) -o $(EXEC) $(FILE_NAMES_SRC) $(FILE_NAMES_BULLET) $(FILE_NAMES_UTILS) $(INCLUDES)

clean:
	rm $(EXEC) $(FILE_NAMES_SRC) $(FILE_NAMES_BULLET) $(FILE_NAMES_UTILS)




