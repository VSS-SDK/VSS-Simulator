CCX = g++ -std=c++11 -w


INCLUDES = -Ibullet/ -Iutils/ -Isrc -Isrc/strategies

LIBRARIES_GLUT = -lGL -lGLU -lGL -lstdc++ -lglut
LIBRARIES_PROTO = `pkg-config --cflags --libs protobuf`
LIBRARIES_BOOST = -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread
#LIBRARIES_BOOST = -pthread

LIBRARIES = $(LIBRARIES_PROTO) $(LIBRARIES_GLUT) $(LIBRARIES_BOOST) -lzmq -pthread -lm -lc -lm -w



SRC := $(shell find -name '*.cpp')
FILE_NAMES_SRC = $(SRC:.cpp=.o)

PROTOS := $(shell find -name '*.cc')
FILE_NAMES_PROTOS = $(PROTOS:.cc=.o)

FILE_NAMES = $(FILE_NAMES_SRC) $(FILE_NAMES_PROTOS) 



RELEASE = VSS-Simulator
DEBUG = VSS-Simulator-Debug

.cpp.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -ffast-math -c -o $@ $< -w

.cc.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -ffast-math -c -o $@ $< -w

all: message_compiling $(RELEASE)
	@echo Done ...

build_debug: 
	$(CCX) -g $(shell find -name '*.cpp') $(shell find -name '*.cc') $(LIBRARIES) $(INCLUDES) -o $(DEBUG)
	
message_compiling:
	@echo Compiling VSS-Simulator ...

message_cleaning:
	@echo Cleaning VSS-Simulator ...

run:
	./$(RELEASE)

$(RELEASE): $(FILE_NAMES)
	@$(CCX) -o $(RELEASE) $(FILE_NAMES) $(LIBRARIES) $(INCLUDES)

clean: message_cleaning
	@rm $(RELEASE) $(FILE_NAMES)

proto:
	cd src/VSS-Interface/protos && make -f protos.make

help: 
	./$(RELEASE) -h
#teste: 
#	$(SRC)

