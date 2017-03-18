
set(L_SRC_DIR "../../")

#Setting up lGL3_GameMode directories.
set(L_TEST_GL3_MAIN_SRC "${L_SRC_DIR}/lTest/TestRenderer")
set(LGL3_GAME_MODE_SRC_DIR "./")
#Setting up lGL3_GameMode src-s.
set(LGL3_GAME_MODE_SRC "${LGL3_GAME_MODE_SRC_DIR}/lGL3_RendererTestInstantiator.cpp" "${L_TEST_GL3_MAIN_SRC}/RendererTest.cpp")
#Compiling lGL3_GameMode
add_library(lGL3_GameMode ${LGL3_GAME_MODE_SRC})
target_link_libraries(lGL3_GameMode l3DScene Md5Loader)
