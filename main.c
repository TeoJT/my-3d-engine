#include "graphics-basics.c"
#include "basic_math.c"
#include "linkedlist.c"
#include "keyboard.c"


#define TBUFFER_SIZE 8196*10



typedef struct struct_mesh {
    Triangle* tris;
    int length;
} Mesh;


Triangle* tBuff;
Triangle* tBuff_index;



Triangle* tappend(Triangle obj) {
    *tBuff_index = obj;
    return tBuff_index++;
}

Triangle* nextSpace() {
    return tBuff_index;
}





double nextDouble(char* buff) {
    int chrCount = 0;

    // First, position ourselves to the next number so we can begin reading it.
    while (!((*buff >= '0' && *buff <= '9') || *buff == '.' || *buff == '-')) {
        buff++;

        // In the event the line's invalid exit after a while.
        chrCount++;
        if (chrCount > 1024) {
            printf("Could not find double.\n");
            return -999.;
        }
    }

    // Here we are.
    char* startPoint = buff;

    // Now let's read the number until there's no more number to read.
    while (*buff != ' ' && *buff != '\n' && *buff != '\0') {
        buff++;

        // In the event the line's invalid exit after a while.
        chrCount++;
        if (chrCount > 1024) {
            printf("Could not read double.\n");
            return -999.;
        }
    }
    char* endPoint = buff;
    double result = strtod(startPoint, &endPoint);

    // Clear up what we just read so that we don't read it again.
    for (char* i = startPoint; i < endPoint; i++) {
        *i = ' ';
    }

    // Convert string to char.
    return result;
}


double nextInt(char* buff) {
    int chrCount = 0;

    // First, position ourselves to the next number so we can begin reading it.
    while (!((*buff >= '0' && *buff <= '9') || *buff == '-')) {
        buff++;

        // In the event the line's invalid exit after a while.
        chrCount++;
        if (chrCount > 1024) {
            printf("Could not find double.\n");
            return -999.;
        }
    }

    // Here we are.
    char* startPoint = buff;

    // Now let's read the number until there's no more number to read.
    while (*buff != ' ' && *buff != '\n' && *buff != '\0' && *buff != '/') {
        buff++;

        // In the event the line's invalid exit after a while.
        chrCount++;
        if (chrCount > 1024) {
            printf("Could not read double.\n");
            return -999.;
        }
    }
    char* endPoint = buff;
    int result = atoi(startPoint);

    // If there's a tuple, clear up the rest of that junk, we don't want that.
    if (*buff == '/') {
        buff++;
        while (*buff != ' ' && *buff != '\n' && *buff != '\0') {
            buff++;
        }
    }

    while (*endPoint != ' ' && *endPoint != '\n') {
        *endPoint = ' ';
        endPoint++;
    }

    // Clear up what we just read so that we don't read it again.
    for (char* i = startPoint; i < endPoint; i++) {
        *i = ' ';
    }

    // Convert string to char.
    return result;
}



Mesh* loadObj(char* filename) {
    // Our buffer for reading each line
    // let's make the dirty assumption that we only need 1024 chars long.
    char buff[1024];

    // First, open just to check.
    FILE* chckrrf = fopen(filename, "r");
    if (chckrrf == NULL) {
        printf("Error: File not found.\n");
        return;
    }

    // Count how many verticies we need so we can allocate memory.
    int verticeCount = 0;
    while (fgets(buff, 1024, chckrrf) != NULL) {
        if (buff[0] == 'v') {
            verticeCount++;
        }
    }

    // Done checking.
    fclose(chckrrf);

    // Now time to actually read the file.
    FILE* fp = fopen(filename, "r");

    // Allocate memory for the verticies.
    Vec3* vpool = malloc(sizeof(Vec3) * verticeCount);
    Vec3* vpoolOrigin = vpool; 

    // Create new mesh.
    Mesh* newMesh = malloc(sizeof(Mesh));
    newMesh->tris = nextSpace();

    int numTriangles = 0;

    // Now read all the data from the obj file
    while (fgets(buff, 1024, fp) != NULL) {
        
        // Vertex
        // Usually in the form e.g.
        // v 1.0000 -1.0000, 4.0000
        if (buff[0] == 'v') {
            vpool->x = nextDouble(&buff);
            vpool->y = nextDouble(&buff);
            vpool->z = nextDouble(&buff);
            //printf("%f %f %f\n", vpool->x, vpool->y, vpool->z);
            vpool++;
        }

        // Face
        // Usually in the form e.g.
        // f 1 2 3
        // Sometimes will be in a tuple:
        // f 1/2/3 2/3/4 3/4/5
        // If it's in a tuple, we only care about the first digit in the tuple.
        // The nextInt() function will deal with this and ignore the other values in the tuple.
        if (buff[0] == 'f') {
            int v1 = nextInt(&buff) - 1;
            int v2 = nextInt(&buff) - 1;
            int v3 = nextInt(&buff) - 1;
            //printf("%d %d %d\n", v1,v2,v3);

            Triangle t;
            t.p[0] = *(vpoolOrigin+v1);
            t.p[1] = *(vpoolOrigin+v2);
            t.p[2] = *(vpoolOrigin+v3);
            tappend(t);

            numTriangles++;
        }

    }
    
    newMesh->length = numTriangles;

    // Done reading.
    fclose(fp);
    free(vpoolOrigin);

    return newMesh;
}


