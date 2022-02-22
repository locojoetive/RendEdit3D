## 1. The Project: 3d-rasterizer

I created this project to gain experience with Dear ImGUI in combination with GLFW.
Throughout its development this Readme will grow and evolve to capture all implemented features and all requirements to setup and build the project

A lot of this project is inspired by [Victor Gordan](https://www.youtube.com/watch?v=45MIykWJ-C4&t=5140s).

### 2. Features

This project is supposed to have the following features:

1. Load and display 3D models.
2. Load and apply a given texture and specular map to the model
4. Support user inputs to rotate around and zoom into the model
5. Support user inputs to change the pivot of the camera's rotation
2. Allow for different types of light sources

Additional Features may be:

1. Load multiple models with their respective textures

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


This is it for now.
