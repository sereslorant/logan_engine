
#Setting up Logan executable

#Setting up Example directories.
set(L_EXAMPLE_DIR "./")
#Setting up Main src-s.
set(L_MAIN_SRC "${L_EXAMPLE_DIR}/lMain.cpp")

#Adding game libraries

add_subdirectory(lP2_TestGameMode)
add_subdirectory(lPacMan)
add_subdirectory(lGL3_RendererTest)

#Compiling game executables

#Compiling lProtoFramework
add_executable(lProtoFramework ${L_MAIN_SRC})
target_link_libraries(lProtoFramework lP2_TestGameMode lSDL2_ApiAdapter lGL3Renderer)
#Compiling lPacManExec
add_executable(lPacManExec ${L_MAIN_SRC})
target_link_libraries(lPacManExec lPM_GameMode lSDL2_ApiAdapter lGL3Renderer)
#Compiling lGL3TestGame
add_executable(lGL3TestGame ${L_MAIN_SRC})
target_link_libraries(lGL3TestGame lGL3_GameMode lSDL2_ApiAdapter lGL3Renderer)
