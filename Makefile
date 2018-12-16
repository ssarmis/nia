CC := g++

test_lib:
	@$(CC) -DNIA_LIBRARY_BUILD -DNIA_DEBUG_BUILD -o ./build/test src/*.cpp src/arch/win32/*.cpp examples/*.cpp -Iinclude/ -lopengl32 -lgdi32 

lib:
	@$(CC) -c -DNIA_LIBRARY_BUILD src/*.cpp src/arch/win32/*.cpp -Iinclude/
	@$(CC) -shared -o nia.dll *.o -lopengl32 -lgdi32

build_test:
	@$(CC) -o ./build/test examples/test.cpp -Lbuild/ -lnia -Iinclude/ -lopengl32 -g

run_test:
	@$(MAKE) build_test
	@./build/test.exe

clean:
	@rd build
	@rm *.o
