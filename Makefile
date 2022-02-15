CFLAGS = -std=c++17
LDFLAGS = -lglfw -lGLU -lGL -ldl -lpthread -lX11 -lXrandr
WDFLAGS = -lglfw3dll -lopengl32 -lwinpthread -static-libstdc++ -static-libgcc
SRC = src/

build: $(SRC)main.cpp
	g++ $(CFLAGS) -o OpenGL $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad -I include $(LDFLAGS)

install: build
	cp OpenGL build

run: install
	./build/OpenGL

build-windows: $(SRC)main.cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) -o OpenGL.exe $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad_windows -I include/ $(WDFLAGS)

run-windows: build-windows
	wine OpenGL.exe

clean:
	rm -r build
	mkdir build
