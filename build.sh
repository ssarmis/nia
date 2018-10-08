#  g++ -o a src/*.cpp -Iinclude/ -lopengl32 -lgdi32
g++ -c -DNIA_LIBRARY_BUILD ./src/*.cpp ./src/arch/win32/*.cpp ./src/arch/linux/*.cpp -fpic -I./include/
# in case I forget, I link opengl here because I use it internally
g++ -shared -o nia.a nia_event.o nia_window.o nia_renderer.o -lX11 -lGL -lGLU
# I link opengl here as well because I might want to use some after the library was made
g++ -o a test.cpp -L. -lnia -I./include/ -lX11 -lGL -lGLU
export LD_LIBRARY_PATH=/home/stephan/nia:$LD_LIBRARY_PATH