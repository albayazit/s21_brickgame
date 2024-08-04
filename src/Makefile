CC = gcc
TETRIS_CFLAGS = -I brick_game/tetris/inc -I gui/cli/inc -Wall -Wextra -Werror --coverage
TETRIS_OBJ_FILES = out/game.o out/main.o out/fsm.o out/gui.o
BACKEND_OBJ_FILES = out/game.o out/fsm.o  out/tests.o
TARGET = tetris
BINDIR = out
ARCHIVE_NAME = backup.tar.gz
SRC = brick_game/ gui/ Doxyfile
TEST_FLAGS = -I brick_game/tetris/inc -lcheck --coverage -lm -pthread
# $< - входной исходный файл
# $^ - все зависимости
# $@ - имя цели

# Основные цели
.PHONY: all install uninstall clean dvi dist

all: create_dir $(TARGET)

$(TARGET): $(TETRIS_OBJ_FILES)
	$(CC) $(TETRIS_CFLAGS) $^ -o $@ -lncurses
install: all
	install -m755 $(TARGET) $(BINDIR)/$(TARGET)
uninstall:
	rm -f $(BINDIR)/$(TARGET)
dvi:
	doxygen Doxyfile
	open docs/html/index.html
dist:
	tar -czf $(ARCHIVE_NAME) $(SRC)
test: clean create_dir $(BACKEND_OBJ_FILES)
	$(CC) $(BACKEND_OBJ_FILES) -o $@ $(TEST_FLAGS)
	./$@
leaks: all
	leaks -atExit -- ./$(TARGET)
gcov_report: all 
	./$(TARGET)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	open coverage/index.html
clang:
	cp ../materials/linters/.clang-format .
	clang-format -i brick_game/tetris/*.c brick_game/tetris/inc/*.h gui/cli/*.c gui/cli/inc/*.h 
	clang-format -n brick_game/tetris/*.c brick_game/tetris/inc/*.h gui/cli/*.c gui/cli/inc/*.h
create_dir:
	mkdir -p $(BINDIR)
clean:
	rm -rf out tetris db.txt
	rm -rf ../docs coverage docs
	rm -f $(ARCHIVE_NAME) test coverage.info .clang-format

out/%.o: brick_game/tetris/%.c
	$(CC) $(TETRIS_CFLAGS) -c $< -o $@

out/%.o: gui/cli/%.c
	$(CC) $(TETRIS_CFLAGS) -c $< -o $@