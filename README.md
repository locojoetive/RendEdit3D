## 1. RendEdit3D

This project is implemented using C++, Dear ImGUI and GLFW (OpenGL), and provides the following features:

1. Load and display multiple 3D models
2. Move around the scene using keyboard and mouse inputs
3. Move, Rotate, and Scale the models


This document aims to clarify how to setup, build, and use the tool.

A lot of this project is inspired by [Victor Gordan](https://www.youtube.com/channel/UC8WizezjQVClpWfdKMwtcmw) and [The Cherno](https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)

![](https://github.com/locojoetive/Simple-Renderer/blob/main/docs/rasterizer.gif)


## 2. Requirements

### 2.1. Development Environment

This project is a C++ project created with the following setup:

**IDE:** Visual Studio 2022 Community

**Compiler:** MSVC 17

**Target System:** x64

### 2.2. Additional Includes
- [ImGUI 1.8.7](https://github.com/ocornut/imgui)

### 2.3. External Packages
- [GLFW 3.3.6](https://github.com/glfw/glfw/releases)
- [Glad](https://glad.dav1d.de/)

### 2.4. Libs
- glfw3.lib (from GLFW source)
- opengl32.lib (included in Windows OS)
