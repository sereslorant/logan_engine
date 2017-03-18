
#Setting up lProtoTestGameMode directories.
set(LP2_TEST_GAME_MODE_SRC_DIR "./")
#Setting up lProtoTestGameMode src-s.
set(LP2_TEST_GAME_MODE_SRC "${LP2_TEST_GAME_MODE_SRC_DIR}/lP2_TestGameModeInstantiator.cpp")
#Compiling lProtoTestGameMode
add_library(lP2_TestGameMode ${LP2_TEST_GAME_MODE_SRC})
