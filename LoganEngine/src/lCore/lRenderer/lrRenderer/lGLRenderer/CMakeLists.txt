
#Setting up lGL3Renderer directories.
set(L_GL_RENDERER_SRC_DIR	"./")
set(L_GL3_CACHED_LAYER_SRC_DIR	"${L_GL_RENDERER_SRC_DIR}/lGL3Renderer/lGL3DCachedLayer")
set(L_GL3_SCENE_CACHE_SRC_DIR	"${L_GL3_CACHED_LAYER_SRC_DIR}/lrGL3SceneCache")

#Setting up lGL3Renderer src-s.
set(L_GL3_SCENE_CACHE_SRC	"${L_GL3_SCENE_CACHE_SRC_DIR}/lrGL3SceneCache.cpp")

set(L_GL3_CACHED_LAYER_SRC	${L_GL3_SCENE_CACHE_SRC}
				"${L_GL3_CACHED_LAYER_SRC_DIR}/lrGL3DCachedLayer.cpp")



set(L_GL_RENDERER_SRC		${L_GL3_CACHED_LAYER_SRC}
							${L_GL_SHADERS_SRC}
							"${L_GL_RENDERER_SRC_DIR}/lGLExt.cpp"
							)

add_library(lGL3Renderer ${L_GL_RENDERER_SRC})
target_link_libraries(lGL3Renderer lGLResourceLoader lGLShaderLibrary ${OPENGL_LIBRARIES})

add_subdirectory(lGLShaders)
add_subdirectory(lGLResources)
