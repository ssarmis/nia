#include "nia.h"

#include "assert.h"

niaMatrix4 test0(2);
niaMatrix4 test1(10);
niaMatrix4 test2;

int main(){
{   // test2 matrix
    test2.m[0 ] = 16;  test2.m[1 ] = 142; test2.m[2 ] = 190; test2.m[3 ] = 20;
    test2.m[4 ] = 200; test2.m[5 ] = 20;  test2.m[6 ] = 258; test2.m[7 ] = 20;
    test2.m[8 ] = 154; test2.m[9 ] = 20;  test2.m[10] = 270; test2.m[11] = 20;
    test2.m[12] = 202; test2.m[13] = 20;  test2.m[14] = 200; test2.m[15] = 20;
}

    niaMatrix4 a0;
    niaMatrix4 b0;
    niaMatrix4 c0 = a0.add(b0);
    assert(niaMatrix4::compareMat4(c0, test0));

    niaMatrix4 a1(2);
    niaMatrix4 b1(8);
    niaMatrix4 c1 = a1.add(b1);
    assert(niaMatrix4::compareMat4(c1, test1));

    niaMatrix4 a2;
    a2.m[0 ] = 3;  a2.m[1 ] = 21; a2.m[2 ] = 45; a2.m[3 ] = 5;
    a2.m[4 ] = 50; a2.m[5 ] = 5;  a2.m[6 ] = 79; a2.m[7 ] = 5;
    a2.m[8 ] = 27; a2.m[9 ] = 5;  a2.m[10] = 85; a2.m[11] = 5;
    a2.m[12] = 51; a2.m[13] = 5;  a2.m[14] = 50; a2.m[15] = 5;

    niaMatrix4 b2(8);
    b2.m[0 ] = 13;  b2.m[1 ] = 121; b2.m[2 ] = 145; b2.m[3 ] = 15;
    b2.m[4 ] = 150; b2.m[5 ] = 15;  b2.m[6 ] = 179; b2.m[7 ] = 15;
    b2.m[8 ] = 127; b2.m[9 ] = 15;  b2.m[10] = 185; b2.m[11] = 15;
    b2.m[12] = 151; b2.m[13] = 15;  b2.m[14] = 150; b2.m[15] = 15;

    niaMatrix4 c2 = a2.add(b2);
    assert(niaMatrix4::compareMat4(c2, test2));

    return 0;
}