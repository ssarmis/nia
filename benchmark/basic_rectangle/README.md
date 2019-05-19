## Nia benchmark
### Run benchmark using GCC
```g++ -o a nia.cpp -L../../build -I../../include -lnia -lX11 -lGL -lbenchmark -lpthread; ./a```

## SDL benchmark
### Installing on Ubuntu
```sudo apt-get install libsdl2-dev```
### Run benchmark using GCC
```g++ -o a sdl.cpp -DSDL_BENCHMARK -lSDL2main -lSDL2 -lbenchmark -lpthread; ./a```

## SFML benchmark
### Installing on Ubuntu
```sudo apt-get install libsfml-dev```
### Run benchmark using GCC
```g++ -o a sfml.cpp -DSFML_BENCHMARK -lsfml-graphics -lsfml-window -lsfml-system -lbenchmark -lpthread; ./a```

## Run Glut benchmark
```g++ -o a glut.cpp -DGLUT_BENCHMARK -lglut -lGL -lGLU -lbenchmark -lpthread; ./a```

# Will come:
    - SIGIL (http://www.libsigil.com/)
    - CINDER (https://libcinder.org/)