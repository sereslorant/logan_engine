
#lCore

These libraries offer the core functionality of LoganEngine, meaning that they offer the bare minimum required to make a simple game. This bare minimum consists of the Math lib, the Renderer, the Scene Graph, the Resource Manager and the Wrapper around OS functionalities

The libraries are listed alphabetically

##lApiAdapter

This library is responsible for offering the functionalities of the OS to the game. These OS functionalities include Input, Window management and multithreading.

##lGame

This library contains the main loop and specifies the interfaces the game logic must implement.

##lMath

This is the Math library of the engine. It offers vector, matrix an quaternion operations, parametric curves and collision detection algorithms for geometric objects.

##lRenderer

This library is responsible for rendering onto the screen using a client API such as OpenGL, Vulkan or DirectX 12.

##lResourceManager

This library is responsible for supplying the game with content.

##lSceneGraph

This library offers the 2D and 3D Scene graphs. The 2D Scene graph consists of 2D shapes such as circles, quads and so on, the 3D scene graph consists of lights, static and skeletal meshes.
