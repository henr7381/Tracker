# Makefile for the Tracker project

CXX := g++

CXXFLAGS := -Wall -Wextra -std=c++17 -I.
CXXFLAGS += $(shell pkg-config --cflags opencv4)

LDFLAGS := -lglfw -lGL $(shell pkg-config --libs opencv4)

SRC_DIR := ./src
BUILD_DIR := ./build
BIN_DIR := ./bin
TARGET := $(BIN_DIR)/Tracker

MAIN_SRC := $(SRC_DIR)/main.cpp
MODULE_SRCS := $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.cpp))
SRCS := $(MAIN_SRC) $(MODULE_SRCS)

MAIN_OBJ := $(BUILD_DIR)/main.o
MODULE_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(MODULE_SRCS))
OBJS := $(MAIN_OBJ) $(MODULE_OBJS)

DEPS := $(OBJS:.o=.d)

all: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

-include $(DEPS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
