cmake_minimum_required(VERSION 2.6)
project(loganengine_old)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC_DIR "../../old")

#Setting main sources
set(lMain "${SRC_DIR}/lMain.cpp" "${SRC_DIR}/lRandomGenerator.cpp" "${SRC_DIR}/lUtils.cpp")

#Setting lApiAdapter sources
set(L_API_ADAPTER_DIR		"${SRC_DIR}/lApiAdapter")
set(L_SDL2_API_ADAPTER_DIR	"${L_API_ADAPTER_DIR}/lSDL2ApiAdapter")

set(lSDL2_ApiAdapter	"${L_SDL2_API_ADAPTER_DIR}/lSDL2ApiAdapter.cpp"
			"${L_SDL2_API_ADAPTER_DIR}/lSDL2FrameLimiter.cpp"
			"${L_SDL2_API_ADAPTER_DIR}/lSDL2GLContext.cpp"
			"${L_SDL2_API_ADAPTER_DIR}/lSDL2GLExtLoader.cpp"
			"${L_SDL2_API_ADAPTER_DIR}/lSDL2Input.cpp"
			)

set(lApiAdapter	${lSDL2_ApiAdapter}
		"${L_API_ADAPTER_DIR}/lInput.cpp"
		"${L_API_ADAPTER_DIR}/lDLLFunctions.cpp"
		)

#Setting lGameMode sources
set(L_GAME_MODE_DIR "${SRC_DIR}/lGameModes")
set(L_MAIN_MENU_DIR "${L_GAME_MODE_DIR}/lMainMenu")
set(L_TECH_DEMO_DIR "${L_GAME_MODE_DIR}/lTechDemo")

set(lMainMenu "${L_MAIN_MENU_DIR}/lMainMenu.cpp"
	      "${L_MAIN_MENU_DIR}/lMenu.cpp"
	      )

set(lTechDemo "${L_TECH_DEMO_DIR}/lTechDemo.cpp"
	      )

set(lGameMode ${lMainMenu}
	      ${lTechDemo}
	      "${L_GAME_MODE_DIR}/lEvent.cpp"
	      "${L_GAME_MODE_DIR}/lGame.cpp"
	      "${L_GAME_MODE_DIR}/lParticleSystem.cpp"
	      "${L_GAME_MODE_DIR}/lTimer.cpp"
	      )

#Setting lGooey sources
set(L_GOOEY_DIR			"${SRC_DIR}/lGooey")
set(L_GOO_ANIMATE_DIR		"${L_GOOEY_DIR}/lGooAnimation/lGooAnimate")
set(L_GOO_SVG_DIR		"${L_GOOEY_DIR}/lGooSvg")
set(L_GOO_SVG_ATTRIBUTES_DIR	"${L_GOO_SVG_DIR}/lGooSvgAttributes")
set(L_GOO_SVG_ELEMENTS_DIR	"${L_GOO_SVG_DIR}/lGooSvgElements")

set(lGooAnimate	"${L_GOO_ANIMATE_DIR}/lGooAnimate.cpp" "${L_GOO_ANIMATE_DIR}/lGooScalarAnimate.cpp")

set(lGooSvgAttributes	"${L_GOO_SVG_ATTRIBUTES_DIR}/lGooSvgAnimAttributes.cpp"
			"${L_GOO_SVG_ATTRIBUTES_DIR}/lGooSvgLineAttributes.cpp"
			"${L_GOO_SVG_ATTRIBUTES_DIR}/lGooSvgCircleAttributes.cpp"
			"${L_GOO_SVG_ATTRIBUTES_DIR}/lGooSvgPaintingAttributes.cpp"
			"${L_GOO_SVG_ATTRIBUTES_DIR}/lGooSvgEllipseAttributes.cpp"
			"${L_GOO_SVG_ATTRIBUTES_DIR}/lGooSvgRectAttributes.cpp"
			)

