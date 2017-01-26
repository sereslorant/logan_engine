cmake_minimum_required(VERSION 2.6)
project(LoganEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(OPENGL REQUIRED gl)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

set(L_SRC_DIR "../../src")

#Setting up lSDL2_ApiAdapter directories.
set(L_SDL2_API_ADAPTER_SRC_DIR	"${L_SRC_DIR}/lApiAdapter/lSDL2_ApiAdapter")
set(L_SDL2_THREAD_SRC_DIR	"${L_SDL2_API_ADAPTER_SRC_DIR}/lSDL2_Thread")

#Setting up lSDL2_ApiAdapter src-s.
set(L_SDL2_THREAD_SRC	"${L_SDL2_THREAD_SRC_DIR}/lSDL2_Thread.cpp")

set(L_SDL2_API_ADAPTER_SRC	${L_SDL2_THREAD_SRC}
				"${L_SDL2_API_ADAPTER_SRC_DIR}/lSDL2_ApiAdapter.cpp"
				"${L_SDL2_API_ADAPTER_SRC_DIR}/lSDL2_FrameLimiter.cpp"
				)

#Compiling lSDL2_ApiAdapter
add_library(lSDL2_ApiAdapter ${L_SDL2_API_ADAPTER_SRC})
target_link_libraries(lSDL2_ApiAdapter ${SDL2_LIBRARIES})


#Setting up lGL3Renderer directories.
set(L_GL_RENDERER_SRC_DIR	"${L_SRC_DIR}/lRenderer/lGLRenderer")
set(L_GL3_SHADERS_SRC_DIR	"${L_GL_RENDERER_SRC_DIR}/lGL3Renderer/lGL3Shaders")
set(L_GL3_CACHED_LAYER_SRC_DIR	"${L_GL_RENDERER_SRC_DIR}/lGL3Renderer/lGL3DCachedLayer")
set(L_GL3_SCENE_CACHE_SRC_DIR	"${L_GL3_CACHED_LAYER_SRC_DIR}/lrGL3SceneCache")

#Setting up lGL3Renderer src-s.
set(L_GL3_SCENE_CACHE_SRC	"${L_GL3_SCENE_CACHE_SRC_DIR}/lrGL3TextureList.cpp"
				"${L_GL3_SCENE_CACHE_SRC_DIR}/lrGL3SceneCache.cpp")

set(L_GL3_CACHED_LAYER_SRC	${L_GL3_SCENE_CACHE_SRC}
				"${L_GL3_CACHED_LAYER_SRC_DIR}/lrGL3DCachedLayer.cpp")

set(L_GL3_SHADERS_SRC		"${L_GL3_SHADERS_SRC_DIR}/lrGL3ShaderLibrary.cpp")

set(L_GL_RENDERER_SRC		${L_GL3_CACHED_LAYER_SRC}
				${L_GL3_SHADERS_SRC}
				"${L_GL_RENDERER_SRC_DIR}/lGLExt.cpp"
				)

#Compiling lGL3Renderer
add_library(lGL3Renderer ${L_GL_RENDERER_SRC})
target_link_libraries(lGL3Renderer ${OPENGL_LIBRARIES})


#Setting up lProtoTestGameMode directories.
set(LP2_TEST_GAME_MODE_SRC_DIR "${L_SRC_DIR}/lProto/lGameModes/lP2_TestGameMode")

#Setting up lProtoTestGameMode src-s.
set(LP2_TEST_GAME_MODE_SRC "${LP2_TEST_GAME_MODE_SRC_DIR}/lP2_TestGameModeInstantiator.cpp")

#Compiling lProtoTestGameMode
add_library(lP2_TestGameMode ${LP2_TEST_GAME_MODE_SRC})


#Setting up lPM_GameMode directories.
set(LPM_GAME_MODE_SRC_DIR "${L_SRC_DIR}/lGame/lTimer.cpp" "${L_SRC_DIR}/lGame/lEvent.cpp" "${L_SRC_DIR}/lProto/lGameModes/lPacMan")

#Setting up lPM_GameMode src-s.
set(LPM_GAME_MODE_SRC "${LPM_GAME_MODE_SRC_DIR}/lPM_GameInstantiator.cpp")

#Compiling lPM_GameMode
add_library(lPM_GameMode ${LPM_GAME_MODE_SRC})


#Setting up lProto directories.
set(L_PROTO_SRC_DIR "${L_SRC_DIR}/lProto")

#Setting up lProto src-s.
set(L_PROTO_MAIN_SRC "${L_PROTO_SRC_DIR}/Main.cpp")

#Compiling lProtoFramework
add_executable(lProtoFramework ${L_PROTO_MAIN_SRC})
target_link_libraries(lProtoFramework lP2_TestGameMode lSDL2_ApiAdapter lGL3Renderer)

#Compiling lPacMan

add_executable(lPacMan ${L_PROTO_MAIN_SRC})
target_link_libraries(lPacMan lPM_GameMode lSDL2_ApiAdapter lGL3Renderer)
