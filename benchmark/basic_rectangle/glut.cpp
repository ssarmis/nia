#ifdef GLUT_BENCHMARK
#include <GL/glut.h>
#include "../benchmark/include/benchmark/benchmark.h"

void Display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
        glVertex2f(0, 0); 
        glVertex2f(0, -1);
        glVertex2f(1, -1); 
        glVertex2f(1, 0); 
    glEnd();
    glFlush();
} 

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    while(state.KeepRunning()){
        Display();
    }
}
BENCHMARK(BM_RENDER_RECTANGLE);

int main(int argc, char **argv){ 	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(200,200);
    glutCreateWindow("GLUT benchmark");
	
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
#endif