set(lGooSvgElements	"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgAnim.cpp"
			"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgEllipse.cpp"
			"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgRect.cpp"
			"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgShape.cpp"
			"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgCircle.cpp"
			"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgLine.cpp"
			"${L_GOO_SVG_ELEMENTS_DIR}/lGooSvgRoot.cpp"
			)

set(lGooSvg	${lGooSvgAttributes}
		${lGooSvgElements}
		"${L_GOO_SVG_DIR}/lGooSVG.cpp"
		"${L_GOO_SVG_DIR}/lGooSvgGlobal.cpp")

set(lGooey	${lGooAnimate}
		${lGooSvg}
		"${L_GOOEY_DIR}/lGooCircle.cpp"
		"${L_GOOEY_DIR}/lGooEvent.cpp"
		"${L_GOOEY_DIR}/lGooObject.cpp"
		"${L_GOOEY_DIR}/lGooRect.cpp"
		)

#Setting lMath sources
set(L_MATH_DIR "${SRC_DIR}/lMath")

set(lMath	"${L_MATH_DIR}/lMath.cpp"
		"${L_MATH_DIR}/lmGeometry.cpp"
		"${L_MATH_DIR}/lmMatrix3x3.cpp"
		"${L_MATH_DIR}/lmMatrix4x4.cpp"
		)

#Setting lPhys2 sources
set(L_PHYS2_DIR				"${SRC_DIR}/lPhys2")
set(L_PHYS_COLLISION_DETECTOR_DIR	"${L_PHYS2_DIR}/lpCollisionDetector")
set(L_PHYS_INTEGRATOR_DIR		"${L_PHYS2_DIR}/lpIntegrator")
set(L_PHYS_INTERACTION_MODEL_DIR	"${L_PHYS2_DIR}/lpInteractionModel")
set(L_PHYS_RIGID_BODY_DIR		"${L_PHYS2_DIR}/lpRigidBody")

set(lpCollisionDetector "${L_PHYS_COLLISION_DETECTOR_DIR}/lpDiscreteCollisionDetector.cpp"
			)

set(lpIntegrator	"${L_PHYS_INTEGRATOR_DIR}/lpRK4Integrator.cpp"
			)

set(lpInteractionModel	"${L_PHYS_INTERACTION_MODEL_DIR}/lpImpulseCollisionModel.cpp"
			"${L_PHYS_INTERACTION_MODEL_DIR}/lpInteractionModel.cpp"
			"${L_PHYS_INTERACTION_MODEL_DIR}/lpSpringCollisionModel.cpp"
			)

set(lpRigidBody		"${L_PHYS_RIGID_BODY_DIR}/lpBox.cpp"
			"${L_PHYS_RIGID_BODY_DIR}/lpMesh.cpp"
			"${L_PHYS_RIGID_BODY_DIR}/lpPlane.cpp"
			"${L_PHYS_RIGID_BODY_DIR}/lpRigidBody.cpp"
			"${L_PHYS_RIGID_BODY_DIR}/lpSphere.cpp"
			)

set(lPhys2	${lpCollisionDetector}
		${lpIntegrator}
		${lpInteractionModel}
		${lpRigidBody}
		"${L_PHYS2_DIR}/lPhys2.cpp"
		"${L_PHYS2_DIR}/lpSimulationWorld.cpp"
		)

#Setting lRenderer sources
set(L_RENDERER_DIR		"${SRC_DIR}/lRenderer")
set(LR_CHARACTER_ANIM_DIR	"${L_RENDERER_DIR}/lrCharacterAnim")
set(LR_GL_RENDERER_DIR		"${L_RENDERER_DIR}/lrGLRenderer")
set(LR_GL3_3GUI_RENDERER_DIR	"${LR_GL_RENDERER_DIR}/lrGL3_3GuiRenderer")
set(LR_GL3_3RENDERER_DIR	"${LR_GL_RENDERER_DIR}/lrGL3_3Renderer")
set(LR_GL_SHADER_DIR		"${LR_GL_RENDERER_DIR}/lrGLShader")
set(LR_SCENE_GRAPH_DIR		"${L_RENDERER_DIR}/lrSceneGraph")

