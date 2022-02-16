all: ball-physics ball-physics-windows

CFLAGS = -std=c++17
LDFLAGS = -lglfw -lGLU -lGL -ldl -lpthread -lX11 -lXrandr
WDFLAGS = -lglfw3dll -lopengl32 -lwinpthread -static-libstdc++ -static-libgcc
SRC = src/

ball-physics: $(SRC)main.cpp
	g++ $(CFLAGS) -o 2D_Ball_Physics $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad -I include $(LDFLAGS)
	mkdir build
	mv 2D_Ball_Physics build

install:
	echo "Installing is not supported"

run:
	./build/2D_Ball_Physics

ball-physics-windows: $(SRC)main.cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) -o 2D_Ball_Physics.exe $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad_windows -I include $(WDFLAGS)
	mv 2D_Ball_Physics.exe build

run-windows:
	wine build/2D_Ball_Physics.exe

clean:
	rm -r build
