: g++ -o a src/*.cpp -Iinclude/ -lopengl32 -lgdi32
g++ -c -DNIA_LIBRARY_BUILD src/*.cpp -Iinclude/
: in case I forget, I link opengl here because I use it internally
g++ -shared -o nia.dll nia_event.o nia_window.o nia_renderer.o -lopengl32 -lgdi32
: I link opengl here as well because I might want to use some after the library was made
g++ -o a test.cpp -L. -lnia -Iinclude/ -lopengl32