set(lrCharacterAnim	"${LR_CHARACTER_ANIM_DIR}/lrCharacter.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrCharacterAnim.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrCharacterAnimBlend.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrCharacterAnimPlay.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrKeyframeAnimation.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrKeyframeAnimState.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrSkeleton.cpp"
			"${LR_CHARACTER_ANIM_DIR}/lrSkeletonBlendState.cpp"
			)

set(lrGL3_3GuiRenderer	"${LR_GL3_3GUI_RENDERER_DIR}/lrGL3_3GuiRenderer.cpp")

set(lrGL3_3Renderer	"${LR_GL3_3RENDERER_DIR}/lrGL3_3ForwardRenderer.cpp"
			"${LR_GL3_3RENDERER_DIR}/lrGL3_3SkmShader.cpp"
			"${LR_GL3_3RENDERER_DIR}/lrGL3_3StmShader.cpp"
			)

set(lrGLShader		"${LR_GL_SHADER_DIR}/lrGLPostProcessShader.cpp"
			"${LR_GL_SHADER_DIR}/lrGLShader.cpp"
			)

set(lrGLRenderer	${lrGL3_3GuiRenderer}
			${lrGL3_3Renderer}
			${lrGLShader}
			"${LR_GL_RENDERER_DIR}/lGLExt.cpp"
			"${LR_GL_RENDERER_DIR}/lGLFunctions.cpp"
			"${LR_GL_RENDERER_DIR}/lrGLFramebuffer.cpp"
			"${LR_GL_RENDERER_DIR}/lrGLImg.cpp"
			"${LR_GL_RENDERER_DIR}/lrGLSkeletalMesh.cpp"
			"${LR_GL_RENDERER_DIR}/lrGLStaticMesh.cpp"
			)

set(lrSceneGraph	"${LR_SCENE_GRAPH_DIR}/lrsSceneNode.cpp"
			"${LR_SCENE_GRAPH_DIR}/lrsSkeletalMeshNode.cpp"
			"${LR_SCENE_GRAPH_DIR}/lrsStaticMeshNode.cpp"
			)

set(lRenderer	${lrCharacterAnim}
		${lrGLRenderer}
		${lrSceneGraph}
		"${L_RENDERER_DIR}/lrMultiMaterial.cpp"
		"${L_RENDERER_DIR}/lrSkeletalMesh.cpp"
		"${L_RENDERER_DIR}/lrStaticMesh.cpp"
		)

#Setting lResourceManager sources
set(lResourceManager	"../../old/lResourceManager/BMP.cpp"
			"../../old/lResourceManager/lResourceManager.cpp"
			"../../old/lResourceManager/lrmHeightmapLoader.cpp"
			"../../old/lResourceManager/lrmMd5Loader.cpp"
			"../../old/lResourceManager/lrmResourceManager.cpp"
			"../../old/lResourceManager/lrmSkeletalMesh.cpp"
			"../../old/lResourceManager/lrmStaticMesh.cpp"
			"../../old/lResourceManager/lrmWavefrontLoader.cpp"
			)

#Setting content directory
set(CONTENT_SRC_DIR	"${SRC_DIR}/Content")
set(CONTENT_DEST_DIR	"./Content")

#Copying content
file(COPY  "${CONTENT_SRC_DIR}/" DESTINATION ${CONTENT_DEST_DIR})

#Building executable
add_executable(loganengine_old ${lMain} ${lApiAdapter} ${lGameMode} ${lGooey} ${lMath} ${lPhys2} ${lRenderer} ${lResourceManager})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(OPENGL REQUIRED gl)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(loganengine_old ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
