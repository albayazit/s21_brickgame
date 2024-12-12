PROJECT_NAME = BrickGame

BUILD_DIR = build
DIST_DIR = dist
INSTALL_DIR = /usr/local/bin
SOURCE_DIR = ../../
QT_CMAKE_PATH = /opt/homebrew/Cellar/qt/6.7.3/lib/cmake
COVERAGE_DIR = $(PWD)/$(BUILD_DIR)/coverage
LCOV = lcov
GENHTML = genhtml


CXX = g++
CMAKE = cmake
MAKE = make
INSTALL = install

.PHONY: all desktop cli install install-desktop install-cli uninstall clean dvi dist test test-tetris test-snake clang coverage

all: desktop cli

desktop:
	@echo "Building Desktop version..."
	@mkdir -p $(BUILD_DIR)/desktop
	cd $(BUILD_DIR)/desktop && $(CMAKE) -DCMAKE_PREFIX_PATH=$(QT_CMAKE_PATH) -DBUILD_DESKTOP=ON $(SOURCE_DIR)
	cd $(BUILD_DIR)/desktop && $(MAKE)

cli:
	@echo "Building CLI version..."
	@mkdir -p $(BUILD_DIR)/cli
	cd $(BUILD_DIR)/cli && $(CMAKE) -DBUILD_DESKTOP=OFF $(SOURCE_DIR)
	cd $(BUILD_DIR)/cli && $(MAKE)

install-desktop: desktop
	@echo "Installing Desktop version to $(INSTALL_DIR)..."
	@mkdir -p $(DIST_DIR)/desktop
	cp $(BUILD_DIR)/desktop/brickgame $(DIST_DIR)/desktop/brickgame-desktop
	$(INSTALL) $(DIST_DIR)/desktop/brickgame-desktop $(INSTALL_DIR)

install-cli: cli
	@echo "Installing CLI version to $(INSTALL_DIR)..."
	@mkdir -p $(DIST_DIR)/cli
	cp $(BUILD_DIR)/cli/brickgame $(DIST_DIR)/cli/brickgame-cli
	$(INSTALL) $(DIST_DIR)/cli/brickgame-cli $(INSTALL_DIR)

install: install-desktop install-cli

uninstall:
	@echo "Uninstalling..."
	rm -f $(INSTALL_DIR)/brickgame-desktop
	rm -f $(INSTALL_DIR)/brickgame-cli

clang:
	@echo "Running clang-format..."
	cp ../materials/linters/.clang-format .
	clang-format -i ./brick_game/tetris/*.c ./brick_game/tetris/inc/*.h ./brick_game/snake/*.cpp ./brick_game/snake/inc/*.h ./gui/cli/inc/*.h ./gui/cli/*.c ./gui/desktop/*.cpp ./gui/cli/inc/*.h ./common/*.h

clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR) $(DIST_DIR)
	rm -rf docs
	rm .clang-format

dvi:
	@echo "Generating documentation..."
	doxygen Doxyfile
	open docs/html/index.html

dist:
	@echo "Creating distribution archive..."
	mkdir -p $(DIST_DIR)
	tar -czvf $(DIST_DIR)/$(PROJECT_NAME).tar.gz $(SOURCE_DIR) --exclude=$(BUILD_DIR) --exclude=$(DIST_DIR)

test: test-tetris test-snake

test-tetris:
	@echo "Running Tetris tests..."
	@mkdir -p $(BUILD_DIR)/tests/tetris
	cd $(BUILD_DIR)/tests/tetris && cmake -DBUILD_TESTS=ON -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_C_FLAGS="--coverage" ../../../
	cd $(BUILD_DIR)/tests/tetris && make
	cd $(BUILD_DIR)/tests/tetris && ctest --output-on-failure

test-snake:
	@echo "Running Snake tests..."
	@mkdir -p $(BUILD_DIR)/tests/snake
	cd $(BUILD_DIR)/tests/snake && cmake -DBUILD_TESTS=ON -DCMAKE_CXX_FLAGS="--coverage" -DCMAKE_C_FLAGS="--coverage" ../../../
	cd $(BUILD_DIR)/tests/snake && make
	cd $(BUILD_DIR)/tests/snake && ctest --output-on-failure