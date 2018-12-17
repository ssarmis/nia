EXAMPLES_ROOT	:= ../..
ROOT			:= .

# Used for examples
CC      		:= g++
CFLAGS 			:= -Wall -g
CINCLUDE		:= -I$(EXAMPLES_ROOT)/include

LIBSPATH		:= $(EXAMPLES_ROOT)/build
LIBS 			:= -lnia -lopengl32 -lgdi32

SRCS 			:= $(shell echo *.cpp)
OBJS 			:= $(SRCS:.cpp=.o)

APP		 		:= example

# Used for help
HELP			:= 'If inside an example directory, type "make run"\n'
HELP			:= $(HELP)'If in main directory(in the "nia" folder) type "make build_lib" to build the library\n'
HELP 			:= $(HELP)'If you want to run a quick test, type "make test" (always after "make build_lib")'


default:
	@$(MAKE) -s wrong_run

help:
	@echo $(HELP)

wrong_run:
	@echo Please consider not running 'make' with no additional arguments\nType 'make help' to get the possible options!

build_lib:
	@mkdir -p build

ifeq ($(DEBUG),)
	@cmake -Bbuild -H$(ROOT) -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"
else
	@cmake -Bbuild -H../ -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_BUILD_TYPE=Debug
endif
	@make -C build

# For examples
run: 
	@$(MAKE) $(APP)

test:
	@make run -C examples/basic_batch_renderer/

$(OBJS): $(SRCS)
	@$(CC) -c $(SRCS) $(CFLAGS)	$(CINCLUDE)

$(APP): $(OBJS)
	@$(CC) -o $(EXAMPLES_ROOT)/build/$@ $< $(CFLAGS) $(CINCLUDE) -L$(LIBSPATH) $(LIBS)
	$(EXAMPLES_ROOT)/build/$@

clean: 
	@rm -rf build/
	@rm -f *.o $(APP)
