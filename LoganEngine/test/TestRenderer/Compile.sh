
MainFile="../../src/lTest/TestRenderer/Test_lffGLRenderer.cpp"

TestEnvironmentFiles="../../src/lTest/TestRenderer/RendererTest.cpp"
SceneGraphFiles="../../src/lScene/lr3DScene/lrMaterialLibrary.cpp"
#TODO Ezt valahogy csinosítani kéne. :S
MathLibFiles="../../src/lMath/lmMatrix3x3.cpp"
MathLibFiles2="../../src/lMath/lmMatrix4x4.cpp"

Flags="-pg"

g++ "${MainFile}" "${TestEnvironmentFiles}" "${SceneGraphFiles}" "${MathLibFiles}" "${MathLibFiles2}" -lGL -lglut -lGLU "${Flags}" -std=c++11 -o Test