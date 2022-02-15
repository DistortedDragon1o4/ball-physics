CFLAGS = -std=c++17
LDFLAGS = -lglfw -lGLU -lGL -ldl -lpthread -lX11 -lXrandr
WDFLAGS = -lglfw3dll -lopengl32 -lwinpthread -static-libstdc++ -static-libgcc
SRC = src/

build: $(SRC)main.cpp
	g++ $(CFLAGS) -o 2D\ Ball\ Physics $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad -I include $(LDFLAGS)

install: clean build
	mkdir build
	mv 2D\ Ball\ Physics build

run: install
	./build/2D\ Ball\ Physics

build-windows: $(SRC)main.cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) -o 2D\ Ball\ Physics.exe $(SRC)main.cpp $(SRC)physics.cpp $(SRC)glad/glad_windows -I include $(WDFLAGS)

run-windows: build-windows
	wine 2D\ Ball\ Physics.exe

clean:
	rm -r build
