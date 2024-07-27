CC = gcc
TETRIS_CFLAGS = -I brick_game/tetris/inc -I gui/cli/inc -Wall -Wextra -Werror
TETRIS_OBJ_FILES = out/game.o out/main.o out/fsm.o out/gui.o

# $< - входной исходный файл
# $^ - все зависимости
# $@ - имя цели

tetris: $(TETRIS_OBJ_FILES)
	$(CC) $(TETRIS_CFLAGS) $^ -o $@ -lncurses
	./tetris

out/%.o: brick_game/tetris/%.c
	$(CC) $(TETRIS_CFLAGS) -c $< -o $@

out/%.o: gui/cli/%.c
	$(CC) $(TETRIS_CFLAGS) -c $< -o $@

clean:
	$(RM) out/*.o tetris