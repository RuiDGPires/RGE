CC := g++
TARGET := gameboy
FLAGS := -Wall -g `sdl2-config --cflags --libs`

C_FILES := $(wildcard src/*.cpp) $(wildcard src/gbc/*.cpp) $(wildcard src/dbg/*.cpp) $(wildcard src/sdl/*.cpp)
H_FILES := $(wildcard src/*.hpp)

$(TARGET): $(C_FILES) $(H_FILES)
	$(CC) -D DEBUG $(FLAGS) $(C_FILES) -o $@ $(FLAGS)

main: $(C_FILES) $(H_FILES)
	$(CC) -D MAIN $(C_FILES) -o $(TARGET) $(FLAGS)
clean:
	rm $(TARGET)

