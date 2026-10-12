# Gradient - Drawing & Image Editor

An interactive graphical application built with C++ and OpenGL, evolving into a real-time drawing and image editor featuring live shader processing, multi-layer manipulation, and a responsive control interface using Dear ImGui.

---

## Project Structure

The project follows a modular architecture (`.h` for declarations and `.cpp` for logic), designed to scale cleanly as new editor subsystems are integrated:

```text
Gradient/
│
├── main.cpp                  # Entry point (Main loop, GLFW, GLEW, and ImGui)
├── gradient.h                # Global state hub and configurations
│
├── shaders/                  # Shaders module
│   ├── shaders.h             # Raw GLSL source strings (Vertex and Fragment shaders)
│   ├── programs.h            # OpenGL program declarations
│   └── programs.cpp          # Shader program compilation and initialization logic
│
├── utils/                    # Toolset module
│   ├── utils.h               # Declarations of helper functions (loaders, creators)
│   └── utils.cpp             # Implementation of OpenGL support functions
│
├── ui/                       # Graphical User Interface module
│   ├── mainmenu.h            # ImGui menu declarations
│   └── mainmenu.cpp          # Layout and control widgets interface
│
└── vendor/                   # Third-party dependencies
    ├── GLFW / GLEW / GLM     # Windowing, contexts, and 3D math support
    ├── imgui/                # Immediate mode user interface
    └── stb/                  # Texture loading (stb_image)
```

---

## Core Features & Vision

* **Real-Time Shader Processing:** Apply and tweak visual filters instantly using custom OpenGL fragment shaders.
* **Modular Architecture:** Clean separation of concerns between window management, UI rendering, shader compilation, and utility math.
* **Editor Roadmap:** Expanding toward multi-layer composition, off-screen rendering via Framebuffer Objects (FBOs), dedicated tool state machines (brushes, eraser, selection), and history tracking (undo/redo).

---

## Prerequisites

To compile and run this project, you will need:

1. **Operating System:** Windows (with support for OpenGL 3.3 or higher).
2. **Development Environment:** 
   * **Option A (Recommended):** Visual Studio with the "Desktop development with C++" workload installed.
   * **Option B (Alternative):** VS Code, CMake, and a modern C++ compiler (such as GCC/MinGW or Clang).
3. **Graphics Card:** GPU with up-to-date OpenGL drivers.

---

## How to Build and Run in Visual Studio

1. **Open the Project:**
   * Open Visual Studio.
   * Select "Open a project or solution" and choose the solution file (`.sln`) inside the `Gradient` folder.

2. **Configure the Environment:**
   * In the top toolbar, set the configuration to `Debug` or `Release`.
   * Set the platform to `x64`.

3. **Build the Solution:**
   * In the top menu, click **Build > Build Solution** (or use the shortcut `Ctrl` + `Shift` + `B`).
   * Confirm that the build finishes successfully (`0 errors`).

4. **Run:**
   * Click **Debug > Start Without Debugging** (or use the shortcut `Ctrl` + `F5`).

---

## How to Build Without Visual Studio (Alternatives)

If you do not use Visual Studio, you can compile the project using alternative tools, provided you correctly configure the include paths and graphic dependencies.

### Option 1: Using VS Code and CMake (Recommended)
If you add a `CMakeLists.txt` file to the project root, you can manage GLFW, GLEW, and ImGui dependencies in an IDE-agnostic way:

1. Install the **C/C++** and **CMake Tools** extensions in VS Code.
2. Open the project root folder in VS Code.
3. Configure the build kit (e.g., GCC/MinGW or MSVC Command Line).
4. Click **Build** in the bottom status bar of VS Code, then **Run**.

### Option 2: Direct Command Line (MinGW / GCC)
If you prefer compiling manually via terminal (making sure to include all necessary `.cpp` files and link OpenGL, GLFW, and GLEW libraries):

```bash
g++ main.cpp utils/utils.cpp shaders/programs.cpp ui/mainmenu.cpp -o gradient.exe -I. -Ivendor -lglfw3 -lglew32 -lopengl32 -lgdi32
```
*(Note: The `-l` linking arguments depend on where you have installed the libraries on your system and the include paths for headers like ImGui and GLM).*

---
