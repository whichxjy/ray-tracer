CC = g++ -std=c++17
INC = include
SRC = src
BUILD = build
INCLUDE = -I ./$(INC)

TARGET = ray

OBJECTS = $(BUILD)/main.o $(BUILD)/color.o $(BUILD)/hittable.o $(BUILD)/sphere.o

$(TARGET): $(OBJECTS)
	@$(CC) -o $@ $^ $(INCLUDE)

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(BUILD)
	@$(CC) -o $@ -c $^ $(INCLUDE)

run: $(TARGET)
	@./$(TARGET) > result.ppm
	@convert result.ppm result.png
	@rm -f result.ppm

clean:
	@rm -rf $(BUILD) $(TARGET) result.ppm result.png