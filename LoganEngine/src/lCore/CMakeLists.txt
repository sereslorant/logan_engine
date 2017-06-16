
set(L_CORE_DIR "./")
include_directories(${L_CORE_DIR})

add_subdirectory(lApiAdapter/lSDL2_ApiAdapter)
add_subdirectory(lRenderer/lrRenderer/lGLRenderer)
add_subdirectory(lScene/lrScene)

