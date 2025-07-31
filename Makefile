# Nombre del ejecutable
NAME = grafos

# Compilador y flags
CC = g++
CXX = g++
CFLAGS = -I$(INTERFACE_DIR) -MMD -DSDL_MAIN_USE_CALLBACKS# Puedes agregar -Wall -Wextra para más advertencias

# Flags de pkg-config para SDL3
SDL_FLAGS = $(shell pkg-config --cflags --libs sdl3)

# Extensiones según el compilador
EXTENSION = .c
ifeq ($(CC),$(CXX))
    EXTENSION = .cpp
endif

# Directorios
IMPLEMENTATION_DIR = implementation
BUILD_DIR = build
INTERFACE_DIR = interface
TARGET = $(BUILD_DIR)/$(NAME)

# Archivos fuente, objetos y dependencias
IMPLEMENTATION = $(wildcard $(IMPLEMENTATION_DIR)/*$(EXTENSION))
OBJ = $(patsubst $(IMPLEMENTATION_DIR)/%$(EXTENSION), $(BUILD_DIR)/%.o, $(IMPLEMENTATION))
DEPS = $(OBJ:.o=.d)

# Regla principal
all: $(TARGET)

# Crear ejecutable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(SDL_FLAGS) -o $@

# Compilar cada archivo fuente a objeto, creando dependencias
$(BUILD_DIR)/%.o: $(IMPLEMENTATION_DIR)/%$(EXTENSION)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SDL_FLAGS) -c $< -o $@

# Incluir dependencias si existen
-include $(DEPS)

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR)/*

# Limpiar todo incluyendo dependencias
distclean: clean
	rm -f $(BUILD_DIR)/*.d

.PHONY: all clean distclean