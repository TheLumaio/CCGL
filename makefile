SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, obj/%.o, $(SRC))
CFLAGS = -std=c++14 -LC:/Bin/mingw/lib -IC:/Bin/mingw/include -w -fpermissive
OUT = build/StandigeEngine.exe

LDFLAGS = 
LIBS = \
	-lglew.dll \
	-lglfw3	\
	-lSOIL \
	-lopengl32\
	-lwinmm \
	-lgdi32 \
	-lfreetype \
	-lws2_32 \
	-lSDL2_net \
	-lpthread \
	-lassimp.dll
CC = g++

$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBS) -o $(OUT)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
