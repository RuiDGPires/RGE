CC := g++
TARGET := gameboy

C_FILES := $(wildcard src/*.cpp) $(wildcard src/gbc/*.cpp) $(wildcard src/dbg/*.cpp)
H_FILES := $(wildcard src/*.hpp)

$(TARGET): $(C_FILES) $(H_FILES)
	$(CC) -Wall $(C_FILES) -o $@ -g

main: $(C_FILES) $(H_FILES)
	$(CC) -D MAIN -Wall $(C_FILES) -o $(TARGET) -g
clean:
	rm $(TARGET)

