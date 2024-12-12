#include "fsm.h"
#include "game.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

int tetrisControllerIsGameOver();
GameInfo_t tetrisControllerUpdateGame();
void tetrisControllerUserInput(UserAction_t *user_action, bool hold);
void tetrisControllerFreeMemory();

#ifdef __cplusplus
}
#endif