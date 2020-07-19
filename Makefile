CC = g++ -std=c++17
INC = include
SRC = src
BUILD = build
INCLUDE = -I ./$(INC)

TARGET = ray

OBJECTS = $(BUILD)/main.o $(BUILD)/color.o $(BUILD)/hittable.o $(BUILD)/sphere.o \
			$(BUILD)/material.o $(BUILD)/mesh.o $(BUILD)/box.o $(BUILD)/bvh.o

$(TARGET): $(OBJECTS)
	@$(CC) -o $@ $^ $(INCLUDE) -lassimp

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	@$(CC) -o $@ -c $^ $(INCLUDE)

run: $(TARGET)
	@./$(TARGET)

clean:
	@rm -rf $(BUILD) $(TARGET) result.png