// Function to insert a triangle into a linked list in sorted order by z value.
void insertTriangle(Triangle* t, LinkedList *list) {
    // If the list is empty, just add it.
    if (list->headNode == NULL) {
        printf("Node: Empty list\n");
        Node newNode;
        list->headNode = &newNode;
        list->size++;
        list->headNode->object = t;
        list->headNode->next = NULL;
        return;
    }

    // If the list is not empty, we need to find the right place to insert it.
    Node* current = list->headNode;
    Node* previous = NULL;

    // Iterate through the list until we find the right place to insert it.
    while (current != NULL) {
        Triangle* ct = current->object;
        if ((t->p[0].z + t->p[1].z + t->p[2].z)/3.0 > (ct->p[0].z + ct->p[1].z + ct->p[2].z)/3.0) {
            break;
        }
        previous = current;
        current = current->next;
    }


    // If we're at the end of the list, just add it.
    if (current == NULL) {
        printf("Node: End of list\n");
        Node newNode;
        previous->next = &newNode;
        list->size++;
        previous->next->object = t;
        previous->next->next = NULL;
        return;
    }

    // If we're not at the end of the list, we need to insert it.
    Node newNode;
    list->size++;
    newNode.object = t;
    newNode.next = current;
    if (previous == NULL) {
        list->headNode = &newNode;
    } else {
        previous->next = &newNode;
    }
    printf("Node: Inserted\n");
}



Mesh* createCube() {
    Mesh* cube = malloc(12 * sizeof(Triangle) + sizeof(Mesh));
    cube->length = 12;
    cube->tris = tappend(tt(0.0, 0.0, 0.0,    0.0, 1.0, 0.0,    1.0, 1.0, 0.0 ));
                tappend(tt(0.0, 0.0, 0.0,    1.0, 1.0, 0.0,    1.0, 0.0, 0.0 ));

                    // EAST                                                      
                tappend(tt(1.0, 0.0, 0.0,    1.0, 1.0, 0.0,    1.0, 1.0, 1.0 ));
                tappend(tt(1.0, 0.0, 0.0,    1.0, 1.0, 1.0,    1.0, 0.0, 1.0 ));

                    // NORTH                                                     
                tappend(tt(1.0, 0.0, 1.0,    1.0, 1.0, 1.0,    0.0, 1.0, 1.0));
                tappend(tt(1.0, 0.0, 1.0,    0.0, 1.0, 1.0,    0.0, 0.0, 1.0));

                    // WEST                                                      
                tappend(tt(0.0, 0.0, 1.0,    0.0, 1.0, 1.0,    0.0, 1.0, 0.0));
                tappend(tt(0.0, 0.0, 1.0,    0.0, 1.0, 0.0,    0.0, 0.0, 0.0));

                    // TOP                                                       
                tappend(tt(0.0, 1.0, 0.0,    0.0, 1.0, 1.0,    1.0, 1.0, 1.0));
                tappend(tt(0.0, 1.0, 0.0,    1.0, 1.0, 1.0,    1.0, 1.0, 0.0));

                    // BOTTOM                                                    
                tappend(tt(1.0, 0.0, 1.0,    0.0, 0.0, 1.0,    0.0, 0.0, 0.0));
                tappend(tt(1.0, 0.0, 1.0,    0.0, 0.0, 0.0,    1.0, 0.0, 0.0));

    return cube;
}






