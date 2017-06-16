
#Setting up Md5Loader directories.
#set(L_MD5_LOADER_SRC_DIR	"${L_CORE_DIR}/lResourceManager/Md5Loader")
set(L_MD5_LOADER_SRC_DIR	".")

#Setting up Md5Loader src-s.
set(L_MD5_LOADER_SRC	"${L_MD5_LOADER_SRC_DIR}/lrmMd5Loader.cpp"
						"${L_MD5_LOADER_SRC_DIR}/md5Anim.cpp"
						"${L_MD5_LOADER_SRC_DIR}/md5File.cpp"
						)

#Compiling Md5Loader
add_library(Md5Loader ${L_MD5_LOADER_SRC})

