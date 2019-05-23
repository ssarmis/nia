# TODO add variable to be overriden in makefile with path
PROJECT_ROOT	?= ../..
ROOT			:= .

# Used for examples
CC      		:= g++
CFLAGS 			:= -Wall -g -Wunused-variable
CINCLUDE		:= -I$(PROJECT_ROOT)/include

LIBSPATH		:= $(PROJECT_ROOT)/build
ifeq ($(OS),Windows_NT)
LIBS 			:= -lnia -lopengl32 -lgdi32
else
LIBS 			:= -lnia -lX11 -lGL
endif

SRCS 			:= $(shell echo *.cpp)
OBJS 			:= $(SRCS:.cpp=.o)

APP		 		:= example

# Used for help
HELP			:= 'If inside an example directory, type "make run"\n'
HELP			:= $(HELP)'If in main directory(in the "nia" folder) type "make build_lib" to build the library, for debugging add DEBUG=true\n'
HELP 			:= $(HELP)'If you want to run a quick test, type "make test" (always after "make build_lib")'


default:
	@$(MAKE) -s wrong_run

help:
	@echo $(HELP)

wrong_run:
	@echo Please consider not running 'make' with no additional arguments\nType 'make help' to get the possible options!

build_lib:
	@mkdir -p build

# @cmake -Bbuild -H$(ROOT) -G "Visual Studio 15 2017" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
ifeq ($(DEBUG),)
# windows or not
ifeq ($(OS),Windows_NT)
	@cmake -Bbuild -H$(ROOT) -G "Visual Studio 15" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
	# @cmake -Bbuild -H$(ROOT) -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
	# @mingw32-make -C build/
else
	@cmake -Bbuild -H$(ROOT) -G "Unix Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
	@make -C build
endif

else
# windows or not
ifeq ($(OS),Windows_NT)
	@cmake -Bbuild -H$(ROOT) -G "Visual Studio 15" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
	# @cmake -Bbuild -H$(ROOT) -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_BUILD_TYPE=Debug
	# @mingw32-make -C build
else
	@cmake -Bbuild -H$(ROOT) -G "Unix Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_BUILD_TYPE=Debug
	@make -C build
endif
endif

# For examples
run: 
	@$(MAKE) $(APP)

test_lib:
ifeq ($(OS),Windows_NT)
	@mingw32-make run -C examples/basic_batch_renderer/
else
	@make run -C examples/basic_batch_renderer/
endif

$(OBJS): $(SRCS)
	@$(CC) -c $(SRCS) $(CFLAGS)	$(CINCLUDE)

$(APP): $(OBJS)
	@$(CC) -o $(PROJECT_ROOT)/build/$@ $< $(CFLAGS) $(CINCLUDE) -L$(LIBSPATH) $(LIBS)
	$(PROJECT_ROOT)/build/$@

clean: 
	@rm -rf build/
	@rm -f *.o $(APP)
