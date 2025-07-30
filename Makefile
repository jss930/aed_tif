# Configuración del proyecto
NAME = grafos
CC = g++
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Flags del compilador
CFLAGS = -std=c++17 -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS = -lSDL3

# Detectar si estamos en MSYS2/Windows
ifeq ($(OS),Windows_NT)
    # Rutas para MSYS2
    SDL_INCLUDE = -I/ucrt64/include -I/ucrt64/include/SDL3
    SDL_LIB = -L/ucrt64/lib -lSDL3
    TARGET = $(BUILD_DIR)/$(NAME).exe
else
    # Para Linux
    SDL_INCLUDE = $(shell pkg-config --cflags sdl3)
    SDL_LIB = $(shell pkg-config --libs sdl3)
    TARGET = $(BUILD_DIR)/$(NAME)
endif

# Archivos fuente
MAIN_SRC = main.cpp
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
ALL_SRC = $(MAIN_SRC) $(SRC_FILES)

# Archivos objeto
MAIN_OBJ = $(BUILD_DIR)/main.o
SRC_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
ALL_OBJ = $(MAIN_OBJ) $(SRC_OBJ)

# Regla principal
all: $(TARGET)

# Crear el ejecutable
$(TARGET): $(ALL_OBJ) | $(BUILD_DIR)
	@echo "Enlazando: $@"
	$(CC) $(ALL_OBJ) $(SDL_LIB) -o $@

# Compilar main.cpp
$(BUILD_DIR)/main.o: $(MAIN_SRC) | $(BUILD_DIR)
	@echo "Compilando: $<"
	$(CC) $(CFLAGS) $(SDL_INCLUDE) -c $< -o $@

# Compilar archivos de src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compilando: $<"
	$(CC) $(CFLAGS) $(SDL_INCLUDE) -c $< -o $@

# Crear directorio build
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Limpiar
clean:
	@echo "Limpiando..."
	@rm -rf $(BUILD_DIR)

# Ejecutar
run: $(TARGET)
	@echo "Ejecutando..."
	./$(TARGET)

# Debug - mostrar variables
debug:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "SDL_INCLUDE: $(SDL_INCLUDE)"
	@echo "SDL_LIB: $(SDL_LIB)"
	@echo "ALL_SRC: $(ALL_SRC)"
	@echo "ALL_OBJ: $(ALL_OBJ)"
	@echo "TARGET: $(TARGET)"

# Verificar instalación de SDL
check-sdl:
	@echo "Verificando SDL3..."
	@if [ -f "/ucrt64/include/SDL3/SDL.h" ]; then \
		echo "✓ SDL3 headers encontrados"; \
	else \
		echo "✗ SDL3 headers NO encontrados"; \
		echo "Instala con: pacman -S mingw-w64-ucrt-x86_64-SDL3"; \
	fi
	@if [ -f "/ucrt64/lib/libSDL3.a" ] || [ -f "/ucrt64/lib/libSDL3.dll.a" ]; then \
		echo "✓ SDL3 library encontrada"; \
	else \
		echo "✗ SDL3 library NO encontrada"; \
	fi

.PHONY: all clean run debug check-sdl