#include "basic_math.c"
#include <stdio.h>

int allTests = 0;
int passedTests = 0;

// test da thing
void testVec3(Vec3* e, Vec3* a) {
    int valid = 1;
    if (e->x != a->x) {
        valid = 0;
    }
    if (e->y != a->y) {
        valid = 0;
    }
    if (e->z != a->z) {
        valid = 0;
    }

    allTests++;
    if (!valid) {
        printf("FAIL %f %f %f\n", a->x, a->y, a->z);
    }
    else {
        printf("pass\n");
        passedTests++;
    }
}

void testTriangle(Triangle* e, Triangle* a) {
    for (int i = 0; i < 3; i++) {
        testVec3(&e->p[i], &a->p[i]);
    }
}

void testDouble(double e, double a) {
    int valid = 1;
    if (e != a) {
        valid = 0;
    }

    allTests++;
    if (!valid) {
        printf("FAIL %f\n", a);
    }
    else {
        printf("pass\n");
        passedTests++;
    }
}

int main() {

    // test defineVec3
    {
    printf("Testing vv\n");
    Vec3 e;
    e.x = 1.;
    e.y = 1.;
    e.z = 1.;
    Vec3 a = vv(1.,1.,1.);
    testVec3(&e, &a);
    }

    // test defineTriangle
    {
        printf("Testing tt\n");
        Triangle e;
        e.p[0].x = 1.;
        e.p[0].y = 1.;
        e.p[0].z = 1.;
        e.p[1].x = 1.;
        e.p[1].y = 1.;
        e.p[1].z = 1.;
        e.p[2].x = 1.;
        e.p[2].y = 1.;
        e.p[2].z = 1.;
        Triangle a = tt(1.,1.,1.,1.,1.,1.,1.,1.,1.);
        testTriangle(&e, &a);
    }

    // test addVec
    {
    printf("Testing addVec\n");
    Vec3 v1 = vv(1., 2., 1.5);
    Vec3 v2 = vv(2., 1., 1.5);
    Vec3 e = vv(3., 3., 3.);
    Vec3 a;
    addVec(&v1, &v2, &a);

    testVec3(&e, &a);
    }

    // test subVec
    {
    printf("Testing subVec\n");
    Vec3 v1 = vv(1., 2., 1.5);
    Vec3 v2 = vv(2., 1., 1.5);
    Vec3 e = vv(-1., 1., 0.);
    Vec3 a;
    subVec(&v1, &v2, &a);
    testVec3(&e, &a);
    }

    // test addTriangle
    {
    printf("Testing addTriangle\n");
    Triangle t1 = {
        .p = {
            vv(1., 2., 1.5),
            vv(2., 1., 1.5),
            vv(1., 1., 1.),
        }
    };
    Triangle t2 = {
        .p = {
            vv(1., 2., 1.5),
            vv(2., 1., 1.5),
            vv(1., 1., 1.),
        }
    };
    Triangle expected = {
        .p = {
            vv(2., 4., 3.),
            vv(4., 2., 3.),
            vv(2., 2., 2.),
        }
    };
    Triangle a;
    addTriangle(&t1, &t2, &a);
    testTriangle(&expected, &a);
    }

    // test subTriangle
    {
    printf("Testing subTriangle\n");
    Triangle t1 = {
        .p = {
            vv(2., 2.5, 1.5),
            vv(2., 1., 1.5),
            vv(2., 1., 1.),
        }
    };
    Triangle t2 = {
        .p = {
            vv(1., 2., 2.),
            vv(4., 1., 2.),
            vv(1., 1., 0.),
        }
    };
    Triangle expected = {
        .p = {
            vv(1., 0.5, -0.5),
            vv(-2., 0., -0.5),
            vv(1., 0., 1.),
        }
    };
    Triangle a;
    subTriangle(&t1, &t2, &a);
    testTriangle(&expected, &a);
    }

    //Test translateTrianglePos
    {
    printf("Testing translateTrianglePos\n");
    Triangle t1 = {
        .p = {
            vv(1., 2., 1.5),
            vv(2., 1., 1.5),
            vv(1., 1., 1.),
        }
    };
    Vec3 v1 = vv(1., 2., 1.5);
    Triangle expected = {
        .p = {
            vv(2., 4., 3.),
            vv(3., 3., 3.),
            vv(2., 3., 2.5),
        }
    };
    Triangle a;
    translateTrianglePos(&t1, &v1, &a);
    testTriangle(&expected, &a);
    }

    // Test multiplyMatrixVector
    if (0) {
    printf("Testing multiplyMatrixVector\n");
    Mat4 m1 = {
        .m = {
            1., 2., 3.,
            4., 5., 6.,
            7., 8., 9.,
        }
    };
    Vec3 v1 = vv(1., 2., 3.);
    Vec3 expected = vv(14., 32., 50.);
    Vec3 a;
    multiplyMatrixVector(&m1, &v1, &a);
    testVec3(&expected, &a);
    }

    // Test multiplyMatrixTriangle
    if (0) {
    printf("Testing multiplyMatrixTriangle\n");
    Mat4 m1 = {
        .m = {
            1., 2., 3.,
            4., 5., 6.,
            7., 8., 9.,
        }
    };
    Triangle t1 = {
        .p = {
            vv(1., 2., 3.),
            vv(4., 5., 6.),
            vv(7., 8., 9.),
        }
    };
    Triangle expected = {
        .p = {
            vv(14., 32., 50.),
            vv(32., 77., 122.),
            vv(50., 122., 194.),
        }
    };
    Triangle a;
    multiplyMatrixTriangle(&m1, &t1, &a);
    testTriangle(&expected, &a);
    }
    
    // Test normalizeVector
    {
    printf("Testing normalizeVector\n");
    Vec3 v1 = vv(1., 2., 3.);
    Vec3 expected = vv(0.2672612419124244, 0.5345224838248488, 0.8017837257372732);
    Vec3 a;
    normalizeVector(&v1);
    a = v1;
    testVec3(&expected, &a);
    }

    

    printf("\nDone.\n");
    printf("Passed %d/%d tests.\n", passedTests, allTests);

    





    

    return 0;
}


//