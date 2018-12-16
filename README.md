# Nia

For debugging use '-DCMAKE_BUILD_TYPE=Debug' when using cmake<br>
## Windows

### Mingw32:
```git clone https://github.com/streanga-sarmis/nia```<br>
```cd nia/```<br>
```mkdir build```<br>
```cd build/```<br>
```cmake ../ -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"```<br>
```mingw32-make```<br>

### MSVC: (untested)
```git clone https://github.com/streanga-sarmis/nia```<br>
```cd nia/```<br>
```mkdir build```<br>
```cd build/```<br>
```cmake ../ -G "Visual Studio XX"```<br>
Now you should have a Visual Studio 20XX project that you can build into a shared library


### Or raw g++:
```g++ -c -DNIA_LIBRARY_BUILD src/*.cpp src/arch/win32/*.cpp -Iinclude/```<br>
```g++ -shared -o nia.dll *.o -lopengl32 -lgdi32```<br>
```g++ -o ./build/a examples/test.cpp -Lbuild/ -lnia -Iinclude/ -lopengl32```<br>

## Linux

### Use:
```git clone https://github.com/streanga-sarmis/nia```<br>
```cd nia/```<br>
```mkdir build```<br>
```cd build/```<br>
```cmake ../```<br>
```make```

### Or raw g++:
```g++ -c -DNIA_LIBRARY_BUILD ./src/*.cpp ./src/arch/win32/*.cpp ./src/arch/linux/*.cpp -fpic -I./include/```<br>
```g++ -shared -o libnia.so *.o -lX11 -lGL -lGLU```<br>
```g++ -o a ./examples/test.cpp -L. -lnia -I./include/ -lGL```<br>
```export LD_LIBRARY_PATH=/home/<your user here>/nia:$LD_LIBRARY_PATH```<br>