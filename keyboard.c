// cmult.c

#define LENGTH 10

int keyIndex;
char keysDown[LENGTH];

void keyDown(char key) {
    // ignore the key if the array is full
    if (keyIndex >= LENGTH) {
        return;
    }
    char* freeSlot = 0;
    for (int i = 0; i < LENGTH; i++) {
        if (key == keysDown[i]) {
            return;
        }
        if (keysDown[i] == 0) {
            freeSlot = &keysDown[i];
        }
    }

    if (freeSlot != 0) {
        *freeSlot = key;
        keyIndex++;
    }
    return;
}

void keyUp(char key) {
    for (int i = 0; i < LENGTH; i++) {
        if (key == keysDown[i]) {
            keysDown[i] = 0;
            keyIndex--;
            return;
        }
    }
}

int keyIsPressed(char key) {
    for (int i = 0; i < LENGTH; i++) {
        if (key == keysDown[i]) {
            return 1;
        }
    }
    return 0;
}

void getKeyArray(char* outArr) {

    for (int i = 0; i < LENGTH; i++) {
        outArr[i] = keysDown[i];
    }
}
