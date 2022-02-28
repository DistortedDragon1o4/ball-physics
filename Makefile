all: clean move ball-physics ball-physics-windows

CFLAGS = -std=c++17
LDFLAGS = -lglfw -lGLU -lGL -ldl -lpthread -lX11 -lXrandr
WDFLAGS = -lglfw3dll -lopengl32 -lwinpthread -static-libstdc++ -static-libgcc
SRC = src/
BUILD = build/

ball-physics: $(SRC)main.cpp
	g++ $(CFLAGS) -o $(BUILD)2D_Ball_Physics $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad -I include -I shaders $(LDFLAGS)

move:
	mkdir build
	cp $(SRC)properties.json $(BUILD)properties.json

install:
	mkdir build

run:
	./build/2D_Ball_Physics

ball-physics-windows: $(SRC)main.cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) -o $(BUILD)2D_Ball_Physics.exe $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad_windows -I include -I shaders $(WDFLAGS)

run-windows:
	wine build/2D_Ball_Physics.exe

clean:
	rm -r build
