
# Nombre del ejecutable
NAME = grafos

# Compilador y flags
#! compilador a usar
CC = g++

CXX = g++
CFLAGS = -Iinclude -MMD -DSDL_MAIN_USE_CALLBACKS# Puedes agregar -Wall -Wextra para más advertencias

# Flags de pkg-config para SDL3
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl3)

# Extensiones según el compilador
EXTENSION = .c
ifeq ($(CC),$(CXX))
    EXTENSION = .cpp
endif

# Directorios
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TARGET = $(BUILD_DIR)/$(NAME)

# Archivos fuente, objetos y dependencias
SRC = $(wildcard $(SRC_DIR)/*$(EXTENSION))
OBJ = $(patsubst $(SRC_DIR)/%$(EXTENSION), $(BUILD_DIR)/%.o, $(SRC))
DEPS = $(OBJ:.o=.d)

# Regla principal
all: $(TARGET)

# @echo "ejecutando"
# TARGET)
# Crear ejecutable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(SDL_FLAGS) -o $@

# @if [ -f ./$(TARGET) ]; then mv ./$(TARGET) ./$(BUILD_DIR)/estable_$(NAME); fi
# Compilar cada archivo fuente a objeto, creando dependencias
$(BUILD_DIR)/%.o: $(SRC_DIR)/%$(EXTENSION)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SDL_FLAGS) -c $< -o $@ 

# Incluir dependencias si existen
-include $(DEPS)

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean
