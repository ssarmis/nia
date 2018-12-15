CC := g++

lib:
	@$(CC) -c -DNIA_LIBRARY_BUILD src/*.cpp src/arch/win32/*.cpp -Iinclude/
	@$(CC) -shared -o nia.dll *.o -lopengl32 -lgdi32

build_test:
	@$(CC) -o ./build/test examples/test.cpp -Lbuild/ -lnia -Iinclude/ -lopengl32

run_test:
	@$(MAKE) build_test
	@./build/test.exe

clean:
	@rd build
	@rm *.o
