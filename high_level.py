import ctypes
import os
import pathlib
import time
from pynput.keyboard import Listener
from pynput import keyboard
from threading import Thread


# compile the c lib: gcc -o libc.so -shared -fPIC -O2 libc.c



def include(fileWithoutExtension):
    os.system("gcc -o "+fileWithoutExtension+".so -shared -fPIC -O2 "+fileWithoutExtension+".c")





include("main");



if __name__ == "__main__":
    # Load the shared library into ctypes
    libname = pathlib.Path().absolute() / "main.so"
    c_lib = ctypes.CDLL(libname)
    c_lib.keyDown.restype = None
    c_lib.keyUp.restype = None
    c_lib.keyIsPressed.restype = ctypes.c_int
    c_lib.getKeyArray.restype = None
    c_lib.keyDown.argtypes = [ctypes.c_char]
    c_lib.keyUp.argtypes = [ctypes.c_char]
    c_lib.keyIsPressed.argtypes = [ctypes.c_char]
    c_lib.getKeyArray.argtypes = [ctypes.c_char_p]
    c_lib.movePlayer.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]

    c_lib.run3D.restype = None
    c_lib.setup.restype = None
    c_lib.movePlayer.restype = None



#x, y = 6, 2.3
#answer = c_lib.cmult(x, ctypes.c_float(y))


def on_press(key):
    try:
        #print("Down: "+str(key.char))
        kk = ctypes.c_char(key.char.encode())
        c_lib.keyDown(kk)
    except AttributeError:
        return
        #print("Down: "+str(key))

def on_release(key):
    pass
    try:
        #print("Up: "+str(key.char))
        c_lib.keyUp(ctypes.c_char(key.char.encode()))
    except AttributeError:
        return
        #print("Up: "+str(key))

def setup():
    print("Running")
    c_lib.setup()

def main_loop():
    while (True):
        # convert c_char array to python array
        c_keyArr_out = (ctypes.c_char * 10)()
        c_lib.getKeyArray(c_keyArr_out)

        
        if (c_lib.keyIsPressed(ctypes.c_char("w".encode())) == 1):
            c_lib.movePlayer(ctypes.c_double(0.0), ctypes.c_double(0), ctypes.c_double(-0.1))

        if (c_lib.keyIsPressed(ctypes.c_char("a".encode())) == 1):
            c_lib.movePlayer(ctypes.c_double(0.1), ctypes.c_double(0), ctypes.c_double(0.0))

        if (c_lib.keyIsPressed(ctypes.c_char("s".encode())) == 1):
            c_lib.movePlayer(ctypes.c_double(0.0), ctypes.c_double(0), ctypes.c_double(0.1))

        if (c_lib.keyIsPressed(ctypes.c_char("d".encode())) == 1):
            c_lib.movePlayer(ctypes.c_double(-0.1), ctypes.c_double(0), ctypes.c_double(0.0))

        # for i in range(10):
        #     if c_keyArr_out[i] == 0:
        #         print("   ", end="")
        #     else:
        #         print((c_keyArr_out[i]).decode("utf-8") +" ", end="")
        # print("")
        c_lib.run3D()
        time.sleep(33/1000)

setup()

t = Thread(target=main_loop, args=[])
t.start()


with Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()

