TESTS_SRCS = \
	test/main.c

BUILD_DIR = bin

INCLUDES = -I. \
		   -Itest \
		   -Itest/lib \
		   -Itest/include

TEST_OBJS = $(TESTS_SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/tests

all: $(BUILD_DIR) $(TARGET)
	@echo "Building $(TESTS_SRCS)"

test: all
	./$(TARGET)

$(TARGET): $(TEST_OBJS)
	gcc $(BUILD_DIR)/test/*.o -o $@

$(BUILD_DIR)/%.o: %.c
	gcc -c $< $(INCLUDES) -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/test

clean:
	rm -rf *.o $(TARGET) $(BUILD_DIR)
