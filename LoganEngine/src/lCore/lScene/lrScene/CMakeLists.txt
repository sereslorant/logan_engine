
#Setting up l3DScene directories.
set(L_3D_SCENE_SRC_DIR	"./lr3DScene")

#Setting up l3DScene src-s.
set(L_3D_SCENE_SRC	"${L_3D_SCENE_SRC_DIR}/lrMaterialLibrary.cpp")

#Compiling l3DScene
add_library(l3DScene ${L_3D_SCENE_SRC})
