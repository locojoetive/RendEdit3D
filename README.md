## 1. The Project: 3d-rasterizer

I created this project to gain experience with Dear ImGUI in combination with GLFW.
Throughout its development this Readme will grow and evolve to capture all implemented features and all requirements to setup and build the project

A lot of this project is inspired by [Victor Gordan](https://www.youtube.com/channel/UC8WizezjQVClpWfdKMwtcmw).

### 2. Features

This project will have the following features:

1. Load and display 3D models
2. Create multiple light sources
3. Support directional-, point- and spot light
4. Support user inputs to move around the scene
5. Move, Rotate, and Scale the models
6. Load and apply a given texture and specular map to the model

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
