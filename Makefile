CC := g++
TARGET := gameboy
FLAGS := -Wall -g `sdl2-config --cflags --libs`

C_FILES := $(wildcard src/*.cpp) $(wildcard src/gbc/*.cpp) $(wildcard src/dbg/*.cpp) $(wildcard src/sdl/*.cpp) $(wildcard src/gbc/mappers/*.cpp)
H_FILES := $(wildcard src/*.hpp) $(wildcard src/gbc/*.hpp) $(wildcard src/dbg/*.hpp) $(wildcard src/sdl/*.hpp) $(wildcard src/gbc/mappers/*.hpp)

$(TARGET): $(C_FILES) $(H_FILES)
	$(CC) -D DEBUG $(FLAGS) $(C_FILES) -o $@ $(FLAGS)

clean:
	rm $(TARGET)

doc: $(C_FILES) $(H_FILES)
	doxygen doxygen.conf

run: $(TARGET)
	./$(TARGET)