double tick = 0;


Mat4 matProj;
Vec3 vCamera;
LinkedList meshesLinkedList;
LinkedList* meshes;

// Projection matrix
double fNear, fFar, fFov, fAspect, fFovRad;
double fTheta = 0.;
int trisRendered = 0;
LinkedList* trianglesToRaster;
Vec3 playerPos;


void movePlayer(double x, double y, double z) {
    playerPos.x += x;
    playerPos.y += y;
    playerPos.z += z;
}


void setup() {
    terminalDisplayInit();

    // while (1) {
    //     fill(0,0,0);
    //     line(sin(tick)*(double)(t_d.width/2)+(double)(t_d.width/2), cos(tick)*(double)(t_d.height/2)+(double)(t_d.height/2), sin(tick*1.5+0.7)*(double)(t_d.width/2)+(double)(t_d.width/2), sin(tick)*(double)(t_d.height/2)+(double)(t_d.height/2));
    //     tick += 0.1;
    //     terminalDisplayRender();
    //     usleep(100000);
    // }

    tBuff = malloc(TBUFFER_SIZE);
    setZbuffer(malloc(t_d.width * t_d.height * sizeof(double)));
    tBuff_index = tBuff;

    meshes = &meshesLinkedList;
    meshes->size = 0;
    

    //Mesh* starMesh = loadObj("./models/star.obj");
    //addAtTail(meshes, starMesh);

    //Mesh* toadMesh = loadObj("./models/toad.obj");
    //addAtTail(meshes, toadMesh);
    //Mesh* bobomb = loadObj("./models/bobombbattlefield.obj");
    //addAtTail(meshes, bobomb);
    Mesh* cube = createCube();
    addAtTail(meshes, cube);

    

    // Projection matrix
    fNear = 0.1;
    fFar = 1000.0;
    fFov = 90.0;
    fAspect = (double)t_d.height / (double)t_d.width;
    fFovRad = 1.0 / tan(fFov * 0.5 / 180. * 3.142857142857143);

    matProj.m[0][0] = fAspect * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0;
    matProj.m[3][3] = 0.0;
    fTheta = 0.;
    trisRendered = 0;

    //Initial position of player in 3d space.
    playerPos = defineVec3(0., 0., 3.);


    trianglesToRaster = malloc(1024*16);
}

