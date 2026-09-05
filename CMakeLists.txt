cmake_minimum_required(VERSION 3.16)
project(genshinn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(genshinn
    main.cpp
    Game.cpp
    Character.cpp
    Player.cpp
    NPC.cpp
    Enemy.cpp
    BossEnemy.cpp
    Inventory.cpp
    Equipment.cpp
    Shop.cpp
    Room.cpp
    World.cpp
    CommandParser.cpp
    QuestManager.cpp
    GameFactory.cpp
    CombatSystem.cpp
    SaveManager.cpp
)

if (MSVC)
    target_compile_options(genshinn PRIVATE /W4 /utf-8)
else()
    target_compile_options(genshinn PRIVATE -Wall -Wextra -Wpedantic)
endif()
