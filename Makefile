CC := g++
TARGET := gameboy

C_FILES := $(wildcard src/*.cpp) $(wildcard src/gbc/*.cpp)
H_FILES := $(wildcard src/*.hpp)

$(TARGET): $(C_FILES) $(H_FILES)
	$(CC) -Wall $(C_FILES) -o $@

clean:
	rm $(TARGET)

