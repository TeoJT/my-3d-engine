#include <math.h>

typedef struct struct_vec3 {
    double x;
    double y;
    double z;
} Vec3;

typedef struct struct_triangle {
    Vec3 p[3];
} Triangle;

typedef struct struct_mat4 {
    double m[4][4];
} Mat4;


// Add 2 vetors
void addVec(Vec3* v1, Vec3* v2, Vec3* out) {
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->z = v1->z + v2->z;
}

void subVec(Vec3* v1, Vec3* v2, Vec3* out) {
    out->x = v1->x - v2->x;
    out->y = v1->y - v2->y;
    out->z = v1->z - v2->z;
}

void addTriangle(Triangle* t1, Triangle* t2, Triangle* out) {
    out->p[0].x = t1->p[0].x + t2->p[0].x;
    out->p[0].y = t1->p[0].y + t2->p[0].y;
    out->p[0].z = t1->p[0].z + t2->p[0].z;

    out->p[1].x = t1->p[1].x + t2->p[1].x;
    out->p[1].y = t1->p[1].y + t2->p[1].y;
    out->p[1].z = t1->p[1].z + t2->p[1].z;

    out->p[2].x = t1->p[2].x + t2->p[2].x;
    out->p[2].y = t1->p[2].y + t2->p[2].y;
    out->p[2].z = t1->p[2].z + t2->p[2].z;
}

void subTriangle(Triangle* t1, Triangle* t2, Triangle* out) {
    out->p[0].x = t1->p[0].x - t2->p[0].x;
    out->p[0].y = t1->p[0].y - t2->p[0].y;
    out->p[0].z = t1->p[0].z - t2->p[0].z;

    out->p[1].x = t1->p[1].x - t2->p[1].x;
    out->p[1].y = t1->p[1].y - t2->p[1].y;
    out->p[1].z = t1->p[1].z - t2->p[1].z;

    out->p[2].x = t1->p[2].x - t2->p[2].x;
    out->p[2].y = t1->p[2].y - t2->p[2].y;
    out->p[2].z = t1->p[2].z - t2->p[2].z;
}

void translateTrianglePos(Triangle* t1, Vec3* pos, Triangle* out) {
    out->p[0].x = t1->p[0].x + pos->x;
    out->p[0].y = t1->p[0].y + pos->y;
    out->p[0].z = t1->p[0].z + pos->z;

    out->p[1].x = t1->p[1].x + pos->x;
    out->p[1].y = t1->p[1].y + pos->y;
    out->p[1].z = t1->p[1].z + pos->z;

    out->p[2].x = t1->p[2].x + pos->x;
    out->p[2].y = t1->p[2].y + pos->y;
    out->p[2].z = t1->p[2].z + pos->z;
}


Triangle defineTriangle(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
    Triangle t;
    t.p[0].x = x1;
    t.p[0].y = y1;
    t.p[0].z = z1;

    t.p[1].x = x2;
    t.p[1].y = y2;
    t.p[1].z = z2;

    t.p[2].x = x3;
    t.p[2].y = y3;
    t.p[2].z = z3;

    return t;
}

Triangle tt(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
    return defineTriangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
}

Vec3 defineVec3(double x, double y, double z) {
    Vec3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

Vec3 vv(double x, double y, double z) {
    return defineVec3(x,y,z);
}

void multiplyMatrixVector(Vec3* i, Vec3* o, Mat4* m) {
    o->x    = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
    o->y    = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
    o->z    = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
    float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

    if (w != 0.0) {
        o->x /= w; 
        o->y /= w; 
        o->z /= w;
    }
}

void multiplyMatrixTriangle(Triangle* i, Triangle* o, Mat4* m) {
    multiplyMatrixVector(&i->p[0], &o->p[0], m);
    multiplyMatrixVector(&i->p[1], &o->p[1], m);
    multiplyMatrixVector(&i->p[2], &o->p[2], m);
}



void normalizeVector(Vec3* v) {
    double l = sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
    v->x /= l;
    v->y /= l;
    v->z /= l;
}

double dotProduct(Vec3* v1, Vec3* v2, Vec3* v3) {
    return   v1->x * (v2->x - v3->x) +
             v1->y * (v2->y - v3->y) +
             v1->z * (v2->z - v3->z);
} 



