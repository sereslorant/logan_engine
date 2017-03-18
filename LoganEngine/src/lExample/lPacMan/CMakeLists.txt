
#Setting up lPM_GameMode directories.
set(LPM_GAME_MODE_SRC_DIR "./")
#Setting up lPM_GameMode src-s.
set(LPM_GAME_MODE_SRC "${LPM_GAME_MODE_SRC_DIR}/lPM_GameInstantiator.cpp")
#Compiling lPM_GameMode
add_library(lPM_GameMode ${LPM_GAME_MODE_SRC})
