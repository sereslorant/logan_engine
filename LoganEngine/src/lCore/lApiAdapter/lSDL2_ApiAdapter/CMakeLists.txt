
#Setting up lSDL2_ApiAdapter directories.
set(L_SDL2_API_ADAPTER_SRC_DIR	"./")
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
