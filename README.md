# SCOP_GL
.obj rendering using OpenGL

Project done during my formation at 19. https://campus19.be/

## Controls (Qwerty)
```
WSADER      - turn object around
+- (PAD)    - change movement speed
+-          - resize object

C           - switch color mode (DEFAULT-MATERIAL-TEXTURE-GRADIENT)
B           - enable/disable GL_DEPTH_TEST
F           - fill/unfill faces with color

74 (PAD)    - change R channel of background color
85 (PAD)    - change G channel of background color
96 (PAD)    - change B channel of background color
```

## Run
After cloning the project, cd into repo and run:

```
/SCOP $> make
/SCOP $> ./scop					# to run default file
/SCOP $> ./scop Ressources/<file_you_want>.obj
```

## Libraries
* [GLFW](https://github.com/glfw/glfw.git) is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development. It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.
* [GLEW](https://github.com/nigels-com/glew.git) provides efficient run-time mechanisms for determining which OpenGL extensions are supported on the target platform.
* [GLM](https://github.com/g-truc/glm.git) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
* [SOIL](https://github.com/littlstar/soil.git) is a tiny C library used primarily for uploading textures into OpenGL.