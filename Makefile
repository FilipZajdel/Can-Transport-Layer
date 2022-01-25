DEPS = \
	CanTp.c \
	CanTp.h \
	CanTp_Types.h \
	test/main.c \
    test/test_CanTp_CancelReceive.c \
    test/test_CanTp_CancelTransmit.c \
    test/test_CanTp_ChangeParameter.c \
    test/test_CanTp_GetVersionInfo.c \
    test/test_CanTp_Init.c \
    test/test_CanTp_MainFunction.c \
    test/test_CanTp_ReadParameter.c \
    test/test_CanTp_RxIndication.c \
    test/test_CanTp_Shutdown.c \
    test/test_CanTp_Transmit.c \
    test/test_CanTp_TxConfirmation.c

TESTS_SRCS = \
	test/main.c \
	test/test_CanTp_CancelReceive.c \
    test/test_CanTp_CancelTransmit.c \
    test/test_CanTp_ChangeParameter.c \
    test/test_CanTp_GetVersionInfo.c \
    test/test_CanTp_Init.c \
    test/test_CanTp_MainFunction.c \
    test/test_CanTp_ReadParameter.c \
    test/test_CanTp_RxIndication.c \
    test/test_CanTp_Shutdown.c \
    test/test_CanTp_Transmit.c \
    test/test_CanTp_TxConfirmation.c

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
	@echo ">>==== Running tests ====<<"
	./$(TARGET)

$(TARGET): $(TEST_OBJS)
	gcc $(BUILD_DIR)/test/*.o -o $@

$(BUILD_DIR)/%.o: %.c $(DEPS)
	gcc -c $< $(INCLUDES) -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_DIR)/test

clean:
	rm -rf *.o $(TARGET) $(BUILD_DIR)
