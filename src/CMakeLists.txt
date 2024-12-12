# Минимальная версия CMake
cmake_minimum_required(VERSION 3.16)

# Название проекта
project(BrickGame)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для выбора версии сборки (по умолчанию CLI)
option(BUILD_DESKTOP "Build desktop version (with Qt)" OFF)
option(BUILD_TESTS "Build unit tests" ON)

# Пути к исходным файлам
set(SNAKE_SRC brick_game/snake)
set(TETRIS_SRC brick_game/tetris)
set(COMMON_SRC common)
set(CLI_SRC gui/cli)
set(DESKTOP_SRC gui/desktop)

# Подключаем общие заголовочные файлы
include_directories(
    ${COMMON_SRC}
    ${SNAKE_SRC}/inc
    ${TETRIS_SRC}/inc
)

# Общие исходники для игр
set(GAME_SOURCES
    ${SNAKE_SRC}/snake_controller_wrapper_c.cpp
    ${TETRIS_SRC}/fsm.c
    ${TETRIS_SRC}/game.c
    ${TETRIS_SRC}/tetris_controller.c
)

if(BUILD_TESTS)
    enable_testing()  # Включаем поддержку тестирования в CMake

    # Ручное указание пути к Check
    set(CHECK_INCLUDE_DIR "/opt/homebrew/Cellar/check/0.15.2/include")
    set(CHECK_LIB_DIR "/opt/homebrew/Cellar/check/0.15.2/lib")

    # Добавляем include path и library path для Check
    include_directories(${CHECK_INCLUDE_DIR})
    link_directories(${CHECK_LIB_DIR})

    # === Тесты для Tetris ===
    add_executable(tetris_tests
        ${TETRIS_SRC}/tests.c
        ${TETRIS_SRC}/fsm.c
        ${TETRIS_SRC}/game.c
    )
    target_link_libraries(tetris_tests check m)  # Линкуем с Check и math
    add_test(NAME TetrisTests COMMAND tetris_tests)

    # === Настройка для Snake ===
    # Указываем путь к GTest
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    # Тесты для Snake
    add_executable(snake_tests
        ${SNAKE_SRC}/tests.cpp  # Ваши тесты GTest
        ${SNAKE_SRC}/inc/snake_fsm.h         # FSM для Snake
        ${SNAKE_SRC}/inc/snake_game.h        # Логика игры Snake
    )
    target_link_libraries(snake_tests ${GTEST_BOTH_LIBRARIES} pthread)  # Линкуем с GTest
    add_test(NAME SnakeTests COMMAND snake_tests)
endif()




# Сборка для Desktop или CLI
if(BUILD_DESKTOP)
    message(STATUS "Building Desktop version (Qt)")

    # Указываем путь к CMake для Qt
    set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.7.3/lib/cmake")  # Замените на ваш путь к Qt
    find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

    # Добавляем Desktop специфичные исходники
    set(DESKTOP_SOURCES
        ${DESKTOP_SRC}/desktop_view.cpp
    )

    # Подключаем заголовки для Desktop
    include_directories(${DESKTOP_SRC}/inc)

    # Генерация MOC файлов для Qt
    qt6_wrap_cpp(MOC_SOURCES ${DESKTOP_SRC}/desktop_view.h)

    # Создаём исполняемый файл для Desktop
    add_executable(brickgame ${GAME_SOURCES} ${DESKTOP_SOURCES} ${MOC_SOURCES})

    # Линкуем с Qt
    target_link_libraries(brickgame Qt6::Core Qt6::Widgets)

else()
    message(STATUS "Building CLI version (ncurses)")

    # Добавляем CLI специфичные исходники
    set(CLI_SOURCES
        ${CLI_SRC}/gui.c
        ${CLI_SRC}/main.c
    )

    # Подключаем заголовки для CLI
    include_directories(${CLI_SRC}/inc)

    # Исполняемый файл для CLI
    add_executable(brickgame ${GAME_SOURCES} ${CLI_SOURCES})

    # Линкуем с ncurses
    target_link_libraries(brickgame ncurses)
endif()