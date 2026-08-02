CXX = g++
CFLAGS = -std=c++17 -O2 -Iinclude -Wall
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11  -lXrandr -lXi

GLSLC = glslc

SRC = src/main.cpp src/app.cpp src/init.cpp utils/debug.cpp src/device.cpp src/swapchain.cpp src/pipeline.cpp src/commands.cpp
OBJ = $(SRC:.cpp=.o)

VERT_SOURCES = $(wildcard shaders/*.vert)
FRAG_SOURCES = $(wildcard shaders/*.frag)
SPV_OBJS = shaders/vert.spv shaders/frag.spv

TARGET = VulkanTest

all: shaders $(TARGET)

shaders: $(SPV_OBJS)

shaders/vert.spv: shaders/shader.vert
	$(GLSLC) $< -o $@

# Mappatura esplicita: da shader.frag produce frag.spv
shaders/frag.spv: shaders/shader.frag
	$(GLSLC) $< -o $@


$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean run shaders
	
clean:
	rm -vf $(OBJ) $(TARGET) shaders/*.spv

run:$(TARGET) shaders
	./VulkanTest 
