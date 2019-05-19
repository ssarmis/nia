## Run Nia benchmark
```make run```

## Run SDL benchmark 
```g++ -o a sdl.cpp -DSDL_BENCHMARK -lSDL2main -lSDL2 -lbenchmark -lpthread; ./a```

## Run SFML benchmark
```g++ -o a sfml.cpp -DSFML_BENCHMARK -lsfml-graphics -lsfml-window -lsfml-system -lbenchmark -lpthread; ./a```

## Run Glut benchmark
```g++ -o a glut.cpp -DGLUT_BENCHMARK -lglut -lGL -lGLU -lbenchmark -lpthread; ./a```

# Will come:
    - SIGIL (http://www.libsigil.com/)
    - CINDER (https://libcinder.org/)