void run3D() {
    
        fill(0, 0, 0);
        clearZBuffer();
        trianglesToRaster->size = 0;
        trianglesToRaster->headNode = NULL;
        trianglesToRaster->tailNode = NULL;

        trisRendered = 0;


        Mat4 matRotZ, matRotX;
        fTheta += 0.05;

        // Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5);
		matRotX.m[1][2] = sinf(fTheta * 0.5);
		matRotX.m[2][1] = -sinf(fTheta * 0.5);
		matRotX.m[2][2] = cosf(fTheta * 0.5);
		matRotX.m[3][3] = 1;

        
        // debugging
        double tmp[10];
        int imp = 0;

        Node* n = meshes->headNode;
        while (n != NULL) {
            Mesh* mesh = n->object;
            n = n->next;

            Triangle* len = mesh->tris+mesh->length;
            for (Triangle* tri = mesh->tris; tri < len; tri++) {

                Triangle triProjected; 
                Triangle triTranslated;
                Triangle triRotatedZ;
                Triangle triRotatedZX;

                // Rotate in z axis

                multiplyMatrixTriangle(tri, &triRotatedZ, &matRotZ);

                // Rotate in X-Axis

                multiplyMatrixTriangle(&triRotatedZ, &triRotatedZX, &matRotX);
                
                // Offset into the screen
                // also copy its data
                triTranslated = triRotatedZX;
                
                Vec3* transltd = &playerPos;
                translateTrianglePos(&triRotatedZX, transltd, &triTranslated);


                // Surface normals
                Vec3 normal, line1, line2;
                subVec(&triTranslated.p[1], &triTranslated.p[0], &line1);
                subVec(&triTranslated.p[2], &triTranslated.p[0], &line2);

                // I don't know what this is called. Dot product or somethin'?
                normal.x = line1.y * line2.z - line1.z * line2.y;
                normal.y = line1.z * line2.x - line1.x * line2.z;
                normal.z = line1.x * line2.y - line1.y * line2.x;

                // Normalize the surface normals
                normalizeVector(&normal);

                // Dot product
                if (normal.x * (triTranslated.p[0].x - vCamera.x) + 
                    normal.y * (triTranslated.p[0].y - vCamera.y) +
                    normal.z * (triTranslated.p[0].z - vCamera.z) < 0.0 ) {
                        
                    
                    // basic face lighting
                    Vec3 light_direction = defineVec3(0.0, 0.0, -3.0);
                    //printf("%f, %f, %f\n", light_direction.x, light_direction.y, light_direction.z);
                    normalizeVector(&light_direction);
                    //printf("%f, %f, %f\n", normal.x, normal.y, normal.z);
                    double dp = (normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z);
                    int c = (int)(255. * (dp*2. + 2.) / 2);
                    //printf("%f\n", dp);

                    // Project triangles 3D -> 2D.
                    multiplyMatrixTriangle(&triTranslated, &triProjected, &matProj);

                    // Scale into view
                    triProjected.p[0].x += 1.0; triProjected.p[0].y += 1.0;
                    triProjected.p[1].x += 1.0; triProjected.p[1].y += 1.0;
                    triProjected.p[2].x += 1.0;
                    triProjected.p[2].y += 1.0;
                    triProjected.p[0].x *= 0.5 * (double)t_d.width;
                    triProjected.p[0].y *= 0.5 * (double)t_d.height;
                    triProjected.p[1].x *= 0.5 * (double)t_d.width;
                    triProjected.p[1].y *= 0.5 * (double)t_d.height;
                    triProjected.p[2].x *= 0.5 * (double)t_d.width;
                    triProjected.p[2].y *= 0.5 * (double)t_d.height;

                    //insertTriangle(&triProjected, trianglesToRaster);
                    

                    // Wireframe
                    // drawTriangle(
                    //     triProjected.p[0].x, triProjected.p[0].y, 
                    //     triProjected.p[1].x, triProjected.p[1].y, 
                    //     triProjected.p[2].x, triProjected.p[2].y,
                    //     0x00000000
                    // );
                    fillTriangle(
                        triProjected.p[0].x, triProjected.p[0].y, 
                        triProjected.p[1].x, triProjected.p[1].y, 
                        triProjected.p[2].x, triProjected.p[2].y,
                        colour(c,c,c)
                    );
                    trisRendered++;

                }
                // pixel(triProjected.p[0].x, triProjected.p[0].y, colour(255,255,255));
                // pixel(triProjected.p[1].x, triProjected.p[1].y, colour(255,255,255));
                // pixel(triProjected.p[2].x, triProjected.p[2].y, colour(255,255,255));


            }
        }

        Node* current = trianglesToRaster->headNode;
        for (int i = 0; i < trianglesToRaster->size-1; i++) {
            // Triangle* tri = current->object;
            // if (tri != NULL) {
            //     printf("hereigo:\n");
            //     printf("%d\n", tri);
            //     fillTriangle(
            //         tri->p[0].x, tri->p[0].y, 
            //         tri->p[1].x, tri->p[1].y, 
            //         tri->p[2].x, tri->p[2].y,
            //         0xFFFFFFFF
            //     );
            //     drawTriangle(
            //         tri->p[0].x, tri->p[0].y, 
            //         tri->p[1].x, tri->p[1].y, 
            //         tri->p[2].x, tri->p[2].y,
            //         0x00000000
            //     );
            //     printf("Drawn\n");
            // }
            //current = current->next;
            //printf("hereigo:\n");
        }
        //printf("Done\n");

        //line(0, 0, 200, 800);
        //line(37, 0, t_d.width, t_d.height+100);
        terminalDisplayRender();
        t_d.dbMssg = 1;
        printf("\e[m tris  %d\n", trisRendered);

        //usleep(33000);

}
