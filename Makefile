CXX = g++
CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11  -lXrandr #-lXi

SRC = main.cpp fun.cpp init.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = VulkanTest

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean run
	
clean:
	rm -vf $(OBJ) $(TARGET)
run:$(TARGET)
	./VulkanTest 
