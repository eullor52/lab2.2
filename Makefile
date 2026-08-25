BUILD_DIR ?= build
UI_BUILD_DIR = ui
CMAKE_GENERATOR = MinGW Makefiles

CMAKE_FLAGS = -DCMAKE_BUILD_TYPE=Debug
ifdef CMAKE_PREFIX_PATH
    CMAKE_FLAGS += -DCMAKE_PREFIX_PATH="$(CMAKE_PREFIX_PATH)"
endif
ifdef CMAKE_GENERATOR
    CMAKE_FLAGS += -G "$(CMAKE_GENERATOR)"
endif

$(BUILD_DIR)/CMakeCache.txt: CMakeLists.txt
	cmake -S . -B $(BUILD_DIR) $(CMAKE_FLAGS)

all: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR)

ui: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR) --target ui

tests: $(BUILD_DIR)/CMakeCache.txt
	cmake --build $(BUILD_DIR) --target run_tests

run_tests: tests
	cd $(BUILD_DIR) && GTEST_BRIEF=1 ctest --output-on-failure

run_ui: ui
	./$(BUILD_DIR)/$(UI_BUILD_DIR)/ui.exe

clean:
	rm -rf $(BUILD_DIR)

.DEFAULT_GOAL := all

.PHONY: all ui tests run_tests run_ui clean